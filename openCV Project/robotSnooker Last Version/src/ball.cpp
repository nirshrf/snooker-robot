#include "ball.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;
    //
    //
    //
    ball::ball(Point pi,string colori,double radi)
    {
        p = pi;
        color = colori;
        rad = radi;
    }
    Point ball::getPoint()
    {
        return p;
    }
    string ball::getName()
    {
        return color;
    }
    double ball::getRadius()
    {
        return rad;
    }
    ball::~ball()
    {

    }
