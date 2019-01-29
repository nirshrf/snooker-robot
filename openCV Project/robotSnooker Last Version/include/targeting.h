#ifndef TARGETING_H
#define TARGETING_H
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <algorithm>
#include <vector>
#include <math.h>
#include "strat.h"
#include "RS232.h"
#include "borderRoute.h"

using namespace cv;
using namespace std;
typedef enum states {stopping,rlMoving,rotating,fbMoving,target,looking}states;

class targeting
{
    public:
        targeting(RS232::SerialConnection serConnx,VideoCapture cap);
        Mat frame;
        strat st;
        RS232::SerialConnection serConn;
        int option = 0;
        bool wayToSort(int i, int j) { return i > j; }
        void accurateShot(VideoCapture cap,Point wb,Point sb);
        bool drawLine(Point wb,Point sb);
        bool robotMovingDirection(Point wb,Point sb);
        Point findY(double x1,double y1,double m,double inX);
        Point findX(double x1,double y1,double m,double inY);
        Point findRobotPos(Point wb,Point sb);
        Point GetWhileBall();
        double calcDegrees(Point wb,Point tb,Point hole);
        Point findHandCenter(Mat img);
        Point robotPos;
        int difference(int x, int y);
        void drawShapes(Point p1,Point p2);
        double dgr(Point start,Point p1,Point p2);
        double range = 0.5;
        struct sortingBorderAlg;
        virtual ~targeting();
    protected:
    private:
        Point wb;
        const unsigned short port = 4u;
        const unsigned baud = 115200u;
        const unsigned char b_stop = 1u;
        const unsigned char b_right = 2u;
        const unsigned char b_left = 3u;
        const unsigned char b_clockWise = 4u;
        const unsigned char b_cClockWise = 17u;
        const unsigned char b_up = 6u;
        const unsigned char ready = 9u;
        const unsigned char b_down = 7u;
        void drawHandShapes(Point handPoint,Point sb);
        void states_target();
        states states_fbMoving(Point handPoint,Point wb);
        states states_rlMoving(double degree);
        states states_rotating(double dgree,Point handPoint,Point sb);
        vector<Point> borderPoints;

};

#endif // TARGETING_H
