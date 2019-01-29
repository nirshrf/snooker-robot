#include "fineTuning.h"
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
#include "comm.h"
using namespace cv;
using namespace std;

fineTuning::fineTuning(comm *cm)
{
    communication = cm;
    screenCenter = Point(160, 120);
    diff = 10;
    left_x = screenCenter.x - diff;
    right_x = screenCenter.x + diff;
    VideoCapture cap(0);

    cap.set(CV_CAP_PROP_FRAME_WIDTH, 320);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 240);
    Point l_top(left_x, 0);
    Point l_bottom(left_x, 240);
    Point r_top(right_x, 0);
    Point r_bottom(right_x, 240);
    while(1)
    {
        Mat frame, frameClone;
        cap >> frame;
        frameClone = frame.clone();
        frame = colorFilter(frame);
        Point center = detectCircle(frame);
        Point cSweetSpot = sweetSpot(center);
        int ret = direct(center);
        if(ret == 1)
        {
            communication->sendData(1);
            break;
        }
        /// Debugging
        circle( frameClone, center, 3, Scalar(0,0,255), -1, 8, 0 );
        circle( frameClone, screenCenter, 3, Scalar(255,0,0), -1, 8, 0 );
      //  Point t_left(0, screenCenter.y - 10);
      //  Point t_right(320, screenCenter.y - 20);
      //  Point b_left(0, screenCenter.y + 20);
     //   Point b_right(320, screenCenter.y + 20);
        line(frameClone, l_top, l_bottom, Scalar(0,0,0), 1, 8, 0);
        line(frameClone, r_top, r_bottom, Scalar(0,0,0), 1, 8, 0);
        //line(frameClone, t_left, t_right, Scalar(0,0,0), 1, 8, 0);
        //line(frameClone, b_left, b_right, Scalar(0,0,0), 1, 8, 0);
        /// End
        imshow("filtered", frame);
        imshow("frame", frameClone);
        if(waitKey(30) >= 0) break;
    }
    communication->sendData(0);
}
Mat fineTuning::colorFilter(Mat frame)
{
    cvtColor(frame, frame, COLOR_BGR2HSV);
    inRange(frame, Scalar(12, 27, 97), Scalar(46, 255, 255), frame);
    /// Noise cleaning
    erode(frame, frame, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    //dilate(frame, frame, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    //dilate(frame, frame, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    //erode(frame, frame, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    /// Blur filter
    GaussianBlur( frame, frame, Size(9, 9), 2, 2 );
    /// Detect edges using Threshold
    //threshold(frame, frame, 100, 255, THRESH_BINARY );
    return frame;
}

Point fineTuning::detectCircle(Mat frame)
{
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours( frame, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    vector<vector<Point> > contours_poly( contours.size() );
    vector<Rect> boundRect(contours.size());
    vector<Point2f>center2(contours.size());
    for( int i = 0; i < contours.size(); i++ )
    {
        approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
        boundRect[i] = boundingRect( Mat(contours_poly[i]) );
        int aX = (boundRect[i].tl().x + boundRect[i].br().x) / 2 ,
        aY = (boundRect[i].tl().y + boundRect[i].br().y) / 2;
        Point result(aX,aY);
        return result;
    }
    return Point(100,100);
}

Point fineTuning::sweetSpot(Point center) {
    float y = (7 * center.y) / 5;
    Point result(center.x, y);
    return result;
}
int fineTuning::hit()
{
  //  usleep(5000);
    communication->sendData(2);
    usleep(1000000);
    communication->sendData(1);
    usleep(8000000);
    communication->sendData(0);
    usleep(2000000);
    communication->sendData(3);
    return 1;
  //  communication->sendData(3);
}
int fineTuning::direct(Point center)
{
    if(center.x < left_x)
    {
        c = 0;
        cout << "go left" << endl;
        communication->sendData(5);
        int left_diff = difference(left_x,center.x);
        int sleep_left_diff = left_diff*100;
        usleep(20000 + sleep_left_diff);
        stop();
    }
    else if(center.x > right_x)
    {
        c = 0;
        cout << "go right" << endl;
        communication->sendData(4);
        int right_diff = difference(center.x,right_x);
        int sleep_right_diff = right_diff*100;
        usleep(20000 + sleep_right_diff);
        stop();
    }
    else if(center.x >= left_x & center.x <= right_x)
    {
        c++;
        if (c > 10)
        {
            cout<< "hit"<< endl;
            c = 0;
            usleep(1000000);
            return hit();
        }
    }
    return 0;
}
void fineTuning::stop()
{
    communication->sendData(6);
}

int fineTuning::difference(int x , int y)
{
    return(x-y);
}

fineTuning::~fineTuning()
{
    cout << "thread closing" << endl;
    //terminate();
}
