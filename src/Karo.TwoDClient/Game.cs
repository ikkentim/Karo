using System;
using Karo.Common;
using Karo.Core;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using CKaro = Karo.Core.Karo;

namespace Karo.TwoDClient
{
    /// <summary>
    ///     This is the main type for your game
    /// </summary>
    public class Game : Microsoft.Xna.Framework.Game
    {
        private readonly Camera2D _camera = new Camera2D(-200, -200);
        private readonly GraphicsDeviceManager _graphics;
        private readonly IPlayer _playerOne;
        private readonly IPlayer _playerTwo;
        private bool _isLeftMouseButtonDown;

        private IPlayer _currentPlayer;
        private CKaro _karo = new CKaro();
        private Vector2 _mousePosition;

        private Vector2 _oldMousePos = new Vector2(0, 0);
        private Tile _selectedPiece;
        private SpriteBatch _spriteBatch;
        private Vector2 _tilePosition;

        public Game(int player1Ai, int player2Ai)
        {
            if (player1Ai == 0)
            {
                _playerOne = new HumanPlayer(Player.Player1);
            } // else new CPU player
            if (player2Ai == 0)
            {
                _playerTwo = new HumanPlayer(Player.Player2);
            } // else new CPU player

            _graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";
            IsMouseVisible = true;
            IsFixedTimeStep = false;
        }

        /// <summary>
        ///     Gets the current turn.
        /// </summary>
        public Player CurrentTurn
        {
            get
            {
                return _currentPlayer == _playerOne
                    ? Player.Player1
                    : _currentPlayer == _playerTwo ? Player.Player2 : Player.None;
            }
        }

        /// <summary>
        /// Gets a value indicating whether the current player is human.
        /// </summary>
        public bool IsCurrentPlayerHuman
        {
            get { return _currentPlayer is HumanPlayer; }
        }

        public bool IsInFirstPhase
        {
            get { return _karo.PieceCount() < 12; }
        }

        /// <summary>
        ///     Initializes this instance.
        /// </summary>
        protected override void Initialize()
        {
            // TODO: Add your initialization logic here

            _currentPlayer = _playerOne;
            _playerOne.DoMove(null, 0, Done);

            base.Initialize();
        }

        /// <summary>
        ///     Gets the instance of the specified player.
        /// </summary>
        /// <param name="player">The player.</param>
        /// <returns>Instance of the specified player.</returns>
        private IPlayer GetPlayer(Player player)
        {
            switch (player)
            {
                case Player.Player1:
                    return _playerOne;
                case Player.Player2:
                    return _playerTwo;
                default:
                    return null;
            }
        }

        /// <summary>
        ///     Gets the opponent of the specified player.
        /// </summary>
        /// <param name="player">The player.</param>
        /// <returns>The opponent of the specified player.</returns>
        private static Player GetOpponent(Player player)
        {
            switch (player)
            {
                case Player.Player1:
                    return Player.Player2;
                case Player.Player2:
                    return Player.Player1;
                default:
                    return Player.None;
            }
        }

        /// <summary>
        ///     Completes the move of the current player.
        /// </summary>
        /// <param name="move">The move.</param>
        private void Done(Move move)
        {
            _karo = _karo.WithMoveApplied(move, CurrentTurn);

            _currentPlayer = GetPlayer(GetOpponent(CurrentTurn));
            _currentPlayer.DoMove(move, 0, Done);
        }

        /// <summary>
        ///     LoadContent will be called once per game and is the place to load
        ///     all of your content.
        /// </summary>
        protected override void LoadContent()
        {
            // Create a new SpriteBatch, which can be used to draw textures.
            _spriteBatch = new SpriteBatch(GraphicsDevice);

            Textures.Load(Content);
        }

        /// <summary>
        ///     UnloadContent will be called once per game and is the place to unload
        ///     all content.
        /// </summary>
        protected override void UnloadContent()
        {
            // TODO: Unload any non ContentManager content here
        }

        /// <summary>
        ///     Allows the game to run logic such as updating the world,
        ///     checking for collisions, gathering input, and playing audio.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Update(GameTime gameTime)
        {
            // Allows the game to exit
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed ||
                Keyboard.GetState().IsKeyDown(Keys.Escape))
                Exit();

