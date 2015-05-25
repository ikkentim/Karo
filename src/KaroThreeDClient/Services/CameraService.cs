using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Audio;

namespace KaroThreeDClient.Services
{
    public class CameraService : GameComponent
    {
        public const float MaxCameraSpeed = 20.0f;
        public const float CameraSpeed = 5.0f;
        public const float DefaultZoom = 3;
        public const float MinZoom = 1;
        public const float MaxZoom = 15;
        public const float CameraTargetOffset = 0.2f;
        public const float CameraHeightOffset = 0.75f;
        private float _aspectRatio;
        private Vector3 _velocity;
        private Vector3 _undirectedVelocity;
        private float _zoom = DefaultZoom;
        private AudioListener _audioListener;

        public CameraService(Game game)
            : base(game)
        {
            View = Matrix.CreateLookAt(Vector3.Zero, Vector3.Right, Vector3.Up);

            Projection = Matrix.Identity;
            _audioListener = new AudioListener { Up = Vector3.Up };

        }

        public Vector3 Position { get; set; }

        public float Zoom
        {
            get { return _zoom; }
        }

        public AudioListener AudioListener
        {
            get { return _audioListener; }
        }

        public float Rotation { get; private set; }
        public Matrix View { get; private set; }
        public Matrix Projection { get; private set; }
        public Vector3 TargetPosition { get; set; }

        public override void Update(GameTime gameTime)
        {
            if (_aspectRatio != Game.GraphicsDevice.Viewport.AspectRatio)
            {
                _aspectRatio = Game.GraphicsDevice.Viewport.AspectRatio;
                Projection = Matrix.CreatePerspectiveFieldOfView(MathHelper.ToRadians(45.0f), _aspectRatio, 0.1f,
                    10000.0f);
            }

            _velocity = (TargetPosition - Position) * CameraSpeed * (float)gameTime.ElapsedGameTime.TotalSeconds;

            LimitVelocity(ref _undirectedVelocity);
            LimitVelocity(ref _velocity);

            if (_undirectedVelocity != Vector3.Zero)
            {
                var worldCameraVelocity = _undirectedVelocity == Vector3.Zero
                    ? Vector3.Zero
                    : Vector3.Transform(_undirectedVelocity, Matrix.CreateRotationY(-Rotation));

                Position += worldCameraVelocity;
                TargetPosition = Position;
            }
            else
            {
                Position += _velocity;
            }

            CalculateView();
            base.Update(gameTime);
        }

        private void LimitVelocity(ref Vector3 velocity)
        {
            if (velocity == Vector3.Zero) return;

            if (velocity.Length() > MaxCameraSpeed)
            {
                velocity.Normalize();
                velocity *= 2;
            }

            if (velocity.LengthSquared() < 0.00001)
                velocity = Vector3.Zero;
            else
                velocity *= 0.9f;
        }

        public void AddVelocity(Vector3 acceleration)
        {
            _undirectedVelocity += acceleration;
        }

        public void Move(float deltaRotation, float deltaZoom)
        {
            _zoom += deltaZoom;
            Rotation += deltaRotation;

            _zoom = MathHelper.Clamp(_zoom + deltaZoom, MinZoom, MaxZoom);
        }

        private void CalculateView()
        {
            var realCameraTarget = Position + new Vector3(0, CameraTargetOffset, 0);
            var cameraPosition = realCameraTarget +
                                 new Vector3((float)Math.Cos(Rotation) * 1.8f,
                                     _zoom / 3 - MinZoom + CameraTargetOffset + CameraHeightOffset,
                                     (float)Math.Sin(Rotation) * 1.8f) * _zoom;

            // Also update listener data
            _audioListener.Position = cameraPosition;
            _audioListener.Forward = Vector3.Normalize(realCameraTarget - cameraPosition);

            View = Matrix.CreateLookAt(cameraPosition, realCameraTarget, Vector3.Up);
        }
    }
}
