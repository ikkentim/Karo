using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Karo.Common;

namespace Karo.TwoDClient
{
    class ComputerPlayer : IPlayer
    {
        public AI.AI ai = new AI.AI();

        public void DoMove(Move previousMove, int timeLimit, Action<Move> done)
        {
            ai.DoMove(previousMove, timeLimit, done);
        }
    }
}
