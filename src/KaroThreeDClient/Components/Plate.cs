using System;
using Karo.Core;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace KaroThreeDClient.Components
{
    internal class Plate : DrawableGameComponent
    {
        private bool _isSelected;
        public Tile Tile;

        public bool IsCornerTile { get; set; }

        private Vector3 _lastPosition;
        private Vector3 _currentPosition;
        protected new Game Game { get; set; }
        protected Vector3 DiffuseColor { get; set; }
        protected float Alpha { get; set; }
        private bool _animating;

        public virtual bool IsSelected
        {
            get { return _isSelected; }
            set
            {
                _isSelected = value;
                DiffuseColor = value ? Color.LimeGreen.ToVector3() : Color.White.ToVector3();
            }
        }

        public BoundingBox BoundingBox
        {
            get
            {
                return
                    new BoundingBox(
                        new Vector3(Tile.X * Game.TileSize - Game.TileSize / 2, 0, Tile.Y * Game.TileSize - Game.TileSize / 2),
                        new Vector3(Tile.X * Game.TileSize + Game.TileSize / 2, 0.1f, Tile.Y * Game.TileSize + Game.TileSize / 2)
                        );
            }
        }

        public Plate(Game game, Tile tile)
            : base(game)
        {
            Game = game;
            Tile = tile;

            DiffuseColor = Color.White.ToVector3();
            Alpha = 1f;

            _currentPosition = new Vector3(Tile.X, Tile.Y, 0);
            _lastPosition = _currentPosition;
        }
        
        public override void Update(GameTime gameTime)
        {
            // are we not there yet?
            if (Math.Abs(_currentPosition.X -Tile.X) > 0.01f || Math.Abs(_currentPosition.Y - Tile.Y) > 0.01f)
            {
                Game.Animating = true;
                _animating = true;

                // use arrive steeringbehaviour for moving in x and y direction, we'll handle Z later
                _currentPosition += Arrive(new Vector3(Tile.X, Tile.Y, 0), _currentPosition) * (float)gameTime.ElapsedGameTime.TotalSeconds;

                // our z position is not based on velocity, the height is the same as the min length of the from and to tile, with a Sin for a smooth curve
                float distTo = (new Vector3(Tile.X, Tile.Y, 0) - new Vector3(_currentPosition.X, _currentPosition.Y, 0)).Length();
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

                speed = Math.Min(speed, 10f);

                Vector3 desiredVelocity = toTarget * speed / distance;

                return desiredVelocity;
            }

            return new Vector3(0, 0, 0);
        }

        public override void Draw(GameTime gameTime)
        {
            GraphicsDevice.BlendState = BlendState.AlphaBlend;

            foreach (var modelMesh in Game.TileModel.Meshes)
            {
                foreach (BasicEffect effect in modelMesh.Effects)
                {
                    var world = Matrix.Identity;

                    world *= Matrix.CreateTranslation(_currentPosition.X*Game.TileSize, -_currentPosition.Z * Game.TileSize, _currentPosition.Y*Game.TileSize);

                    effect.Alpha = Alpha;

                    if(Game.IsSelectingCornerTile && IsCornerTile)
                        effect.DiffuseColor = Color.DarkRed.ToVector3();
                    else
                        effect.DiffuseColor = DiffuseColor;

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