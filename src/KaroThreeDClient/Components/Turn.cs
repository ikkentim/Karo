using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Karo.Core;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace KaroThreeDClient.Components
{
    class Turn : DrawableGameComponent
    {
        private Game _game;

        public Turn(Game game) : base(game)
        {
            _game = game;
        }

        public override void Draw(GameTime gameTime)
        {
            List<Model> models = new List<Model>();

            models.Add(_game.WhitePawnModel);
            models.Add(_game.RedPawnModel);

            foreach (Model model in models)
            {
                foreach (ModelMesh modelMesh in model.Meshes)
                {
                    foreach (BasicEffect effect in modelMesh.Effects)
                    {
                        Matrix world = Matrix.Identity;

                        world *= Matrix.CreateScale(new Vector3(0.3f, 0.3f, 0.3f));

                        Vector3 near = GraphicsDevice.Viewport.Unproject(new Vector3(200, 200, 0), _game.CameraService.Projection, _game.CameraService.View, Matrix.Identity);

                        effect.World = world;
                        effect.View = Matrix.CreateLookAt(
                            new Vector3(1, 1, 1),
                            near,
                            Vector3.Up
                        );
                        effect.Projection = _game.CameraService.Projection;
                        effect.EnableDefaultLighting();
                    }

                    modelMesh.Draw();
                }

            }
            base.Draw(gameTime);
        }
    }
}
