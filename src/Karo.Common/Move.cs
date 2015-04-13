namespace Karo.Common
{
    /// <summary>
    ///     Represents a move of a karo piece.
    /// </summary>
    public struct Move
    {
        /// <summary>
        ///     Initializes a new instance of the <see cref="Move" /> struct.
        /// </summary>
        /// <param name="newPieceX">The X-coordinate of the location the piece is moving to.</param>
        /// <param name="newPieceY">The Y-coordinate of the location the piece is moving to.</param>
        /// <param name="oldPieceX">The X-coordinate of the location the piece is moving from.</param>
        /// <param name="oldPieceY">The Y-coordinate of the location the piece is moving from.</param>
        /// <param name="oldTileX">The X-coordinate of the location of the tile this move has taken the tile from.</param>
        /// <param name="oldTileY">The Y-coordinate of the location of the tile this move has taken the tile from.</param>
        public Move(int newPieceX, int newPieceY, int oldPieceX, int oldPieceY, int oldTileX, int oldTileY) : this()
        {
            NewPieceX = newPieceX;
            NewPieceY = newPieceY;
            OldPieceX = oldPieceX;
            OldPieceY = oldPieceY;
            OldTileX = oldTileX;
            OldTileY = oldTileY;
        }

        /// <summary>
        ///     Gets the X-coordinate of the location the piece is moving to.
        /// </summary>
        public int NewPieceX { get; private set; }

        /// <summary>
        ///     Gets the Y-coordinate of the location the piece is moving to.
        /// </summary>
        public int NewPieceY { get; private set; }

        /// <summary>
        ///     Gets the X-coordinate of the location the piece is moving from.
        /// </summary>
        public int OldPieceX { get; private set; }

        /// <summary>
        ///     Gets the Y-coordinate of the location the piece is moving from.
        /// </summary>
        public int OldPieceY { get; private set; }

        /// <summary>
        ///     Gets the X-coordinate of the location of the tile this move has taken the tile from.
        /// </summary>
        public int OldTileX { get; private set; }

        /// <summary>
        ///     Gets the Y-coordinate of the location of the tile this move has taken the tile from.
        /// </summary>
        public int OldTileY { get; private set; }
    }
}