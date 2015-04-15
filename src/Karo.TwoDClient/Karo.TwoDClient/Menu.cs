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
        public Menu()
        {
            InitializeComponent();

            player1AI.SelectedIndex = 0;
            player2AI.SelectedIndex = 0;
        }

        private void play_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void exit_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

    }
}
