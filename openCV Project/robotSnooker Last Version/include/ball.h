#ifndef BALL_H
#define BALL_H
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

class ball
{
    public:
        virtual ~ball();
        ball(Point pi,string colori,double radi);
        Point getPoint();
        string getName();
        double getRadius();
    protected:
    private:
        Point p;
        string color;
        double rad;
};

#endif // BALL_H
