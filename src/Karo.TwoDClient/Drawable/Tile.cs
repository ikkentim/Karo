using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace Karo.TwoDClient.Drawable
{
    class Tile
    {
        public int X;
        public int Y;

        public int TileSize = 50;

        private Texture2D _texture;
        private Texture2D _piece;

        public bool HasPiecie = false;

        public Tile(int x, int y, Texture2D texture, Texture2D piece)
        {
            X = x;
            Y = y;
            _texture = texture;
            _piece = piece;
        }

        public void Draw(GraphicsDeviceManager graphics, SpriteBatch spriteBatch)
        {

        }
    }
}
