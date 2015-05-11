using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Windows.Forms;
using Karo.Core;
using Microsoft.Xna.Framework;

namespace Karo.TwoDClient
{
#if WINDOWS || LINUX
    /// <summary>
    /// The main class.
    /// </summary>
    public static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            
            Menu menu = new Menu();
			int p1ai = 0;
			int p2ai = 0;
            var result = menu.ShowDialog();

			p1ai = menu.ai1;
			p2ai = menu.ai2;

			using (Game game = new Game(p1ai, p2ai))
            {
                game.Run();
            }
        }
    }
#endif
}
