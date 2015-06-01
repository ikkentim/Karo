using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing.Imaging;
using System.Linq;
using System.Threading;
using System.Windows.Forms;
using System.Xml.Schema;
using Karo.Common;
using Karo.Core;
using Karo.ThreeDClient;
using Karo.ThreeDDClient;
using KaroThreeDClient.Components;
using KaroThreeDClient.Services;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using ButtonState = Microsoft.Xna.Framework.Input.ButtonState;
using Keys = Microsoft.Xna.Framework.Input.Keys;
using Tile = Karo.Core.Tile;

namespace KaroThreeDClient
{
    /// <summary>
    /// This is the main type for your game
    /// </summary>
    public class Game : Microsoft.Xna.Framework.Game
    {
        GraphicsDeviceManager _graphics;
        SpriteBatch spriteBatch;

        private IPlayer _player1;
        private IPlayer _player2;
        private IPlayer _currentPlayer;
        public KaroBoardState _karo = new KaroBoardState();

        public Model TileModel;
        public Model RedPawnModel;
        public Model WhitePawnModel;
        public Model NyanCat;

        private bool _awaitingMove = false;
        private bool _isThinking;
        private readonly Camera3D _camera = new Camera3D(-200, -200);

        private Move _lastMove;
        private TimeSpan _lastMoveTime;
        private Vector2 _oldMousePos = new Vector2(0, 0);
        private Position _selectedNewPiece;
        private Position _selectedOldPiece;
        private SpriteBatch _spriteBatch;
        private Vector2 _tilePosition;
        public float TileSize = 1.05f;
        private bool _isLeftMouseButtonDown;
        private int _beforeEvaluationScore;
        private Stopwatch _moveTime = new Stopwatch();
        public CameraService CameraService;
        public ConsoleService ConsoleService;
        private MouseState _lastMouseState;
        private int _lastScroll;
        private float _unprocessedScrollDelta;

        private const float ScrollMaxSpeed = 0.3f;
        private const float ScrollMultiplier = 0.01f;
        private const float ScrollMinDelta = 0.05f;
        private const float ScrollModifier = 2.5f;
        private float _scrollVelocity;

        public bool Animating;

        public bool IsSelectingCornerTile;
        public bool IsSelectingGhostTile;

        private Thread _aiThread;
        private bool _exiting;

        public Game(IPlayer player1, IPlayer player2)
        {
            if (player1 == null) throw new ArgumentNullException("player1");
            if (player2 == null) throw new ArgumentNullException("player2");

            _player1 = player1;
            _player2 = player2;

            _graphics = new GraphicsDeviceManager(this);

            _graphics.PreferredBackBufferWidth = GraphicsAdapter.DefaultAdapter.CurrentDisplayMode.Width - 50;
            _graphics.PreferredBackBufferHeight = GraphicsAdapter.DefaultAdapter.CurrentDisplayMode.Height - 100;

            //_graphics.IsFullScreen = true;

            _graphics.ApplyChanges();

            //_graphics.PreferMultiSampling = true;
        }

        /// <summary>
        ///     Gets the current turn.
        /// </summary>
        public KaroPlayer CurrentTurn
        {
            get
            {
                return _currentPlayer == _player1
                    ? KaroPlayer.Player1
                    : _currentPlayer == _player2 ? KaroPlayer.Player2 : KaroPlayer.None;
            }
        }

        /// <summary>
        ///     Gets a value indicating whether the current player is human.
        /// </summary>
        public bool IsCurrentPlayerHuman
        {
            get { return _currentPlayer is HumanPlayer; }
        }

        /// <summary>
        ///     Gets a value indicating whether the game is in the first phase.
        /// </summary>
        public bool IsInFirstPhase
        {
            get { return _karo.PieceCount < 12; }
        }


        /// <summary>
        /// Allows the game to perform any initialization it needs to before starting to run.
        /// This is where it can query for any required services and load any non-graphic
        /// related content.  Calling base.Initialize will enumerate through any components
        /// and initialize them as well.
        /// </summary>
        protected override void Initialize()
        {
            Content.RootDirectory = "Content";
            IsMouseVisible = true;

            foreach (Tile tile in _karo.Tiles)
                Components.Add(new Plate(this, tile));

            Services.AddService(typeof(CameraService), CameraService = new CameraService(this));
            Services.AddService(typeof(ConsoleService), ConsoleService = new ConsoleService(this));
            Components.Add(CameraService);
            Components.Add(ConsoleService);

            Components.Add(new NyanCat(this));
            Components.Add(new Turn(this));

            base.Initialize();
        }

