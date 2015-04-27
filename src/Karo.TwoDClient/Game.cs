using System;
using System.Collections.Generic;
using System.Linq;
using Karo.Common;
using Karo.Core;
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
		Camera2D cam = new Camera2D(-200, -200);
		Vector2 tilePos;
		Vector2 mousePos;
		Common.IPlayer PlayerOne;
		Common.IPlayer PlayerTwo;

		Core.Tile selectedPiece;

		bool playerOneTurn = false;
		Vector2 oldMousePos = new Vector2(0, 0);

        bool _canPlacePiece = true;

        public Game(int player1ai, int player2ai)
        {
			if (player1ai == 0) { PlayerOne = new HumanPlayer(Player.Player1, karo); } // else new CPU player
			if (player2ai == 0) { PlayerTwo = new HumanPlayer(Player.Player2, karo); } // else new CPU player

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
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed || Keyboard.GetState().IsKeyDown(Keys.Escape))
                this.Exit();

            //get the current mouse state, save to vector				
            MouseState ms = Mouse.GetState();

			mousePos = new Vector2(ms.X, ms.Y);

            //update the camera's position based on how far the right mouse button has been dragged since last update
			Vector2 move = oldMousePos - new Vector2(ms.X, ms.Y);
			if (ms.RightButton == ButtonState.Pressed)
			{
				cam.Move(move);
			}
			oldMousePos = new Vector2(ms.X, ms.Y);


            //since the mouse click location is relative to the camera we need to get the absolute position in the playing field
			Vector2 camPos = cam.position;
			Vector2 absPos = mousePos + camPos;
			tilePos = new Vector2((int)Math.Floor(absPos.X / 50), (int)Math.Floor(absPos.Y / 50));

            //if mouse button is pressed
			if(ms.LeftButton == ButtonState.Pressed && _canPlacePiece) {
                //get the tile at the mouse location
			    Core.Tile t = karo.GetTileAt((int) tilePos.X, (int) tilePos.Y);

			    _canPlacePiece = false;
                //handle turn logic
			    
                //if the clicked tile is not empty
			    if (t != null)
                    //if there is no current selected tile
			        if (selectedPiece == null)
			        {
                        //select the clicked tile
			            selectedPiece = t;
			        }
			        else
			        {
                        //make a move
                        //test code, move-making code goes here
			            HumanPlayer testplayer;
			            if (playerOneTurn)
			            {
			                testplayer = (HumanPlayer) PlayerOne;
			            }
			            else
			            {
			                testplayer = (HumanPlayer) PlayerTwo;
			            }
			            testplayer.PrepareMove(new Move(t.X, t.Y, 0, 0, 0, 0));
			            selectedPiece = null;
			            karo = testplayer.GetNewBoard();
                        if (playerOneTurn)
                        {
                            PlayerOne = testplayer;
                        }
                        else
                        {
                            PlayerTwo = testplayer;
                        }
			            playerOneTurn = !playerOneTurn;
			        }
			}
            if (ms.LeftButton == ButtonState.Released && !_canPlacePiece)
            {
                _canPlacePiece = true;
            }
		
            base.Update(gameTime);
        }


        /// <summary>
        /// This is called when the game should draw itself.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.CornflowerBlue);

			spriteBatch.Begin(SpriteSortMode.Deferred,
						BlendState.AlphaBlend,
						null,
						null,
						null,
						null,
						cam.get_transform(graphics));

            DrawTiles();

            DrawPieces();

            DrawUI();

            spriteBatch.End();

            base.Draw(gameTime);
        }

        private void DrawTiles()
        {
            foreach (Core.Tile tile in karo.Tiles)
            {
                Vector2 coor = new Vector2(tile.X * (50 + 1), tile.Y * (50 + 1));
                
                spriteBatch.Draw(Textures.tileTex, coor, Color.White);
            }
        }

        private void DrawPieces()
        {
            int counter = 0;
            if (karo.Pieces != null)
                foreach (Core.Piece piece in karo.Pieces)
                {
                    if (piece != null)
                    {
                        Vector2 coord = new Vector2(piece.Tile.X, piece.Tile.Y);
                        if (piece.Player == Core.Player.Player1)
                            if (piece.IsFaceUp)
                            {
                                spriteBatch.Draw(Textures.redTex, coord, Color.White);
                            }
                            else
                            {
                                spriteBatch.Draw(Textures.redMarkTex, coord, Color.White);
                            }
                        if (piece.Player == Core.Player.Player2)
                        {
                            if (piece.IsFaceUp)
                            {
                                spriteBatch.Draw(Textures.whiteTex, coord, Color.White);
                            }
                            else
                            {
                                spriteBatch.Draw(Textures.whiteMarkTex, coord, Color.White);
                            }
                        }
                    }
                    else
                    {
                        spriteBatch.Draw(Textures.whiteTex, new Vector2(-100, (counter * 10)), Color.White);
                    }
                    counter++;
                }
            else
            {
                spriteBatch.Draw(Textures.redTex, new Vector2(50, 50), Color.White);
            }
        }

        private void DrawUI()
        {
            Vector2 marker = tilePos * 51;

            spriteBatch.Draw(Textures.cursorTex, marker, Color.White);

            spriteBatch.Draw(Textures.redTex, cam.position + new Vector2(200, 25), Color.White);
            spriteBatch.Draw(Textures.whiteTex, cam.position + new Vector2(250, 25), Color.White);

            if (playerOneTurn)
            {
                spriteBatch.Draw(Textures.turnIndicator, cam.position + new Vector2(200, 75), Color.White);
            }
            else
            {
                spriteBatch.Draw(Textures.turnIndicator, cam.position + new Vector2(250, 75), Color.White);
            }
            if (selectedPiece != null)
            {
                spriteBatch.Draw(Textures.selectIndicator, new Vector2(selectedPiece.X * 51, selectedPiece.Y * 51), Color.White);
            }
        }
    }
}
