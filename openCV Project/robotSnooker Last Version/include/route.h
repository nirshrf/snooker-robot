#ifndef ROUTE_H
#define ROUTE_H
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/opencv.hpp"


using namespace std;
using namespace cv;

class route
{
    public:
        virtual ~route();
        route(int ballIdInput,int holeIdInput,double ratingInput);
        double getRating();
        void addToRating(double d);
        int getBallId();
        int getHoleId();
    protected:
    private:
        int ballId,holeId;
        double rating;
};
#endif // ROUTE_H
