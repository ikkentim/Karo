using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Karo.TwoDClient
{
    public partial class Menu : Form
    {
		public int ai1;
		public int ai2;
        public Menu()
        {
            InitializeComponent();

            player1AI.SelectedIndex = 0;
            player2AI.SelectedIndex = 0;
        }

        private void play_Click(object sender, EventArgs e)
        {
			ai1 = player1AI.SelectedIndex;
			ai2 = player2AI.SelectedIndex;
            Close();
        }

        private void exit_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

    }
}
