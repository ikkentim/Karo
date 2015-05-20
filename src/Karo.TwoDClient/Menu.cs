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

            player1ComboBox.Items.AddRange(Types.Select(t => new PlayerType(t.FullName, t)).ToArray());
            player2ComboBox.Items.AddRange(Types.Select(t => new PlayerType(t.FullName, t)).ToArray());

            player1ComboBox.SelectedIndex = 1;
            player2ComboBox.SelectedIndex = 1;
        }

        public static IEnumerable<Type> Types { get; private set; }
        public IPlayer Player1 { get; private set; }
        public IPlayer Player2 { get; private set; }

        private void playButton_Click(object sender, EventArgs e)
        {
            Player1 = Activator.CreateInstance((player1ComboBox.SelectedItem as PlayerType).Type) as IPlayer;
            Player2 = Activator.CreateInstance((player2ComboBox.SelectedItem as PlayerType).Type) as IPlayer;

            DialogResult = DialogResult.OK;

            Close();
        }

        private void exitButton_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void Menu_Load(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            OpenFileDialog of = new OpenFileDialog();
            of.Filter = "DLL Files (.dll)|*.dll|All Files (*.*)|*.*";
            of.FilterIndex = 1;

            of.ShowDialog();

            System.Reflection.Assembly assembly = System.Reflection.Assembly.LoadFrom(of.FileName);

            AppDomain.CurrentDomain.Load(assembly.GetName());

            Type t = assembly.GetType(assembly.GetName() + ".Player");

            var iPlayer = typeof(IPlayer);
            if (t == null)
                t = assembly.GetTypes().FirstOrDefault(n => iPlayer.IsAssignableFrom(n));

            try
            {
                if (t != null)
                {
                    player1ComboBox.Items.Add(new PlayerType(assembly.GetName().ToString(), t));
                    player2ComboBox.Items.Add(new PlayerType(assembly.GetName().ToString(), t));
                }
            }
            catch (Exception exception)
            {
                MessageBox.Show(exception.ToString());
            }
            
        }

        class PlayerType{
            public String Name { get; set; }
            public Type Type { get; set; }

            public PlayerType(String name, Type type)
            {
                Name = name;
                Type = type;
            }

            public override string ToString()
            {
                return Name;
            }
        }
    }
}