using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;
namespace Karo.TwoDClient
{
	class Camera2D
	{
		public float zoom;
		public Matrix transform;
		public Vector2 position {get;set;}
		public float rotation;

		public Camera2D(int x, int y)
		{
			zoom = 1.0f;
			rotation = 0.0f;
			position = new Vector2(x, y);
		}

		public void Move(Vector2 movement)
		{
			position += movement;
		}
		public Matrix get_transform(GraphicsDeviceManager graphics)
		{
			transform = Matrix.CreateTranslation(new Vector3(-position.X, -position.Y, 0))
				* Matrix.CreateRotationZ(rotation)
				* Matrix.CreateScale(new Vector3(zoom, zoom, 1))
				* Matrix.CreateTranslation(new Vector3(0, 0, 0));
			return transform;
		}
	}
}
