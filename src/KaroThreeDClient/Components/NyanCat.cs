using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace KaroThreeDClient.Components
{
    class NyanCat : DrawableGameComponent
    {
        private Game _game;

        public NyanCat(Game game) : base(game)
        {
            _game = game;
        }

        public override void Draw(GameTime gameTime)
        {
            foreach (ModelMesh modelMesh in _game.NyanCat.Meshes)
            {
                foreach (BasicEffect effect in modelMesh.Effects)
                {
                    Matrix world = Matrix.Identity;

                    world *= Matrix.CreateTranslation(-10, 0, 0);

                    world *= Matrix.CreateRotationY(MathHelper.ToRadians(-45));

                    effect.World = world;
                    effect.View = Matrix.CreateLookAt(
                        new Vector3(5, 5, 5),
                        new Vector3(0, .5f, 0),
                        Vector3.Up
                    );
                    effect.Projection = _game.CameraService.Projection;
                    effect.EnableDefaultLighting();
                }

                modelMesh.Draw();
            }

            base.Draw(gameTime);
        }
    }
}
