using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace robotController_PubNub
{
    public partial class configForm : Form
    {
        public configForm()
        {
            InitializeComponent();
        }
        private void Form1_Load(object sender, EventArgs e)
        {
            foreach (string str in SerialPort.GetPortNames())
            {
                ComsComboBox.Items.Add(str);
            }
            if (!Properties.Settings.Default.firstTime)
            {
                int index = ComsComboBox.FindString(Properties.Settings.Default.port);
                ComsComboBox.SelectedIndex = index;
                index = baudComboBox.FindString(Properties.Settings.Default.baud.ToString());
                baudComboBox.SelectedIndex = index;
            }
            
        }

        private void startButton_Click(object sender, EventArgs e)
        {
            if (ComsComboBox.SelectedIndex > -1 && baudComboBox.SelectedIndex > -1)
            {
                string com = ComsComboBox.SelectedItem.ToString();
                int baud = Convert.ToInt32(baudComboBox.SelectedItem.ToString());
                if (SerialPort.GetPortNames().Contains(com))
                {
                    Properties.Settings.Default.port = com;
                    Properties.Settings.Default.baud = baud;
                    if (Properties.Settings.Default.firstTime)
                        Properties.Settings.Default.firstTime = false;
                    Properties.Settings.Default.Save();
                    Controller controller = new Controller(com, baud);
                    controller.Show();
                    this.Hide();
                }
            }
            else
                MessageBox.Show("Please Choose Valid Port and Baudrate!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }
    }
}
