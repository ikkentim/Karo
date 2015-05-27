using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Karo.Core;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace KaroThreeDClient.Components
{
    class GhostPlate : Plate
    {
        public new bool IsSelected
        {
            get { return base.IsSelected; }
            set
            {
                base.IsSelected = value;
                DiffuseColor = value ? Color.LimeGreen.ToVector3() : Color.Gray.ToVector3();
                Alpha = value ? 1f : 0.3f;
            }
        }
        public GhostPlate(Game game, Tile tile) : base(game, tile)
        {
            DiffuseColor = Color.Gray.ToVector3();
            Alpha = 0.3f;
        }

        public override void Draw(GameTime gameTime)
        {
            if(Game.IsSelectingGhostTile)
                base.Draw(gameTime);
        }
    }
}
