using System;
using System.Diagnostics;
using System.Threading;
using System.Windows.Forms;

namespace Karo.TwoDClient
{
#if WINDOWS || LINUX
    /// <summary>
    ///     The main class.
    /// </summary>
    public static class Program
    {
        /// <summary>
        ///     The main entry point for the application.
        /// </summary>
        [STAThread]
        private static void Main()
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