        /// <summary>
        ///     Gets the instance of the specified player.
        /// </summary>
        /// <param name="player">The player.</param>
        /// <returns>Instance of the specified player.</returns>
        private IPlayer GetPlayer(KaroPlayer player)
        {
            switch (player)
            {
                case KaroPlayer.Player1:
                    return _player1;
                case KaroPlayer.Player2:
                    return _player2;
                default:
                    return null;
            }
        }

        /// <summary>
        ///     Gets the opponent of the specified player.
        /// </summary>
        /// <param name="player">The player.</param>
        /// <returns>The opponent of the specified player.</returns>
        private static KaroPlayer GetOpponent(KaroPlayer player)
        {
            switch (player)
            {
                case KaroPlayer.Player1:
                    return KaroPlayer.Player2;
                case KaroPlayer.Player2:
                    return KaroPlayer.Player1;
                default:
                    return KaroPlayer.None;
            }
        }

        private int? EvaluateCurrentPlayer()
        {
            if (_currentPlayer == null || _currentPlayer is HumanPlayer) return null;

            var evaluate = _currentPlayer.GetType().GetMethod("Evaluate");
            if (evaluate.GetParameters().Length != 0 || evaluate.ReturnType != typeof (int))
                return null;

            return (int) evaluate.Invoke(_currentPlayer, null);
        }
        /// <summary>
        ///     Completes the move of the current player.
        /// </summary>
        /// <param name="move">The move.</param>
        private void Done(Move move)
        {
            if (_exiting)
            {
                return;
            }

            _isThinking = false;
            
            if (IsInFirstPhase)
            {
                _karo.ApplyMove(move, CurrentTurn);
                Components.Add(new Pawn(this, _karo.Pieces.Last(p =>p != null)));

                ConsoleService.WriteChatLine(Color.Wheat, "{0} placed ({1}, {2})", CurrentTurn, move.NewPieceX, move.NewPieceY);
            }
            else
            {
                _karo.ApplyMove(move, CurrentTurn);

                ConsoleService.WriteChatLine(Color.Wheat, "{0} moved ({1}, {2}) to ({3}, {4})", CurrentTurn, move.OldPieceX, move.OldPieceY, move.NewPieceX, move.NewPieceY);
            }

            ConsoleService.WriteLine(Color.White, "Move took {0}", _moveTime.Elapsed);
            int? evaluation = EvaluateCurrentPlayer();
            if (evaluation != null)
            {
                ConsoleService.WriteLine(Color.White, "Move changed score from {0} to {1}",
                    _beforeEvaluationScore, evaluation);

                var evaluation_count = _currentPlayer.GetType().GetProperty("evaluation_count");

                if(evaluation_count != null)
                    ConsoleService.WriteLine(Color.White, "Evaluation count {0}", evaluation_count.GetValue(_currentPlayer, null));
            }

            UpdateTileData();

            _lastMove = move;
            
            var winner = _karo.GetWinner();
            if (winner == KaroPlayer.None)
            {
                _currentPlayer = GetPlayer(GetOpponent(CurrentTurn));

                if (IsCurrentPlayerHuman)
                {
                    _moveTime.Restart();
                    _currentPlayer.DoMove(_lastMove, 0, Done);
                }
                else
                    _awaitingMove = true;

                // Calculate evaluated score for debugging
                _beforeEvaluationScore = EvaluateCurrentPlayer() ?? 0;
            }
            else
            {
                _currentPlayer = null;
                ConsoleService.WriteChatLine(Color.White, "There is a Winner!");
            }
        }

        private void UpdateTileData()
        {
            GhostPlate[] plates = Components.OfType<GhostPlate>().ToArray();
            
            foreach (GhostPlate plate in plates)
                Components.Remove(plate);

            List<Tile> ghostTiles = new List<Tile>();

            foreach (Tile tile in _karo.Tiles)
            {
                Components.OfType<Plate>().First(p => p.Tile == tile).IsCornerTile = _karo.IsCornerTile(tile.X, tile.Y);

                for (int x = -1; x <= 1; x++)
                {
                    for (int y = -1; y <= 1; y++)
                    {
                        if(Math.Abs(x) == Math.Abs(y))
                            continue;

                        Tile ghostTile = new Tile(tile.X + x, tile.Y + y);

                        if (!ghostTiles.Any(t => t.X == ghostTile.X && t.Y == ghostTile.Y) && !_karo.Tiles.Any(t => t.X == ghostTile.X && t.Y == ghostTile.Y))
                            ghostTiles.Add(ghostTile);
                    }
                    
                }
            }

            foreach (Tile ghostTile in ghostTiles)
            {
                Components.Add(new GhostPlate(this, ghostTile));
            }
        }


