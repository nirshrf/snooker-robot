#ifndef STRAT_H
#define STRAT_H
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/opencv.hpp"
#include <windows.h>
#include <math.h>
#include <sstream>
#include <algorithm>
#include <math.h>
#include "ball.h"
#include "route.h"
# define PI           3.14159265358979323846

using namespace std;
using namespace cv;

class strat
{
    public:
        strat();
        virtual ~strat();
        vector<Point> getPositions(VideoCapture cap);
        vector<Point> getPositions(Mat img);
        vector<Point> edges;
        vector<Point> borderPoints;
        vector<Point> loadBorderPoints(Mat fr);
        void detectWhite(Mat e);
        Mat detectTable(Mat e);
        Mat findRect(Mat img);
        Mat equalizeMat(Mat inputMat);
        Point simulateKick(int ballId,int holeId);
        Point findY(double x1,double y1,double m,double inX);
        Point findX(double x1,double y1,double m,double inY);
        double calcDegrees(Point wb,Point tb,Point hole);
        double slope(Point p1,Point p2);
        double Distance(Point p1,Point p2);
        struct sortingAlg;
        struct sortingBorderAlg;
        Point* findRectPoints(Point center1,Point center2);
        string intToString (int a);
        int getBallsSize();

    protected:
    private:
        Mat clean,frameClone,frame;
        Point simBall;
        Point holes[6] = {Point(30,30),Point(320,30),Point(610,30),Point(30,450),Point(320,450),Point(610,450)};
        void detectRed(Mat e);
        void detectBlue(Mat e);
        void detectGreen(Mat e);
        void detectHoles(Mat _img);
        void ballHandler(Mat img,string color);
        void drawRoute(route r,double multi,int winner);
        void detectWhiteBall();
        void bestRoute();
        void findBestShot();
        int ballCount = 0,windex;
        bool checkIfBlocked(int ballId,int holeId); // p1 & p2 = white ball & hole
        int checkIfCenterOnArea(int holeId,Point tempBall,int ballId);
        int checkIfBallOnArea(int holeId,Point tempBall,int ballId);
        double stat_distScore(int ballId,int holeId);
        double stat_whiteRedDist(int ballId,int holeId);
        double stat_relatedDegree(int ballId,int holeId);
        double stat_whiteDist(int ballId,int holeId);
        bool whiteBallFound,holesFound;
        vector<ball> balls;
        vector<double> blackList;
        route stat_createRoute(int ballId,int holeId,bool failureRoute);
};



#endif // STRAT_H
