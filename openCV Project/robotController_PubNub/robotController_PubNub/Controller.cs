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
using PubNubMessaging.Core;
namespace robotController_PubNub
{
    public partial class Controller : Form
    {
        SerialPort rs232;
        Pubnub pubnub;
        byte[] auto = new byte[1] { 0x09 };
        byte[] stop = new byte[1] { 0x01 };
        byte[] right = new byte[1] { 0x02 };
        byte[] left = new byte[1] { 0x03 };
        byte[] forward = new byte[1] { 0x06 };
        byte[] backward = new byte[1] { 0x07 };
        byte[] cWise = new byte[1] { 0x04 };
        byte[] cCwise = new byte[1] { 0x05 };
        byte[] ready = new byte[1] { 0x09 };

        public Controller(string com,int baud)
        {
            InitializeComponent();
            KeyPreview = true;
            rs232 = new SerialPort(com, baud);
            rs232.Open();
        }

        private void Controller_Load(object sender, EventArgs e)
        {
            pubnub = new Pubnub("pub-c-5459b9ce-6b95-436d-a11b-2f50fc894d21", "sub-c-0a557698-fd8f-11e4-b54a-0619f8945a4f");
            pubnub.Subscribe<string>("robot", DisplaySubscribeReturnMessage, DisplaySubscribeConnectStatusMessage, DisplayErrorMessage);
            foreach (var button in Controls.OfType<Button>())
            {
                button.Click += button_Click;
            }

        }
        private void button_Click(object sender, EventArgs eventArgs)
        {
            selectButton((Button)sender);
        }
        protected override bool ProcessCmdKey(ref Message msg, Keys keyData)
        {
            switch (keyData)
            {
                case Keys.Down:
                    selectButton(downButton);
                    break;
                case Keys.Up:
                    selectButton(upButton);
                    break;
                case Keys.Right:
                    selectButton(rightButton);
                    break;
                case Keys.Left:
                    selectButton(leftButton);
                    break;
                case Keys.A:
                    selectButton(rotateCclkWsButton);
                    break;
                case Keys.D:
                    selectButton(rotateClkWsButton);
                    break;
                case Keys.S:
                    selectButton(autoButton);
                    break;
                default:
                    selectButton(stopButton);
                    break;

            }
            
            return base.ProcessCmdKey(ref msg, keyData);
        }
        void selectButton(Button n)
        {
            foreach(Control b in this.Controls)
                if (b is Button)
                {
                    ((Button)b).FlatAppearance.BorderSize = 0;
                    n.FlatAppearance.BorderSize = 3;
                }
            
            if (n.Name == rightButton.Name)
                rs232.Write(right,0,1);
            else if(n.Name == leftButton.Name)
                rs232.Write(left, 0, 1);
            else if (n.Name == upButton.Name)
                rs232.Write(forward, 0, 1);
            else if (n.Name == downButton.Name)
                rs232.Write(backward, 0, 1);
            else if (n.Name == rotateClkWsButton.Name)
                rs232.Write(cWise, 0, 1);
            else if (n.Name == rotateCclkWsButton.Name)
                rs232.Write(cCwise, 0, 1);
            else if (n.Name == stopButton.Name)
                rs232.Write(stop, 0, 1);
            else if (n.Name == autoButton.Name)
                rs232.Write(auto, 0, 1);
            else if (n.Name == readyButton.Name)
                rs232.Write(ready, 0, 1);
        }
        void DisplaySubscribeReturnMessage(string result)
        {
            string str = result.Substring(1, 1);
            int input = Convert.ToInt32(str);
            switch (input)
            {
                case 7:
                    selectButton(downButton);
                    break;
                case 6:
                    selectButton(upButton);
                    break;
                case 2:
                    selectButton(rightButton);
                    break;
                case 3:
                    selectButton(leftButton);
                    break;
                case 4:
                    selectButton(rotateCclkWsButton);
                    break;
                case 5:
                    selectButton(rotateClkWsButton);
                    break;
                case 0:
                    selectButton(autoButton);
                    break;
                default:
                    selectButton(stopButton);
                    break;
            }
        }
        static void DisplaySubscribeConnectStatusMessage(string result)
        {
           
        }
        static void DisplayErrorMessage(PubnubClientError result)
        {
            
        }

        private void Controller_MouseLeave(object sender, EventArgs e)
        {
            label1.Focus();
        }

        private void Controller_FormClosed(object sender, FormClosedEventArgs e)
        {
            Application.Exit();
        }

        private void autoButton_Click(object sender, EventArgs e)
        {

        }
    }
}
