using System;
using System.Windows.Forms;

namespace Karo.TwoDClient
{
#if WINDOWS
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        static void Main(string[] args)
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            
            Menu menu = new Menu();

            menu.ShowDialog();

            using (Game game = new Game())
            {
                game.Run();
            }
        }
    }
#endif
}

