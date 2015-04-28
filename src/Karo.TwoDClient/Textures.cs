using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.Graphics;

namespace Karo.TwoDClient
{
    internal class Textures
    {
        public static Texture2D Tile;
        public static Texture2D Piece;
        public static Texture2D RedPiece;
        public static Texture2D WhitePiece;
        public static Texture2D RedPieceMarked;
        public static Texture2D WhitePieceMarked;
        public static Texture2D Cursor;
        public static Texture2D TurnIndicator;
        public static Texture2D MoveIndicator;
        public static Texture2D SelectIndicator;


        public static void Load(ContentManager content)
        {
            Tile = content.Load<Texture2D>("tile");
            Piece = content.Load<Texture2D>("piece");
            RedPiece = content.Load<Texture2D>("redpiece");
            WhitePiece = content.Load<Texture2D>("whitepiece");
            RedPieceMarked = content.Load<Texture2D>("redpiecemark");
            WhitePieceMarked = content.Load<Texture2D>("whitepiecemark");
            Cursor = content.Load<Texture2D>("selected");
            TurnIndicator = content.Load<Texture2D>("turn_indicator");
            MoveIndicator = content.Load<Texture2D>("possiblemove");
            SelectIndicator = content.Load<Texture2D>("selectedpiece");
        }
    }
}