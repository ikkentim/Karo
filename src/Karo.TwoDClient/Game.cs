using System;
using System.Collections.Generic;
using System.Linq;
using Karo.TwoDClient.Drawable;
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
        List<Tile> tiles = new List<Tile>();
        CKaro karo = new CKaro();
		Vector2 zeropoint = new Vector2(0, 0);
		Vector2 mousePos;
		Core.Player turn = Core.Player.Player1;
		Vector2 oldMousePos = new Vector2(0, 0);

        bool _canPlacePiece = true;

        public Game(int player1ai, int player2ai)
        {

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

			int x = (int)Math.Floor((ms.X / 50.0) - (int)Math.Floor(zeropoint.X/50));
			int y = (int)Math.Floor((ms.Y / 50.0) - (int)Math.Floor(zeropoint.Y/50));
			mousePos = new Vector2(x, y);

			Vector2 move = oldMousePos - new Vector2(ms.X, ms.Y);
			if (ms.RightButton == ButtonState.Pressed)
			{
				zeropoint -= move;
			}
			oldMousePos = new Vector2(ms.X, ms.Y);


			


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

            spriteBatch.Begin();

			
            foreach (Core.Tile tile in karo.Tiles)
            {
				Vector2 coor = new Vector2(tile.X * (50 + 1), tile.Y * (50 + 1));
				coor += zeropoint ;
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

			Vector2 marker = new Vector2(mousePos.X * (50 + 1) + zeropoint.X, mousePos.Y * (50 + 1) + zeropoint.Y);
			spriteBatch.Draw(Textures.selectedMarkerTex, marker, Color.White);
			
			spriteBatch.Draw(Textures.redTex, new Vector2(200, 25), Color.White);
			spriteBatch.Draw(Textures.whiteTex, new Vector2(250, 25), Color.White);
			if(turn == Core.Player.Player1){
				spriteBatch.Draw(Textures.turnIndicator, new Vector2(200, 75), Color.White);
			} else if (turn == Core.Player.Player2) {
				spriteBatch.Draw(Textures.turnIndicator, new Vector2(250, 75), Color.White);
			}
			

            spriteBatch.End();

            // TODO: Add your drawing code here

            base.Draw(gameTime);
        }
    }
}
