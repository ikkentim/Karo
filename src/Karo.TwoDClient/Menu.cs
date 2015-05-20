using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Windows.Forms;
using Karo.AI;
using Karo.Common;

namespace Karo.TwoDClient
{
    public partial class Menu : Form
    {
        private static readonly IEnumerable<Type> Types = new[]
        {
            typeof (HumanPlayer),
            typeof (Player)
        };

        public Menu()
        {
            InitializeComponent();

            player1ComboBox.Items.AddRange(Types.Select(t => new PlayerType(t.FullName, t)).ToArray());
            player2ComboBox.Items.AddRange(Types.Select(t => new PlayerType(t.FullName, t)).ToArray());

            player1ComboBox.SelectedIndex = 1;
            player2ComboBox.SelectedIndex = 1;
        }

        public IPlayer Player1 { get; private set; }
        public IPlayer Player2 { get; private set; }

        private void playButton_Click(object sender, EventArgs e)
        {
            var type1 = player1ComboBox.SelectedItem as PlayerType;
            var type2 = player2ComboBox.SelectedItem as PlayerType;

            if (type1 == null || type2 == null)
            {
                MessageBox.Show("Please select a player.", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            Player1 = Activator.CreateInstance(type1.Type) as IPlayer;
            Player2 = Activator.CreateInstance(type2.Type) as IPlayer;

            DialogResult = DialogResult.OK;

            Close();
        }

        private void exitButton_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            var of = new OpenFileDialog
            {
                Filter = "DLL Files|*.dll|All Files (*.*)|*.*", 
                FilterIndex = 1
            };

            if (of.ShowDialog() != DialogResult.OK)
                return;

            var assembly = System.Reflection.Assembly.LoadFrom(of.FileName);

            AppDomain.CurrentDomain.Load(assembly.GetName());

            var type = assembly.GetType(assembly.GetName() + ".Player") ??
                       assembly.GetTypes().FirstOrDefault(n => typeof (IPlayer).IsAssignableFrom(n));

            try
            {
                if (type == null) return;

                player1ComboBox.Items.Add(new PlayerType(assembly.GetName().ToString(), type));
                player2ComboBox.Items.Add(new PlayerType(assembly.GetName().ToString(), type));
            }
            catch (Exception exception)
            {
                MessageBox.Show(exception.ToString());
            }
            
        }

        private class PlayerType
        {
            private readonly String _name;

            public Type Type { get; private set; }

            public PlayerType(String name, Type type)
            {
                _name = name;
                Type = type;
            }

            public override string ToString()
            {
                return _name;
            }
        }
    }
}