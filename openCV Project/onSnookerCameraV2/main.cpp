#include <iostream>
//#include "include/Circle.h"
#include "include/fineTuning.h"
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <thread>
#include "comm.h"

using namespace cv;
using namespace std;

comm *cm;

void callFineTuning()
{
    //fineTuning *fnt = new fineTuning();
    fineTuning fnt(cm);
    cout << "Starting next round.." << endl;
//    delete fnt;
}
int main()
{
    int x = 0;
    thread *run;
    while(1)
    {
        if(x==0){
           cm = new comm();
           run = new thread(callFineTuning);
           x=1;
        }
        if(run->joinable())
        {
            run->join();
            x=0;
            cout << "new thread" << endl;
            usleep(1000000);
        }

    }
    delete cm;
}
