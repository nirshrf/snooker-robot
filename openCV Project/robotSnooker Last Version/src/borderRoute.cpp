#include "borderRoute.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/opencv.hpp"

borderRoute::borderRoute(Point p,double d)
{
    borderPoint = p;
    rating = d;
}
Point borderRoute::getBorderPoint()
{
    return borderPoint;
}
double borderRoute::getRating()
{
    return rating;
}
borderRoute::~borderRoute()
{
    //dtor
}
