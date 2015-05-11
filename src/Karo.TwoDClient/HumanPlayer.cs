using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Karo.Common;
using Karo.Core;

namespace Karo.TwoDClient
{
    
	class HumanPlayer : IPlayer
	{
        private KaroPlayer playerNumber;
	    private Action<Move> chosenMove;
	    private KaroBoardState Board;

	    public HumanPlayer(KaroPlayer player)
	    {
            Board = new KaroBoardState();
	        playerNumber = player;
            
	    }
	    public void DoMove(Move previousMove, int timeLimit, Action<Move> done)
	    {
	        if (previousMove != null)
	            Board = Board.WithMoveApplied(previousMove,
                    playerNumber == KaroPlayer.Player1 ? KaroPlayer.Player2 : KaroPlayer.Player1);

	        chosenMove = done;
	    }

	    public void PrepareMove(Move move)
	    {
	        if (chosenMove == null) return;

	        if (Board.IsValidMove(move))
	        {
	            Board = Board.WithMoveApplied(move, playerNumber);
	            var tmp = chosenMove;
	            chosenMove = null;
	            tmp(move);
	        }
	    }
	}
}
