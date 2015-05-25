using System;
using Karo.Common;
using Karo.Core;
using Karo.ThreeDDClient;
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
                new Vector3(3, 3, 3),
                new Vector3(0, .5f, 0),
                Vector3.Up
            );

            _projection = Matrix.CreatePerspectiveFieldOfView(
                MathHelper.ToRadians(70),
                1.3f,
                .1f,
                10f
            );

            base.Initialize();
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

            // TODO: Add your update logic here

            base.Update(gameTime);
        }

        /// <summary>
        /// This is called when the game should draw itself.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.CornflowerBlue);
            
            foreach (ModelMesh modelMesh in _whitePawn.Meshes)
            {
                foreach (BasicEffect effect in modelMesh.Effects)
                {
                    effect.World *= _world;
                    effect.View = _view;
                    effect.Projection = _projection;
                    effect.EnableDefaultLighting();

                }

                modelMesh.Draw();
            }

            base.Draw(gameTime);
        }
    }
}
