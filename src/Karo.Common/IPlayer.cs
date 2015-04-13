using System;

namespace Karo.Common
{
    /// <summary>
    ///     Contains methods that define a player.
    /// </summary>
    public interface IPlayer
    {
        /// <summary>
        ///     Is called when the turn of this <see cref="IPlayer" /> has started.
        /// </summary>
        /// <param name="previousMove">The previous move.</param>
        /// <param name="done">A method to be called when the move has been calculated.</param>
        void DoMove(Move previousMove, Func<Move> done);
    }
}