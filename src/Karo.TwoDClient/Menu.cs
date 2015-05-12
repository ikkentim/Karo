using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Windows.Forms;
using Karo.AI;
using Karo.Common;

namespace Karo.TwoDClient
{
    public partial class Menu : Form
    {
        static Menu()
        {
            Types = new[]
            {
                typeof (HumanPlayer),
                typeof (Player)
            };
        }

        public Menu()
        {
            InitializeComponent();

            player1ComboBox.Items.AddRange(Types as Type[]);
            player2ComboBox.Items.AddRange(Types as Type[]);

            player1ComboBox.SelectedIndex = 0;
            player2ComboBox.SelectedIndex = 0;
        }

        public static IEnumerable<Type> Types { get; private set; }
        public IPlayer Player1 { get; private set; }
        public IPlayer Player2 { get; private set; }

        private void playButton_Click(object sender, EventArgs e)
        {
            Player1 = Activator.CreateInstance(player1ComboBox.SelectedItem as Type) as IPlayer;
            Player2 = Activator.CreateInstance(player2ComboBox.SelectedItem as Type) as IPlayer;

            DialogResult = DialogResult.OK;

            Close();
        }

        private void exitButton_Click(object sender, EventArgs e)
        {
            Close();
        }
    }
}