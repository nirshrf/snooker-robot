namespace robotController_PubNub
{
    partial class Controller
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Controller));
            this.label1 = new System.Windows.Forms.Label();
            this.stopButton = new System.Windows.Forms.Button();
            this.rotateCclkWsButton = new System.Windows.Forms.Button();
            this.rotateClkWsButton = new System.Windows.Forms.Button();
            this.autoButton = new System.Windows.Forms.Button();
            this.upButton = new System.Windows.Forms.Button();
            this.downButton = new System.Windows.Forms.Button();
            this.leftButton = new System.Windows.Forms.Button();
            this.rightButton = new System.Windows.Forms.Button();
            this.readyButton = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(96, 39);
            this.label1.TabIndex = 0;
            this.label1.Text = "Move - Arrow Keys\r\nRotate  - A , D\r\nAuto Mode - S\r\n";
            // 
            // stopButton
            // 
            this.stopButton.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.stopButton.FlatAppearance.BorderColor = System.Drawing.Color.Red;
            this.stopButton.FlatAppearance.BorderSize = 0;
            this.stopButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.stopButton.Location = new System.Drawing.Point(97, 130);
            this.stopButton.Name = "stopButton";
            this.stopButton.Size = new System.Drawing.Size(64, 64);
            this.stopButton.TabIndex = 7;
            this.stopButton.Text = "Stop";
            this.stopButton.UseVisualStyleBackColor = true;
            // 
            // rotateCclkWsButton
            // 
            this.rotateCclkWsButton.BackgroundImage = global::robotController_PubNub.Properties.Resources.ic_rotate_left_48px_256;
            this.rotateCclkWsButton.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.rotateCclkWsButton.FlatAppearance.BorderColor = System.Drawing.Color.Red;
            this.rotateCclkWsButton.FlatAppearance.BorderSize = 0;
            this.rotateCclkWsButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.rotateCclkWsButton.Location = new System.Drawing.Point(27, 272);
            this.rotateCclkWsButton.Name = "rotateCclkWsButton";
            this.rotateCclkWsButton.Size = new System.Drawing.Size(64, 64);
            this.rotateCclkWsButton.TabIndex = 12;
            this.rotateCclkWsButton.UseVisualStyleBackColor = true;
            // 
            // rotateClkWsButton
            // 
            this.rotateClkWsButton.BackgroundImage = global::robotController_PubNub.Properties.Resources.ic_rotate_right_48px_256;
            this.rotateClkWsButton.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.rotateClkWsButton.FlatAppearance.BorderColor = System.Drawing.Color.Red;
            this.rotateClkWsButton.FlatAppearance.BorderSize = 0;
            this.rotateClkWsButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.rotateClkWsButton.Location = new System.Drawing.Point(167, 272);
            this.rotateClkWsButton.Name = "rotateClkWsButton";
            this.rotateClkWsButton.Size = new System.Drawing.Size(64, 64);
            this.rotateClkWsButton.TabIndex = 13;
            this.rotateClkWsButton.UseVisualStyleBackColor = true;
            // 
            // autoButton
            // 
            this.autoButton.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.autoButton.FlatAppearance.BorderColor = System.Drawing.Color.Red;
            this.autoButton.FlatAppearance.BorderSize = 0;
            this.autoButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.autoButton.Location = new System.Drawing.Point(97, 272);
            this.autoButton.Name = "autoButton";
            this.autoButton.Size = new System.Drawing.Size(64, 64);
            this.autoButton.TabIndex = 14;
            this.autoButton.Text = "Auto Mode";
            this.autoButton.UseVisualStyleBackColor = true;
            this.autoButton.Click += new System.EventHandler(this.autoButton_Click);
            // 
            // upButton
            // 
            this.upButton.BackgroundImage = global::robotController_PubNub.Properties.Resources.arrow3;
            this.upButton.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.upButton.FlatAppearance.BorderColor = System.Drawing.Color.Red;
            this.upButton.FlatAppearance.BorderSize = 0;
            this.upButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.upButton.Location = new System.Drawing.Point(97, 60);
            this.upButton.Name = "upButton";
            this.upButton.Size = new System.Drawing.Size(64, 64);
            this.upButton.TabIndex = 11;
            this.upButton.UseVisualStyleBackColor = true;
            // 
            // downButton
            // 
            this.downButton.BackgroundImage = global::robotController_PubNub.Properties.Resources.arrow1;
            this.downButton.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.downButton.FlatAppearance.BorderColor = System.Drawing.Color.Red;
            this.downButton.FlatAppearance.BorderSize = 0;
            this.downButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.downButton.Location = new System.Drawing.Point(97, 200);
            this.downButton.Name = "downButton";
            this.downButton.Size = new System.Drawing.Size(64, 64);
            this.downButton.TabIndex = 10;
            this.downButton.UseVisualStyleBackColor = true;
            // 
            // leftButton
            // 
            this.leftButton.BackgroundImage = global::robotController_PubNub.Properties.Resources.arrow2;
            this.leftButton.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.leftButton.FlatAppearance.BorderColor = System.Drawing.Color.Red;
            this.leftButton.FlatAppearance.BorderSize = 0;
            this.leftButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.leftButton.Location = new System.Drawing.Point(27, 130);
            this.leftButton.Name = "leftButton";
            this.leftButton.Size = new System.Drawing.Size(64, 64);
            this.leftButton.TabIndex = 9;
            this.leftButton.UseVisualStyleBackColor = true;
            // 
            // rightButton
            // 
            this.rightButton.BackgroundImage = global::robotController_PubNub.Properties.Resources.arrow;
            this.rightButton.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.rightButton.FlatAppearance.BorderColor = System.Drawing.Color.Red;
            this.rightButton.FlatAppearance.BorderSize = 0;
            this.rightButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.rightButton.ForeColor = System.Drawing.SystemColors.Control;
            this.rightButton.Location = new System.Drawing.Point(167, 130);
            this.rightButton.Name = "rightButton";
            this.rightButton.Size = new System.Drawing.Size(64, 64);
            this.rightButton.TabIndex = 8;
            this.rightButton.UseVisualStyleBackColor = true;
            // 
            // readyButton
            // 
            this.readyButton.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.readyButton.FlatAppearance.BorderColor = System.Drawing.Color.Red;
            this.readyButton.FlatAppearance.BorderSize = 0;
            this.readyButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.readyButton.Location = new System.Drawing.Point(179, 12);
            this.readyButton.Name = "readyButton";
            this.readyButton.Size = new System.Drawing.Size(64, 64);
            this.readyButton.TabIndex = 15;
            this.readyButton.Text = "ready";
            this.readyButton.UseVisualStyleBackColor = true;
            // 
            // Controller
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(255, 352);
            this.Controls.Add(this.readyButton);
            this.Controls.Add(this.autoButton);
            this.Controls.Add(this.rotateClkWsButton);
            this.Controls.Add(this.rotateCclkWsButton);
            this.Controls.Add(this.upButton);
            this.Controls.Add(this.downButton);
            this.Controls.Add(this.leftButton);
            this.Controls.Add(this.rightButton);
            this.Controls.Add(this.stopButton);
            this.Controls.Add(this.label1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.KeyPreview = true;
            this.MaximizeBox = false;
            this.MaximumSize = new System.Drawing.Size(271, 390);
            this.MinimumSize = new System.Drawing.Size(271, 390);
            this.Name = "Controller";
            this.Text = "Controller";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Controller_FormClosed);
            this.Load += new System.EventHandler(this.Controller_Load);
            this.MouseLeave += new System.EventHandler(this.Controller_MouseLeave);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button upButton;
        private System.Windows.Forms.Button downButton;
        private System.Windows.Forms.Button leftButton;
        private System.Windows.Forms.Button rightButton;
        private System.Windows.Forms.Button stopButton;
        private System.Windows.Forms.Button rotateCclkWsButton;
        private System.Windows.Forms.Button rotateClkWsButton;
        private System.Windows.Forms.Button autoButton;
        private System.Windows.Forms.Button readyButton;
    }
}