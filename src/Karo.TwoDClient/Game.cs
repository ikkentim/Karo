using System;
using System.Collections.Generic;
using System.Linq;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Media;
using CKaro = Karo.Core.Karo;

namespace Karo.TwoDClient
{
    /// <summary>
    /// This is the main type for your game
    /// </summary>
    public class Game : Microsoft.Xna.Framework.Game
    {
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;
        CKaro karo = new CKaro();
		Camera2D cam = new Camera2D();
		Vector2 tilePos;
		Vector2 mousePos;
		Core.Player PlayerOne;
		Core.Player PlayerTwo;

		Core.Tile selectedPiece;

		bool playerOneTurn = false;
		Vector2 oldMousePos = new Vector2(0, 0);

        bool _canPlacePiece = true;

        public Game(int player1ai, int player2ai)
        {
			//if (player1ai == 0) { PlayerOne = new HumanPlayer(); } // else new computer
			//if (player2ai == 0) { PlayerTwo = new HumanPlayer(); } // else new computer

            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";
            IsMouseVisible = true;
            IsFixedTimeStep = false;
        }

        /// <summary>
        /// Allows the game to perform any initialization it needs to before starting to run.
        /// This is where it can query for any required services and load any non-graphic
        /// related content.  Calling base.Initialize will enumerate through any components
        /// and initialize them as well.
        /// </summary>
        protected override void Initialize()
        {
            // TODO: Add your initialization logic here

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

            var core = new CKaro();

            var tiles = core.Tiles;

            Textures.Load(Content);
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
            // Allows the game to exit
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed)
                this.Exit();
			if (Keyboard.GetState().IsKeyDown(Keys.Escape)) 
			{
				this.Exit();
			}
				
            MouseState ms = Mouse.GetState();

			mousePos = new Vector2(ms.X, ms.Y);

			

			Vector2 move = oldMousePos - new Vector2(ms.X, ms.Y);
			if (ms.RightButton == ButtonState.Pressed)
			{
				cam.Move(move);
			}
			oldMousePos = new Vector2(ms.X, ms.Y);

			Vector2 camPos = cam.position;
			Vector2 absPos = mousePos + camPos;
			tilePos = new Vector2((int)Math.Floor(absPos.X / 50), (int)Math.Floor(absPos.Y / 50));

			if(ms.LeftButton == ButtonState.Pressed && _canPlacePiece) {
				Core.Tile t = FindTile((int)tilePos.X, (int)tilePos.Y);

				if(playerOneTurn) {
					if (selectedPiece == null)
					{
						selectedPiece = t;
					}
					else
					{
						//PlayerOne.DoMove();
					}
				}
				_canPlacePiece = true;
			}
			


            // TODO: Add your update logic here

            base.Update(gameTime);
        }

		Core.Tile FindTile(int x, int y)
		{
			foreach (Core.Tile tile in karo.Tiles)
			{
				if (tile.X == x && tile.Y == y)
				{
					return tile;
				}
			}
			return null;
		}

        /// <summary>
        /// This is called when the game should draw itself.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.CornflowerBlue);

			spriteBatch.Begin(SpriteSortMode.BackToFront,
						BlendState.AlphaBlend,
						null,
						null,
						null,
						null,
						cam.get_transform(graphics));

			
            foreach (Core.Tile tile in karo.Tiles)
            {
				Vector2 coor = new Vector2(tile.X * (50 + 1), tile.Y * (50 + 1));
				coor += new Vector2(200, 200);
                spriteBatch.Draw(Textures.tileTex, coor, Color.White);
            }
			
			
			int counter = 0;
			if(karo.Pieces != null)
			foreach(Core.Piece piece in karo.Pieces ) {
				if(piece.Tile != null){
					Vector2 coord = new Vector2(piece.Tile.X, piece.Tile.Y);
					if(piece.Player == Core.Player.Player1)
						if(piece.IsFaceUp){
							spriteBatch.Draw(Textures.redTex,coord, Color.White);
						} else {
							spriteBatch.Draw(Textures.redMarkTex, coord, Color.White);
						}
					if(piece.Player == Core.Player.Player2) {
						if(piece.IsFaceUp){
							spriteBatch.Draw(Textures.whiteTex,coord, Color.White);
						} else {
							spriteBatch.Draw(Textures.whiteMarkTex, coord, Color.White);
						}
					}
				} else {
					spriteBatch.Draw(Textures.whiteTex, new Vector2(50, 50 + (counter*10)), Color.White);
				}
				counter++;
			}
			else
			{
				spriteBatch.Draw(Textures.redTex, new Vector2(50, 50), Color.White);
			}
			
			Vector2 marker = tilePos * 50;

			spriteBatch.Draw(Textures.cursorTex, marker, Color.White);
			
			spriteBatch.Draw(Textures.redTex, new Vector2(200, 25), Color.White);
			spriteBatch.Draw(Textures.whiteTex, new Vector2(250, 25), Color.White);
			if(playerOneTurn){
				spriteBatch.Draw(Textures.turnIndicator, new Vector2(200, 75), Color.White);
			} else {
				spriteBatch.Draw(Textures.turnIndicator, new Vector2(250, 75), Color.White);
			}
			

            spriteBatch.End();

            base.Draw(gameTime);
        }
    }
}
