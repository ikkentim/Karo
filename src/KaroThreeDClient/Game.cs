using System;
using System.Linq;
using System.Threading;
using Karo.Common;
using Karo.Core;
using Karo.ThreeDClient;
using Karo.ThreeDDClient;
using KaroThreeDClient.Services;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;

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
        private KaroBoardState _karo = new KaroBoardState();
        private BasicEffect _effect;
        private Matrix _world;
        private Matrix _view;
        private Matrix _projection;

        private Model _tile;
        private Model _redPawn;
        private Model _whitePawn;

        private bool _awaitingMove = true;
        private bool _isThinking;
        private readonly Camera3D _camera = new Camera3D(-200, -200);

        private Move _lastMove;
        private TimeSpan _lastMoveTime;
        private Vector2 _oldMousePos = new Vector2(0, 0);
        private Position _selectedNewPiece;
        private Position _selectedOldPiece;
        private SpriteBatch _spriteBatch;
        private Vector2 _tilePosition;
        private float TileSize = 2.1f;
        private bool _isLeftMouseButtonDown;

        private CameraService _cameraService;
        private MouseState _lastMouseState;
        private int _lastScroll;
        private float _unprocessedScrollDelta;

        private const float ScrollMaxSpeed = 0.3f;
        private const float ScrollMultiplier = 0.01f;
        private const float ScrollMinDelta = 0.05f;
        private const float ScrollModifier = 2.5f;
        private const float MouseRotationModifier = 0.005f;
        private float _scrollVelocity;

        public Game(IPlayer player1, IPlayer player2)
        {
            if (player1 == null) throw new ArgumentNullException("player1");
            if (player2 == null) throw new ArgumentNullException("player2");

            _player1 = player1;
            _player2 = player2;
            
            _graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";
            IsMouseVisible = true;
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
            _effect = new BasicEffect(GraphicsDevice);

            _world = Matrix.Identity;
            _view = Matrix.CreateLookAt(
                new Vector3(5, 5, 5),
                new Vector3(0, .5f, 0),
                Vector3.Up
            );

            _projection = Matrix.CreatePerspectiveFieldOfView(
                MathHelper.ToRadians(70),
                1.3f,
                .1f,
                10f
            );

            _currentPlayer = _player1;
            _currentPlayer.DoMove(null, 0, Done);

            Services.AddService(typeof(CameraService), _cameraService = new CameraService(this));
            Components.Add(_cameraService);

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
        
        /// <summary>
        ///     Completes the move of the current player.
        /// </summary>
        /// <param name="move">The move.</param>
        private void Done(Move move)
        {
            _isThinking = false;

            //_history.Add(move);
            _lastMove = move;
            _karo.ApplyMove(move, CurrentTurn);

            var winner = _karo.GetWinner();
            if (winner == KaroPlayer.None)
            {
                _currentPlayer = GetPlayer(GetOpponent(CurrentTurn));

                if (IsCurrentPlayerHuman)
                    _currentPlayer.DoMove(_lastMove, 0, Done);
                else
                    _awaitingMove = true;
            }
            else
            {
                _currentPlayer = null;
                Console.WriteLine("There is a winner!!");
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

            _tile = Content.Load<Model>("tile");
            _redPawn = Content.Load<Model>("red");
            _whitePawn = Content.Load<Model>("white");
        }

        /// <summary>
        /// UnloadContent will be called once per game and is the place to unload
        /// all content.
        /// </summary>
        protected override void UnloadContent()
        {
            // TODO: Unload any non ContentManager content here
        }

        /// <summary>
        /// Allows the game to run logic such as updating the world,
        /// checking for collisions, gathering input, and playing audio.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Update(GameTime gameTime)
        {
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed || Keyboard.GetState().IsKeyDown(Keys.Escape))
                Exit();

            // 
            if (_awaitingMove)
            {
                if (IsCurrentPlayerHuman)
                    _awaitingMove = false;
                else
                {
                    _lastMoveTime += gameTime.ElapsedGameTime;
                    if (_lastMoveTime > new TimeSpan(0, 0, 0, 0, 500))
                    {
                        _lastMoveTime = TimeSpan.Zero;
                        _awaitingMove = false;
                        new Thread(() =>
                        {
                            _isThinking = true;
                            _currentPlayer.DoMove(_lastMove, 0, Done);
                        }).Start();
                    }
                }
            }

            //get the current mouse state			
            var mouseState = Mouse.GetState();
            var mousePosition = new Vector2(mouseState.X, mouseState.Y);

            //update the camera's position based on how far the right mouse button has been dragged since last update
            var move = _oldMousePos - new Vector2(mouseState.X, mouseState.Y);
            if (mouseState.RightButton == ButtonState.Pressed)
                _camera.Move(move);

            _oldMousePos = mousePosition;


            //since the mouse click location is relative to the camera we need to get the absolute position in the playing field
            var absolutePosition = mousePosition + _camera.Position;
            _tilePosition = new Vector2((int)Math.Floor(absolutePosition.X / TileSize),
                (int)Math.Floor(absolutePosition.Y / TileSize));

            var human = _currentPlayer as HumanPlayer;

            if (IsActive && mouseState.RightButton == ButtonState.Pressed)
            {
                _selectedNewPiece = null;
                _selectedOldPiece = null;
            }
            if (IsActive && mouseState.LeftButton == ButtonState.Pressed && !_isLeftMouseButtonDown && human != null)
            {
                _isLeftMouseButtonDown = true;

                //get the tile at the mouse location
                var tile = _karo.GetTile((int)_tilePosition.X, (int)_tilePosition.Y);

                //if the clicked tile is not empty
                if (_selectedOldPiece == null)
                {
                    if (IsInFirstPhase)
                    {
                        if (tile != null)
                            human.PrepareMove(new Move(tile.X, tile.Y, 0, 0, 0, 0));
                    }
                    else
                    {
                        if (tile != null)
                        {
                            var piece = _karo.GetPiece(tile.X, tile.Y);

                            if (piece != null && piece.Player == CurrentTurn)
                                _selectedOldPiece = new Position(tile.X, tile.Y);
                        }
                        else
                        {
                            _selectedOldPiece = null;
                            _selectedNewPiece = null;
                        }
                    }
                }
                else
                {
                    if (_selectedNewPiece != null)
                    {
                        var corner = new Position((int)_tilePosition.X, (int)_tilePosition.Y);

                        if (_karo.GetPiece(corner.X, corner.Y) == null &&
                            _karo.IsCornerTile(corner.X, corner.Y))
                        {
                            human.PrepareMove(new Move(_selectedNewPiece.X, _selectedNewPiece.Y, _selectedOldPiece.X,
                                _selectedOldPiece.Y, corner.X, corner.Y));

                            _selectedOldPiece = null;
                            _selectedNewPiece = null;
                        }
                    }
                    else
                    {
                        _selectedNewPiece = new Position((int)_tilePosition.X, (int)_tilePosition.Y);

                        if (_karo.GetTile(_selectedNewPiece.X, _selectedNewPiece.Y) != null)
                        {
                            human.PrepareMove(new Move(_selectedNewPiece.X, _selectedNewPiece.Y, _selectedOldPiece.X,
                                _selectedOldPiece.Y, 0, 0));

                            _selectedOldPiece = null;
                            _selectedNewPiece = null;
                        }
                    }
                }
            }
            if (mouseState.LeftButton == ButtonState.Released)
            {
                _isLeftMouseButtonDown = false;
            }
            
            #region Process camera manipulation input

            float deltaCameraRotation = 0;
            float deltaCameraZoom = 0;

            // If middle or right button is pressed, hide the mouse button and track the x-axis (rotation) movements
            if (mouseState.MiddleButton == ButtonState.Pressed || mouseState.RightButton == ButtonState.Pressed)
            {
                // If this is the first update in which the button is pressed, recenter the mouse and
                // wait for the next update, in which we actually update the mouse rotation
                if (_lastMouseState.MiddleButton == ButtonState.Pressed ||
                    _lastMouseState.RightButton == ButtonState.Pressed)
                {
                    // Add rotation delta based on mouse movement
                    deltaCameraRotation += ((float)mouseState.X - Window.ClientBounds.Width / 2) * MouseRotationModifier;

                    // Recenter mouse.
                    Mouse.SetPosition(
                        Window.ClientBounds.Width / 2,
                        Window.ClientBounds.Height / 2);
                }
                else
                {
                    // Hide and center mouse.
                    IsMouseVisible = false;
                    Mouse.SetPosition(
                        Window.ClientBounds.Width / 2,
                        Window.ClientBounds.Height / 2);

                    // TODO: It might be better only to recenter the mouse when you start dragging and when you stop relocate the mouse to were you started.
                }
            }
            else if (_lastMouseState.MiddleButton == ButtonState.Pressed ||
                     _lastMouseState.RightButton == ButtonState.Pressed)
            {
                // Button is released; show mouse again.
                IsMouseVisible = true;
            }

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

                deltaCameraZoom += (_scrollVelocity * _cameraService.Zoom /
                                   (CameraService.MaxZoom - CameraService.MinZoom + 1)) * ScrollModifier;
            }

            Vector3 acceleration = Vector3.Zero;
            if (Keyboard.GetState().IsKeyDown(Keys.A)) acceleration += Vector3.Backward;
            if (Keyboard.GetState().IsKeyDown(Keys.D)) acceleration += Vector3.Forward;
            if (Keyboard.GetState().IsKeyDown(Keys.W)) acceleration += Vector3.Left;
            if (Keyboard.GetState().IsKeyDown(Keys.S)) acceleration += Vector3.Right;

            _cameraService.AddVelocity(acceleration * (float)gameTime.ElapsedGameTime.TotalSeconds);
            _cameraService.Move(deltaCameraRotation, deltaCameraZoom);

            #endregion

            base.Update(gameTime);
        }

        #region Drawing

        /// <summary>
        ///     This is called when the game should draw itself.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.CornflowerBlue);

            foreach (Tile tile in _karo.Tiles)
            {
                foreach (ModelMesh modelMesh in _tile.Meshes)
                {
                    foreach (BasicEffect effect in modelMesh.Effects)
                    {
                        effect.World = _world + Matrix.CreateTranslation(tile.X * TileSize, 0, tile.Y * TileSize);
                        effect.View = _cameraService.View;
                        effect.Projection = _cameraService.Projection;
                        effect.EnableDefaultLighting();
                    }

                    modelMesh.Draw();
                }
            }

            foreach (Piece piece in _karo.Pieces)
            {
                Model m = (piece.Player == KaroPlayer.Player1) ? _whitePawn : _redPawn;

                foreach (ModelMesh modelMesh in m.Meshes)
                {
                    foreach (BasicEffect effect in modelMesh.Effects)
                    {
                        Matrix world = _world;

                        if (piece.IsFaceUp)
                            world *= Matrix.CreateRotationX(MathHelper.ToRadians(180));

                        world *= Matrix.CreateTranslation(piece.X*TileSize/2, 0.4f, piece.Y*TileSize/2);

                        effect.World = world;
                        effect.View = _cameraService.View;
                        effect.Projection = _cameraService.Projection;
                        effect.EnableDefaultLighting();
                    }

                    modelMesh.Draw();
                }
            }
            
            base.Draw(gameTime);
        }
        #endregion
    }
}
