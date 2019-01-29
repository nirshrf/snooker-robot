#ifndef STRATEGY_H_INCLUDED
#define STRATEGY_H_INCLUDED
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/opencv.hpp"
#include <windows.h>
#include <math.h>
#include <sstream>
#include <algorithm>
#include <math.h>
# define PI           3.14159265358979323846
using namespace std;
using namespace cv;

class ball
{
    Point p;
    string color;
    double rad;
public:
    ball(Point pi,string colori,double radi);
    Point getPoint();
    string getName();
    double getRadius();
};
class route
{
    int ballId,holeId;
    double rating;
public:
    route(int ballIdInput,int holeIdInput,double ratingInput);
    double getRating();
    void addToRating(double d);
    int getBallId();
    int getHoleId();
};

class strat
{
public:
    Point holes[6] = {Point(30,30),Point(320,30),Point(610,30),Point(30,450),Point(320,450),Point(610,450)};
    Mat clean;
    Mat frameClone;
    Mat frame;
    const double cp = 3.5/26;
    void getPositions(VideoCapture cap);
    void detectRed(Mat e);
    void detectBlue(Mat e);
    void detectGreen(Mat e);
    void detectWhite(Mat e);
    void detectHoles(Mat img);
    Mat findRect(Mat img);
    Mat detectTable(Mat e);
    void ballHandler(Mat img,string color);
    void drawRoute(route r);
    Point simulateKick(int ballId,int holeId);
    double calcDegrees(Point wb,Point tb,Point hole);
    double slope(Point p1,Point p2);
    int checkIfBlocked(int ballId,int holeId); // p1 & p2 = white ball & hole
    int checkIfCenterOnArea(int holeId,Point tempBall,int ballId);
    int checkIfBallOnArea(int holeId,Point tempBall,int ballId);
    double stat_distScore(int ballId,int holeId);
    route stat_createRoute(int ballId);
    bool cmd(route r1, route r2);
    void bestRoute();
    Point* findRectPoints(Point center1,Point center2);
    string intToString (int a);
    vector<ball> balls;
    vector<double> blackList;
    vector<Point> edges;
    int windex;
    Point simBall;
    bool whiteBallFound,holesFound;
    void distRB();
    double Distance(Point p1,Point p2);

};

#endif // STRATEGY_H_INCLUDED