            //get the current mouse state, save to vector				
            var ms = Mouse.GetState();

            _mousePosition = new Vector2(ms.X, ms.Y);

            //update the camera's position based on how far the right mouse button has been dragged since last update
            var move = _oldMousePos - new Vector2(ms.X, ms.Y);
            if (ms.RightButton == ButtonState.Pressed)
            {
                _camera.Move(move);
            }
            _oldMousePos = new Vector2(ms.X, ms.Y);


            //since the mouse click location is relative to the camera we need to get the absolute position in the playing field
            var camPos = _camera.position;
            var absPos = _mousePosition + camPos;
            _tilePosition = new Vector2((int) Math.Floor(absPos.X/50), (int) Math.Floor(absPos.Y/50));

            var human = _currentPlayer as HumanPlayer;

            if (ms.LeftButton == ButtonState.Pressed && !_isLeftMouseButtonDown && human != null)
            {
                _isLeftMouseButtonDown = true;

                //get the tile at the mouse location
                var tile = _karo.GetTileAt((int) _tilePosition.X, (int) _tilePosition.Y);

                //if the clicked tile is not empty
                if (_selectedPiece == null)
                {
                    if (IsInFirstPhase)
                    {
                        if (tile != null)
                            human.PrepareMove(new Move(tile.X, tile.Y, 0, 0, 0, 0));
                    }
                    else
                    {
                        var piece = _karo.GetPiece(tile.X, tile.Y);

                        if (piece != null && piece.Player == CurrentTurn)
                            _selectedPiece = tile;
                    }
                }
                else
                {
                    //make a move
                    human.PrepareMove(new Move((int)_tilePosition.X, (int)_tilePosition.Y, _selectedPiece.X, _selectedPiece.Y, 0, 0));
                    _selectedPiece = null;
                }
            }
            if (ms.LeftButton == ButtonState.Released)
            {
                _isLeftMouseButtonDown = false;
            }

            base.Update(gameTime);
        }


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
                _camera.get_transform(_graphics));

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
                var coor = new Vector2(tile.X*(50 + 1), tile.Y*(50 + 1));

                _spriteBatch.Draw(Textures.tileTex, coor, Color.White);
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
                        var coord = new Vector2(piece.Tile.X*50, piece.Tile.Y*50);
                        if (piece.Player == Player.Player1)
                            if (piece.IsFaceUp)
                            {
                                _spriteBatch.Draw(Textures.redTex, coord, Color.White);
                            }
                            else
                            {
                                _spriteBatch.Draw(Textures.redMarkTex, coord, Color.White);
                            }
                        if (piece.Player == Player.Player2)
                        {
                            if (piece.IsFaceUp)
                            {
                                _spriteBatch.Draw(Textures.whiteTex, coord, Color.White);
                            }
                            else
                            {
                                _spriteBatch.Draw(Textures.whiteMarkTex, coord, Color.White);
                            }
                        }
                    }
                    else
                    {
                        _spriteBatch.Draw(Textures.whiteTex, new Vector2(-100, (counter*10)), Color.White);
                    }
                    counter++;
                }
            else
            {
                _spriteBatch.Draw(Textures.redTex, new Vector2(50, 50), Color.White);
            }
        }

        private void DrawUI()
        {
            var marker = _tilePosition*51;

            _spriteBatch.Draw(Textures.cursorTex, marker, Color.White);

            _spriteBatch.Draw(Textures.redTex, _camera.position + new Vector2(200, 25), Color.White);
            _spriteBatch.Draw(Textures.whiteTex, _camera.position + new Vector2(250, 25), Color.White);

            if (CurrentTurn == Player.Player1)
            {
                _spriteBatch.Draw(Textures.turnIndicator, _camera.position + new Vector2(200, 75), Color.White);
            }
            else
            {
                _spriteBatch.Draw(Textures.turnIndicator, _camera.position + new Vector2(250, 75), Color.White);
            }
            if (_selectedPiece != null)
            {
                _spriteBatch.Draw(Textures.selectIndicator, new Vector2(_selectedPiece.X*51, _selectedPiece.Y*51),
                    Color.White);
            }
        }
    }
}