namespace Karo.TwoDClient
{
    partial class Menu
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.player1AI = new System.Windows.Forms.ComboBox();
            this.player2AI = new System.Windows.Forms.ComboBox();
            this.play = new System.Windows.Forms.Button();
            this.exit = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(45, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "Player 1";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 44);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(45, 13);
            this.label2.TabIndex = 1;
            this.label2.Text = "Player 2";
            // 
            // player1AI
            // 
            this.player1AI.FormattingEnabled = true;
            this.player1AI.Items.AddRange(new object[] {
            "Human",
            "Computer 1"});
            this.player1AI.Location = new System.Drawing.Point(63, 6);
            this.player1AI.Name = "player1AI";
            this.player1AI.Size = new System.Drawing.Size(121, 21);
            this.player1AI.TabIndex = 2;
            // 
            // player2AI
            // 
            this.player2AI.FormattingEnabled = true;
            this.player2AI.Items.AddRange(new object[] {
            "Human",
            "Computer 2"});
            this.player2AI.Location = new System.Drawing.Point(63, 41);
            this.player2AI.Name = "player2AI";
            this.player2AI.Size = new System.Drawing.Size(121, 21);
            this.player2AI.TabIndex = 3;
            // 
            // play
            // 
            this.play.Location = new System.Drawing.Point(109, 68);
            this.play.Name = "play";
            this.play.Size = new System.Drawing.Size(75, 23);
            this.play.TabIndex = 4;
            this.play.Text = "Play";
            this.play.UseVisualStyleBackColor = true;
            this.play.Click += new System.EventHandler(this.play_Click);
            // 
            // exit
            // 
            this.exit.Location = new System.Drawing.Point(12, 68);
            this.exit.Name = "exit";
            this.exit.Size = new System.Drawing.Size(75, 23);
            this.exit.TabIndex = 5;
            this.exit.Text = "Exit";
            this.exit.UseVisualStyleBackColor = true;
            this.exit.Click += new System.EventHandler(this.exit_Click);
            // 
            // Menu
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(193, 101);
            this.Controls.Add(this.exit);
            this.Controls.Add(this.play);
            this.Controls.Add(this.player2AI);
            this.Controls.Add(this.player1AI);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Name = "Menu";
            this.Text = "Menu";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ComboBox player1AI;
        private System.Windows.Forms.ComboBox player2AI;
        private System.Windows.Forms.Button play;
        private System.Windows.Forms.Button exit;
    }
}