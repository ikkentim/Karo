using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Karo.Core;
using Karo.ThreeDClient;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace KaroThreeDClient.Components
{
    class Pawn : DrawableGameComponent
    {
        private readonly Piece _pawn;

        private new readonly Game Game;

        private Vector3 _lastPosition;
        private Vector3 _currentPosition;

        private bool _animating;

        public Pawn(Game game, Piece pawn)
            : base(game)
        {
            Game = game;
            _pawn = pawn;

            _currentPosition = new Vector3(_pawn.X, _pawn.Y, 0);
            _lastPosition = _currentPosition;
        }
        
        public override void Update(GameTime gameTime)
        {
            // are we not there yet?
            if (Math.Abs(_currentPosition.X - _pawn.X) > 0.01f || Math.Abs(_currentPosition.Y - _pawn.Y) > 0.01f)
            {
                Game.Animating = true;
                _animating = true;

                // use arrive steeringbehaviour for moving in x and y direction, we'll handle Z later
                _currentPosition += Arrive(new Vector3(_pawn.X, _pawn.Y, 0), _currentPosition) * (float)gameTime.ElapsedGameTime.TotalSeconds;

                // our z position is not based on velocity, the height is the same as the min length of the from and to tile, with a Sin for a smooth curve
                float distTo = (new Vector3(_pawn.X, _pawn.Y, 0) - new Vector3(_currentPosition.X, _currentPosition.Y, 0)).Length();
                float distFrom = (new Vector3(_lastPosition.X, _lastPosition.Y, 0) - new Vector3(_currentPosition.X, _currentPosition.Y, 0)).Length();

                _currentPosition.Z = (float)Math.Sin(Math.Min(distTo, distFrom));
            }
            // only complete the animation if we were actually animating
            else if (_animating)
            {
                // we arrived, or still stand on our place
                _lastPosition = _currentPosition;
                Game.Animating = false;
                _animating = false;
            }

            base.Update(gameTime);
        }

        public Vector3 Arrive(Vector3 targetPos, Vector3 currentPos)
        {
            Vector3 toTarget = targetPos - currentPos;

            float distance = toTarget.Length();

            if (distance > 0)
            {
                float decelerationTweaker = 0.3f;

                float speed = distance / ((int)1 * decelerationTweaker);

                speed = Math.Min(speed, 5f);

                Vector3 desiredVelocity = toTarget * speed / distance;

                return desiredVelocity;
            }

            return new Vector3(0, 0, 0);
        }


        public override void Draw(GameTime gameTime)
        {
            Model m = (_pawn.Player == KaroPlayer.Player1) ? Game.WhitePawnModel : Game.RedPawnModel;

            foreach (ModelMesh modelMesh in m.Meshes)
            {
                foreach (BasicEffect effect in modelMesh.Effects)
                {
                    Matrix world = Matrix.Identity;

                    if (_pawn.IsFaceUp)
                        world *= Matrix.CreateRotationX(MathHelper.ToRadians(180));

                    world *= Matrix.CreateTranslation(_currentPosition.X * Game.TileSize, 0.4f + _currentPosition.Z, _currentPosition.Y * Game.TileSize);

                    effect.World = world;
                    effect.View = Game.CameraService.View;
                    effect.Projection = Game.CameraService.Projection;
                    effect.EnableDefaultLighting();
                }

                modelMesh.Draw();
            }

            base.Draw(gameTime);
        }
    }
}
