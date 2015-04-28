using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Karo.Common;
using Karo.Core;
using Karo = Karo.Core.Karo;

namespace Karo.TwoDClient
{
    
	class HumanPlayer : IPlayer
	{
	    private Player playerNumber;
	    private Action<Move> chosenMove;
	    private Core.Karo Board;

	    public HumanPlayer(Player player)
	    {
	        Board = new Core.Karo();
	        playerNumber = player;
            
	    }
	    public void DoMove(Move previousMove, int timeLimit, Action<Move> done)
	    {
	        if (previousMove != null)
	            Board = Board.WithMoveApplied(previousMove,
	                playerNumber == Player.Player1 ? Player.Player2 : Player.Player1);

	        chosenMove = done;
	    }

	    public void PrepareMove(Move move)
	    {
	        if (chosenMove == null) return;

	        if (Board.IsValidMove(move))
	        {
	            Board = Board.WithMoveApplied(move, playerNumber);
	            chosenMove(move);

	            chosenMove = null;
	        }
	    }
	}
}
