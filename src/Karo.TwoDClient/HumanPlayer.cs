using System;
using Karo.Common;
using Karo.Core;

namespace Karo.TwoDClient
{
    internal class HumanPlayer : IPlayer
    {
        private KaroBoardState Board;
        private Action<Move> chosenMove;

        public HumanPlayer()
        {
            Board = new KaroBoardState();
        }

        public void DoMove(Move previousMove, int timeLimit, Action<Move> done)
        {
            if (previousMove != null)
                Board = Board.WithMoveApplied(previousMove, KaroPlayer.Player2);

            chosenMove = done;
        }

        public void PrepareMove(Move move)
        {
            if (chosenMove == null) return;

            if (!Board.IsValidMove(move))
                return;

            Board = Board.WithMoveApplied(move, KaroPlayer.Player1);

            chosenMove(move);
            chosenMove = null;
        }
    }
}