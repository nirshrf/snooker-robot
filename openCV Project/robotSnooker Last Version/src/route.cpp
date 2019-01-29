#include "route.h"

 route::route(int ballIdInput,int holeIdInput,double ratingInput)
    {
        ballId = ballIdInput;
        holeId = holeIdInput;
        rating = ratingInput;
    }
    double route::getRating()
    {
        return rating;
    }
    void route::addToRating(double d)
    {
        rating = rating + d;
    }
    int route::getBallId()
    {
        return ballId;
    }
    int route::getHoleId()
    {
        return holeId;
    }
    route::~route()
    {

    }
