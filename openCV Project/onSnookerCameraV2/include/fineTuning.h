#ifndef FINETUNING_H
#define FINETUNING_H
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include "comm.h"

using namespace cv;
using namespace std;

class fineTuning
{
    public:
        fineTuning(comm *cm);
        comm *communication;
        Point screenCenter;
        int diff;
        int c;
        int left_x;
        int right_x;
        Mat colorFilter(Mat frame);
        Point detectCircle(Mat frame);
        Point sweetSpot(Point center);
        int difference(int x , int y);
        int direct(Point center);
        void stop();
        void sendData(int input);
        int hit();
        virtual ~fineTuning();
    protected:
    private:
};

#endif // FINETUNING_H
