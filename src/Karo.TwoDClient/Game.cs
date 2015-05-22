using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using Karo.Common;
using Karo.Core;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;

namespace Karo.TwoDClient
{
    /// <summary>
    ///     This is the main type for your game
    /// </summary>
    public class Game : Microsoft.Xna.Framework.Game
    {
        private const int TileSize = 50;
        private readonly Camera2D _camera = new Camera2D(-200, -200);
        private readonly List<Move> _history = new List<Move>();
        private readonly IPlayer _player1;
        private readonly IPlayer _player2;
        private bool _awaitingMove = true;
        private IPlayer _currentPlayer;
        private SpriteFont _font;
        private GraphicsDeviceManager _graphics;
        private bool _isLeftMouseButtonDown;
        private bool _isThinking;
        private KaroBoardState _karo = new KaroBoardState();
        private Move _lastMove;
        private TimeSpan _lastMoveTime;
        private Vector2 _oldMousePos = new Vector2(0, 0);
        private Position _selectedNewPiece;
        private Position _selectedOldPiece;
        private SpriteBatch _spriteBatch;
        private Vector2 _tilePosition;

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
        ///     Initializes this instance.
        /// </summary>
        protected override void Initialize()
        {
            _currentPlayer = _player1;
            _currentPlayer.DoMove(null, 0, Done);

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
        ///     LoadContent will be called once per game and is the place to load
        ///     all of your content.y
        /// </summary>
        protected override void LoadContent()
        {
            // Create a new SpriteBatch, which can be used to draw textures.
            _spriteBatch = new SpriteBatch(GraphicsDevice);

            _font = Content.Load<SpriteFont>("Spritefont1");

            Textures.Load(Content);
        }

        /// <summary>
        ///     UnloadContent will be called once per game and is the place to unload
        ///     all content.
        /// </summary>
        protected override void UnloadContent()
        {
            _spriteBatch.Dispose();
        }

        /// <summary>
        ///     Completes the move of the current player.
        /// </summary>
        /// <param name="move">The move.</param>
        private void Done(Move move)
        {
            _isThinking = false;

            _history.Add(move);
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
        ///     Allows the game to run logic such as updating the world,
        ///     checking for collisions, gathering input, and playing audio.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Update(GameTime gameTime)
        {
            // Allows the game to exit
            if (Keyboard.GetState().IsKeyDown(Keys.Escape))
            {
                Exit();
            }

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
            _tilePosition = new Vector2((int) Math.Floor(absolutePosition.X/TileSize),
                (int) Math.Floor(absolutePosition.Y/TileSize));

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
                var tile = _karo.GetTile((int) _tilePosition.X, (int) _tilePosition.Y);

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
                        var corner = new Position((int) _tilePosition.X, (int) _tilePosition.Y);

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
                        _selectedNewPiece = new Position((int) _tilePosition.X, (int) _tilePosition.Y);

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

            _spriteBatch.Begin(SpriteSortMode.Deferred,
                BlendState.AlphaBlend,
                null,
                null,
                null,
                null,
                _camera.Transform);

            DrawTiles();

            DrawPieces();

            DrawUI();

            _spriteBatch.End();

            base.Draw(gameTime);
        }

        private void DrawTiles()
        {
            foreach (var tile in _karo.Tiles)
            {
                var coor = new Vector2(tile.X*(TileSize + 1), tile.Y*(TileSize + 1));

                _spriteBatch.Draw(Textures.Tile, coor, Color.White);
            }
        }

        private void DrawPieces()
        {
            var counter = 0;
            if (_karo.Pieces != null)
                foreach (var piece in _karo.Pieces)
                {
                    if (piece != null)
                    {
                        var coord = new Vector2(piece.X*(TileSize + 1), piece.Y*(TileSize + 1));

                        if (piece.Player == KaroPlayer.Player1)
                            _spriteBatch.Draw(piece.IsFaceUp ? Textures.RedPieceMarked : Textures.RedPiece, coord,
                                Color.White);

                        if (piece.Player == KaroPlayer.Player2)
                            _spriteBatch.Draw(piece.IsFaceUp ? Textures.WhitePieceMarked : Textures.WhitePiece, coord,
                                Color.White);
                        
                    }
                    else
                    {
                        _spriteBatch.Draw(Textures.WhitePiece, new Vector2(-100, (counter*10)), Color.White);
                    }
                    counter++;
                }
            else
            {
                _spriteBatch.Draw(Textures.RedPiece, new Vector2(TileSize, TileSize), Color.White);
            }
        }

        private void DrawUI()
        {
            var marker = _tilePosition * (TileSize + 1);

            _spriteBatch.Draw(Textures.Cursor, marker, Color.White);

            _spriteBatch.Draw(Textures.RedPiece, _camera.Position + new Vector2(200, 25), Color.White);
            _spriteBatch.Draw(Textures.WhitePiece, _camera.Position + new Vector2(250, 25), Color.White);

            _spriteBatch.Draw(Textures.TurnIndicator,
                _camera.Position + new Vector2(CurrentTurn == KaroPlayer.Player1 ? 200 : 250, 75), Color.White);

            if (_selectedOldPiece != null)
            {
                _spriteBatch.Draw(Textures.SelectIndicator, new Vector2(_selectedOldPiece.X*51, _selectedOldPiece.Y*51),
                    Color.White);
            }
            if (_selectedNewPiece != null)
            {
                _spriteBatch.Draw(Textures.Piece, new Vector2(_selectedNewPiece.X*51, _selectedNewPiece.Y*51),
                    Color.White);
            }

            var i = 0;
            foreach (var move in Enumerable.Reverse(_history).Take(10))
            {
                var str = "Piece from " + move.OldPieceX + "," + move.OldPieceY + " to " + move.NewPieceX + "," +
                       move.NewPieceY;

                _spriteBatch.DrawString(_font, str, _camera.Position + new Vector2(5, 5), Color.Red, 0f,
                    new Vector2(0, i*-50), new Vector2(0.3f), new SpriteEffects(), 0f);

                i++;
            }

            if (_isThinking)
            {
                _spriteBatch.DrawString(_font, "Thinking...",
                    _camera.Position + new Vector2(200,5), Color.Bisque, 0, Vector2.Zero, new Vector2(0.4f), SpriteEffects.None, 0);
            }
        }

        #endregion
    }
}