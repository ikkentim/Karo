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
        public static Texture2D tileTex;
		public static Texture2D redTex;
		public static Texture2D whiteTex;
		public static Texture2D redMarkTex;
		public static Texture2D whiteMarkTex;
		public static Texture2D cursorTex;
		public static Texture2D turnIndicator;
		public static Texture2D moveIndicator;
		public static Texture2D selectIndicator;



        public static void Load(ContentManager content)
        {
            tileTex = content.Load<Texture2D>("tile");
            redTex = content.Load<Texture2D>("redpiece");
			whiteTex = content.Load<Texture2D>("whitepiece");
			redMarkTex = content.Load<Texture2D>("redpiecemark");
			whiteMarkTex = content.Load<Texture2D>("whitepiecemark");
			cursorTex = content.Load<Texture2D>("selected");
			turnIndicator = content.Load<Texture2D>("turn_indicator");
			moveIndicator = content.Load<Texture2D>("possiblemove");
			selectIndicator = content.Load<Texture2D>("selectedpiece");
        }
    }
}
