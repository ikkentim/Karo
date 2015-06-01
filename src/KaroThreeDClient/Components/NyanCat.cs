using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace KaroThreeDClient.Components
{
    class NyanCat : DrawableGameComponent
    {
        private Game _game;

        private Matrix[] _boneTransformationsMatrix;
        private float _rotation;
        private Matrix[] _randomMatrices = new Matrix[6];
        private Matrix _offset = Matrix.Identity;
        private Matrix _center = Matrix.Identity;
        public NyanCat(Game game) : base(game)
        {
            _game = game;
        }

        public override void Update(GameTime gameTime)
        {
            _rotation -= (float) gameTime.ElapsedGameTime.TotalSeconds;

            _offset = Matrix.CreateRotationY(_rotation / 2);

            for (var i = 0; i < _randomMatrices.Length; i++)
                _randomMatrices[i] = Matrix.CreateRotationX(_rotation/(((float) i + 1)/4))*
                                     Matrix.CreateRotationY(_rotation/(((float) i + 2)/4))*
                                     Matrix.CreateRotationZ(_rotation/(((float) i + 3)/4));
            
            var position = _game._karo.Tiles.Where(p => p != null)
                .Select(p => new Vector3(p.X*_game.TileSize, 0, p.Y*_game.TileSize))
                .Aggregate(Vector3.Zero, (p, v) => p + v)/20;

            _center = Matrix.CreateTranslation(position);

            base.Update(gameTime);
        }

        public override void Draw(GameTime gameTime)
        {
            if (_boneTransformationsMatrix == null)
            {
                _boneTransformationsMatrix = new Matrix[_game.NyanCat.Bones.Count];
                _game.NyanCat.CopyAbsoluteBoneTransformsTo(_boneTransformationsMatrix);
            }

            foreach (ModelMesh modelMesh in _game.NyanCat.Meshes)
            {
                foreach (BasicEffect effect in modelMesh.Effects)
                {

                    if (modelMesh.Name.Length < 8 || new[] {1, 2, 3, 4, 5, 6, 18, 11}.Contains(int.Parse(modelMesh.Name.Substring(5, 3))))
                    {
                        effect.World = _boneTransformationsMatrix[modelMesh.ParentBone.Index]*
                                       Matrix.CreateRotationX(MathHelper.ToRadians(90))*
                                       Matrix.CreateRotationY(MathHelper.ToRadians(180))*
                                       Matrix.CreateTranslation(new Vector3(5, 0, 0))*
                                       _offset*
                                       _center;
                    }
                    else
                    {
                        effect.World = _randomMatrices[modelMesh.ParentBone.Index%_randomMatrices.Length]*
                                       _boneTransformationsMatrix[modelMesh.ParentBone.Index]*
                                       Matrix.CreateRotationX(MathHelper.ToRadians(90))*
                                       Matrix.CreateRotationY(MathHelper.ToRadians(180))*
                                       Matrix.Identity*
                                       _center;
                    }
          

                    effect.View = _game.CameraService.View;
                    effect.Projection = _game.CameraService.Projection;
                    //effect.EnableDefaultLighting();
                }

                modelMesh.Draw();
            }

            base.Draw(gameTime);
        }
    }
}
