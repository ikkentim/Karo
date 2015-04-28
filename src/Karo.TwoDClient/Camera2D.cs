using Microsoft.Xna.Framework;

namespace Karo.TwoDClient
{
    internal class Camera2D
    {
        public float Rotation { get; private set; }
        public Matrix Transform { get
        {
            return Matrix.CreateTranslation(new Vector3(-Position.X, -Position.Y, 0))
                   *Matrix.CreateRotationZ(Rotation)
                   *Matrix.CreateScale(new Vector3(Zoom, Zoom, 1))
                   *Matrix.CreateTranslation(new Vector3(0, 0, 0));
        } }
        public float Zoom { get; private set; }

        public Camera2D(int x, int y)
        {
            Zoom = 1.0f;
            Rotation = 0.0f;
            Position = new Vector2(x, y);
        }

        public Vector2 Position { get; set; }

        public void Move(Vector2 movement)
        {
            Position += movement;
        }
    }
}