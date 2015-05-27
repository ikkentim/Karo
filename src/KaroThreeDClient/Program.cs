#region Using Statements
using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;
#endregion

namespace KaroThreeDClient
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

            var menu = new Menu();
            if (menu.ShowDialog() != DialogResult.OK) return;

            var game = new Game(menu.Player1, menu.Player2);

            game.Run();
        }
    }
#endif
}
