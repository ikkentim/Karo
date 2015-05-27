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

        public Plate(Game game, Tile tile)
            : base(game)
        {
            Game = game;
            Tile = tile;

            DiffuseColor = Color.White.ToVector3();
            Alpha = 1f;
        }

        protected new Game Game { get; set; }
        protected Vector3 DiffuseColor { get; set; }
        protected float Alpha { get; set; }

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
                        new Vector3(Tile.X*Game.TileSize - Game.TileSize/2, 0, Tile.Y*Game.TileSize - Game.TileSize/2),
                        new Vector3(Tile.X*Game.TileSize + Game.TileSize/2, 0.1f, Tile.Y*Game.TileSize + Game.TileSize/2)
                        );
            }
        }

        public override void Initialize()
        {
            base.Initialize();
        }

        public override void Update(GameTime gameTime)
        {
            base.Update(gameTime);
        }

        public override void Draw(GameTime gameTime)
        {
            GraphicsDevice.BlendState = BlendState.AlphaBlend;

            foreach (var modelMesh in Game.TileModel.Meshes)
            {
                foreach (BasicEffect effect in modelMesh.Effects)
                {
                    var world = Matrix.Identity;

                    world *= Matrix.CreateTranslation(Tile.X*Game.TileSize, 0, Tile.Y*Game.TileSize);

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