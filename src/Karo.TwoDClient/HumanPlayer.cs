using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Karo.Common;

namespace Karo.TwoDClient
{
    
	class HumanPlayer : Common.IPlayer
	{
	    private Core.Player playerNumber;
	    private Action<Move> chosenMove;
	    private Core.Karo Board;

	    public HumanPlayer(Core.Player playern, Core.Karo old)
	    {
	        Board = old;
	        playerNumber = playern;
            
	    }
	    public void DoMove(Move previousMove, int timeLimit, Action<Move> done)
	    {
	        Board = Board.WithMoveApplied(previousMove, playerNumber);
	        chosenMove = done;
	    }

	    public void PrepareMove(Move move)
	    {
	        if (chosenMove == null) return;

	        chosenMove(move);

	        chosenMove = null;
	    }

	    public Core.Karo GetNewBoard()
	    {
	        return Board;
	    }
        

	}
}