        /// <summary>
        /// LoadContent will be called once per game and is the place to load
        /// all of your content.
        /// </summary>
        protected override void LoadContent()
        {
            // Create a new SpriteBatch, which can be used to draw textures.
            spriteBatch = new SpriteBatch(GraphicsDevice);

            TileModel = Content.Load<Model>("tile");
            RedPawnModel = Content.Load<Model>("red");
            WhitePawnModel = Content.Load<Model>("white");
            NyanCat = Content.Load<Model>("nyan");
        }

        /// <summary>
        /// UnloadContent will be called once per game and is the place to unload
        /// all content.
        /// </summary>
        protected override void UnloadContent()
        {
            TileModel = null;
            RedPawnModel = null;
            WhitePawnModel = null;
            NyanCat = null;
        }

        /// <summary>
        /// Allows the game to run logic such as updating the world,
        /// checking for collisions, gathering input, and playing audio.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Update(GameTime gameTime)
        {
            if (Keyboard.GetState().IsKeyDown(Keys.Escape))
            {
                _exiting = true;

                if (_aiThread != null)
                {
                    _aiThread.Interrupt();
                    _aiThread.Abort();
                }

                Exit();
            }

            if (_currentPlayer == null)
            {

                _currentPlayer = _player1;
                _moveTime.Restart();

                _aiThread = new Thread(() =>
                {
                    _isThinking = true;
                    _moveTime.Restart();
                    _currentPlayer.DoMove(null, 0, Done);
                });

                _aiThread.Start();
            }
            if (_awaitingMove)
            {
                if (IsCurrentPlayerHuman)
                    _awaitingMove = false;
                else
                {
                    _lastMoveTime += gameTime.ElapsedGameTime;
                    if (_lastMoveTime > new TimeSpan(0, 0, 0, 0, 500) && !Animating)
                    {
                        _lastMoveTime = TimeSpan.Zero;
                        _awaitingMove = false;
                        new Thread(() =>
                        {
                            _isThinking = true;
                            _moveTime.Restart();
                            _currentPlayer.DoMove(_lastMove, 0, Done);
                        }).Start();
                    }
                }
            }
	
            var mouseState = Mouse.GetState();

            var human = _currentPlayer as HumanPlayer;

            #region Choosing Moves
            // cancel move on right click
            if (IsActive && mouseState.RightButton == ButtonState.Pressed)
                CancelMove();

            if (IsActive && mouseState.LeftButton == ButtonState.Pressed && human != null && !Animating)
            {
                Vector3 near = _graphics.GraphicsDevice.Viewport.Unproject(new Vector3(mouseState.X, mouseState.Y, 0), CameraService.Projection, CameraService.View, Matrix.Identity);
                Vector3 far = _graphics.GraphicsDevice.Viewport.Unproject(new Vector3(mouseState.X, mouseState.Y, 1), CameraService.Projection, CameraService.View, Matrix.Identity);

                Ray ray = new Ray(near, Vector3.Normalize(far - near));

                float dist = float.MaxValue;
                Plate selected = null;

                foreach (var component in Components)
                {
                    if (component is Plate)
                    {
                        Plate plate = component as Plate;

                        float? newDist = ray.Intersects(plate.BoundingBox);

                        Piece piece = _karo.GetPiece(plate.Tile.X, plate.Tile.Y);

                        if (newDist != null && newDist < dist && (piece == null || piece.Player == CurrentTurn))
                        {
                            dist = newDist.Value;
                            selected = plate;
                        }
                    }
                }

                foreach (Plate component in Components.OfType<Plate>())
                {
                    component.IsSelected = component == selected;
                }


                if (selected != null)
                {
                    // if the game is in the first phase, just place a new piece on the selected tile
                    if (IsInFirstPhase)
                    {
                        human.PrepareMove(new Move(selected.Tile.X, selected.Tile.Y, 0, 0, 0, 0));

                        selected.IsSelected = false;
                    }
                    // if we're not in the first phase, we need to move pieces, check if we have not select a piece to be moved
                    else if (_selectedOldPiece == null)
                    {
                        // see if the selected tile has a piece, abort otherwise
                        if (_karo.GetPiece(selected.Tile.X, selected.Tile.Y) != null)
                        {
                            // set piece to be moved
                            _selectedOldPiece = new Position(selected.Tile.X, selected.Tile.Y);

                            IsSelectingGhostTile = true;
                        }
                        else
                        {
                            // MISSION ABORT!!!
                            CancelMove();
                        }
                    }
                    else if (_selectedNewPiece != null)
                    {
                        if (_karo.CornerTiles.Any(t => t == selected.Tile))
                        {
                            human.PrepareMove(new Move(_selectedNewPiece.X, _selectedNewPiece.Y, _selectedOldPiece.X, _selectedOldPiece.Y, selected.Tile.X, selected.Tile.Y));

                            CancelMove();
                        }
                    }
                    // we are not in the first phase and we chose a piece to be moved, select the tile to be moved to
                    else
                    {
                        if (selected is GhostPlate)
                        {
                            _selectedNewPiece = new Position(selected.Tile.X, selected.Tile.Y);

                            IsSelectingCornerTile = true;
                        }
                        else if (_karo.GetPiece(selected.Tile.X, selected.Tile.Y) == null && _selectedNewPiece == null)
                        {
                            human.PrepareMove(new Move(selected.Tile.X, selected.Tile.Y, _selectedOldPiece.X, _selectedOldPiece.Y, 0, 0));

                            CancelMove();
                        }
                    }
                }
            }
#endregion

            #region Scrolling

            float deltaCameraZoom = 0;

            // Calculate the delta scroll value.
            var scroll = mouseState.ScrollWheelValue;
            var deltaScroll = scroll - _lastScroll;
            _lastScroll = scroll;

            // Add the delta scroll to the unprocessed scroll value. 
            _unprocessedScrollDelta -= deltaScroll;

            // If there is some reasonable amount of unprocessed scroll, handle it.
            if (Math.Abs(_unprocessedScrollDelta) > ScrollMinDelta)
            {
                _scrollVelocity = MathHelper.Clamp(_unprocessedScrollDelta * ScrollMultiplier, -ScrollMaxSpeed,
                    ScrollMaxSpeed);

                _unprocessedScrollDelta -= _scrollVelocity / ScrollMultiplier;

                deltaCameraZoom += (_scrollVelocity * CameraService.Zoom /
                                   (CameraService.MaxZoom - CameraService.MinZoom + 1)) * ScrollModifier;
            }

            #endregion

            #region Movement
            Vector3 acceleration = Vector3.Zero;
            float deltaCameraRotation = 0;
            if (Keyboard.GetState().IsKeyDown(Keys.A)) acceleration += Vector3.Backward;
            if (Keyboard.GetState().IsKeyDown(Keys.D)) acceleration += Vector3.Forward;
            if (Keyboard.GetState().IsKeyDown(Keys.W)) acceleration += Vector3.Left;
            if (Keyboard.GetState().IsKeyDown(Keys.S)) acceleration += Vector3.Right;

            if (Keyboard.GetState().IsKeyDown(Keys.Q)) deltaCameraRotation += 2.5f * (float)gameTime.ElapsedGameTime.TotalSeconds;
            if (Keyboard.GetState().IsKeyDown(Keys.E)) deltaCameraRotation -= 2.5f * (float)gameTime.ElapsedGameTime.TotalSeconds;
            #endregion 

            CameraService.AddVelocity(acceleration * (float)gameTime.ElapsedGameTime.TotalSeconds);
            CameraService.Move(deltaCameraRotation, deltaCameraZoom);

            base.Update(gameTime);
        }

        private void CancelMove()
        {
            _selectedNewPiece = null;
            _selectedOldPiece = null;
            IsSelectingGhostTile = false;
            IsSelectingCornerTile = false;
            
            foreach (Plate tile in Components.OfType<Plate>())
            {
                tile.IsSelected = false;
            }
        }

        /// <summary>
        ///     This is called when the game should draw itself.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.CornflowerBlue);

            base.Draw(gameTime);
        }
    }
}
