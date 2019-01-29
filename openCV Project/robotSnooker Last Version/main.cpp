#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <algorithm>
#include <vector>
#include <math.h>
#include "strat.h"
#include "RS232.h"
#include "targeting.h"

using namespace cv;
using namespace std;

RS232::SerialConnection serConn;
VideoCapture cap(0);
unsigned char b = '0';
int main()
{


    while(1)
    {
        serConn.OpenPort(4u,115200u);
        targeting target(serConn,cap);
        if(target.st.getBallsSize() > 0)
        {
            int x = 0;
            while(x<200)
            {
                Sleep(100);
                x++;
                cout << x << endl;
            }
            serConn.SendByte(4u,9u);
            cout << "NEW ROUND" << endl;
        }
        else
            break;
    }
    cout << "Someone won! :D" << endl;
}




