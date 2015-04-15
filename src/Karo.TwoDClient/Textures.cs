using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Policy;
using System.Text;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;

namespace Karo.TwoDClient
{
    class Textures
    {
        public static Texture2D tileTexture;
        public static Texture2D pieceTexture;

        public static void Load(ContentManager content)
        {
            tileTexture = content.Load<Texture2D>("tile");
            pieceTexture = content.Load<Texture2D>("piece");
        }
    }
}
