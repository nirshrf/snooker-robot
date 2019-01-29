#ifndef BORDERROUTE_H
#define BORDERROUTE_H
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

class borderRoute
{
    public:
        borderRoute(Point p,double d);
        Point getBorderPoint();
        double getRating();
        Point borderPoint;
        double rating;
        virtual ~borderRoute();
    protected:
    private:
};

#endif // BORDERROUTE_H
