#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/opencv.hpp"
#include <windows.h>
#include <math.h>
#include <sstream>
#include <algorithm>
#include <math.h>
# define PI           3.14159265358979323846
#include "strategy.h"

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
    //
    //
    //
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
    //
    //
    //
    void strat::getPositions(VideoCapture cap)
    {
        whiteBallFound = false;
        holesFound = false;
        namedWindow("frames",1);
        while(whiteBallFound == false || holesFound == false)
        {
            static int counter = 0;
            cap >> frame;
          //  frame = imread("picture.jpg", CV_LOAD_IMAGE_COLOR);
            frameClone = frame.clone();
            frame = detectTable(frame);
            clean = frame.clone();
            if(counter > 10)
            {
                try
                {
                    detectRed(clean);
                    detectHoles(frame);
                    detectWhite(frame);
                    distRB();
                    bestRoute();
                }
                catch (cv::Exception)
                {
                    cout << "exception" << endl;
                }

            }
            imshow("frames", clean);
            balls.clear();
            counter++;
            if(waitKey(30) >= 0) break;

        }
        cout << "White Ball : " << balls[windex].getPoint() << endl;
        cout << "Simulated Point : " << simBall << endl;
    }
    void strat::detectRed(Mat e)
    {
        Mat hsv = e.clone(),part1,part2,redScreen;
        cvtColor(hsv,hsv,CV_BGR2HSV);
        inRange(hsv, Scalar(0, 100, 100), Scalar(10, 255, 255), part1);
        inRange(hsv, Scalar(160, 100, 100), Scalar(179, 255, 255), part2);
        addWeighted(part1, 1.0, part2, 1.0, 0.0, redScreen);
        GaussianBlur(redScreen,redScreen, Size(9, 9), 2, 2);
        ballHandler(redScreen,"red");
        imshow("red",redScreen);
    }
    void strat::detectBlue(Mat e)
    {
        Mat img = e.clone();
        cvtColor(img, img,COLOR_BGR2HSV);
        inRange(img, Scalar(98, 50, 50), Scalar(120, 255, 255), img);
        GaussianBlur(img, img, Size(9, 9), 2, 2);
        ballHandler(img,"blue");
        //imshow("Blue", img);

    }
    void strat::detectGreen(Mat e)
    {
        Mat img = e;
        cvtColor(img, img,COLOR_BGR2HSV);
        inRange(img, Scalar(0, 0, 66), Scalar(179, 255, 111), img);
        GaussianBlur(img, img, Size(9, 9), 2, 2);
        ballHandler(img,"green");
        //imshow("Green", img);

    }
    void strat::detectWhite(Mat e)
    {
        Mat img;

        cvtColor(e, img,COLOR_BGR2HSV);
        inRange(img, Scalar(0, 136, 157), Scalar(179,247,255), img);
        erode(img, img, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        dilate( img, img, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        //morphological closing (fill small holes in the foreground)
        dilate( img, img, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        erode(img, img, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
       // bitwise_not(img,img);
        GaussianBlur(img, img, Size(9, 9), 2, 2);
        ballHandler(img,"white");
        imshow("White", img);
    }
    void strat::detectHoles(Mat img)
    {
        cvtColor(img, img,COLOR_BGR2HSV);
        inRange(img, Scalar(0,0,51), Scalar(179,255,255), img);
        erode(img, img, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        dilate( img, img, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        //morphological closing (fill small holes in the foreground)
        dilate( img, img, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        erode(img, img, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        bitwise_not(img,img);
        GaussianBlur(img, img, Size(9, 9), 2, 2);
        imshow("holes",img);
        vector<Vec3f> circles;
        HoughCircles(img, circles, CV_HOUGH_GRADIENT, 1, img.rows/8, 100, 20, 0, 0);
        int add = 0;
        if(circles.size() == 6)
        {
            add = 1;
            holesFound = true;
        }
        else
        {
            cout << "couldnt find 6 holes!" << endl;
            holesFound = false;
        }
        for(size_t current_circle = 0; current_circle < circles.size(); ++current_circle)
        {
                Point center = Point(round(circles[current_circle][0]), round(circles[current_circle][1]));
                int radius = round(circles[current_circle][2]);
                circle(clean, center, radius, Scalar(255, 255, 255), 1);
                circle(clean, center, 1, Scalar(0, 255, 0), 3);
                if(add==1)
                    holes[current_circle] = center;
        }

    }
    void strat::distRB()
    {
        system("CLS");
        int isize = balls.size();
        windex = -1;
        for(int i = 0; i != isize; i++)
        {
           if(balls[i].getName() == "white1")
            windex = i;
        }
        if(windex >= 0)
        {
            if(balls[windex].getPoint() != Point(0,0))
            {
                for(int i = 0; i != isize; i++)
                {
                    if(i != windex)
                    {
                        double d = Distance(balls[windex].getPoint(),balls[i].getPoint());
                        d = d * cp;
                        line(clean,balls[windex].getPoint(),balls[i].getPoint(),Scalar(255,255,255));
                       // cout << "Distance between " << balls[windex].getName() << " & " << balls[i].getName() << " : " << d << endl;
                    }
                }
            }
            whiteBallFound = true;
        }
        else
        {
            cout << "White ball was not found!" << endl;
            whiteBallFound = false;
        }

    }
    double strat::Distance(Point p1,Point p2)
    {
        return sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
    }
    string strat::intToString (int a)
    {
        ostringstream t;
        t<<a;
        return t.str();
    }
    void strat::ballHandler(Mat img,string color)
    {
        vector<Vec3f> circles;
        HoughCircles(img, circles, CV_HOUGH_GRADIENT, 1, 10, 100, 20, 8,0);
        Point center;
        for(size_t current_circle = 0; current_circle < circles.size(); ++current_circle)
        {

            if(color != "white" || (color == "white" && circles.size() == 1))
            {
                center = Point(round(circles[current_circle][0]), round(circles[current_circle][1]));
                int radius = round(circles[current_circle][2]);
                circle(clean, center, radius, Scalar(0, 0, 0), 1);
                circle(clean, center, 1, Scalar(0, 0, 0), 3);
                ball b(center,color + intToString(current_circle+1),circles[current_circle][2]);
                balls.push_back(b);
                putText(clean,color + intToString(current_circle+1),Point(center.x - 14,center.y - 8),1,0.5,Scalar(0, 0, 0),1);
            }
        }
    }
    double strat::calcDegrees(Point wb,Point tb,Point hole) // wb = whiteball, tb = target ball
    {
        double a = Distance(wb,tb);
        double b = Distance(tb,hole);
        double c = Distance(wb,hole);
        double cosinAlpha = ((a * a ) + (b * b) - (c * c)) / (2 * a * b);
        double dgr = acos(cosinAlpha) * 180.0 / PI;
        return dgr;
    }
    void strat::findBestShot()
    {
        double bigI = 0;
        double degrees[balls.size()][6];
        for(int i = 0;i<balls.size();i++)
        {
            if(i != windex)
            {
                double bigJ = 0;
                for(int j = 0;j<6;j++)
                {
                    double dgr = calcDegrees(balls[windex].getPoint(),balls[i].getPoint(),holes[j]);
                    degrees[i][j] = dgr;
                    if(find(blackList.begin(), blackList.end(), dgr) == blackList.end())
                    {
                        if(abs(180-dgr) < abs(180-bigJ))
                        bigJ = dgr;
                    }


                }
                if(abs(180-bigJ) < abs(180-bigI))
                    bigI = bigJ;
            }
        }
        for(int i = 0;i<balls.size();i++)
            for(int j = 0;j<6;j++)
                if(degrees[i][j] == bigI)
                {
                    if(find(blackList.begin(), blackList.end(), bigI) == blackList.end())
                    {
                        Point newP = simulateKick(i,j);
                        int blocked = checkIfBlocked(i,j);
                        if(blocked > 0)
                        {
                            blackList.push_back(bigI);
                            findBestShot();
                             cout << "LOOKING FOR ANOTHER ROUTE" << endl;

                        }
                        else
                        {
                            blackList.clear();
                        }
                    }
                }

    }
    Point strat::simulateKick(int ballId,int holeId)
    {
        double m = slope(holes[holeId],balls[ballId].getPoint());
        double dgr = atan(m) * 180.0 / PI;
        double w = cos(dgr * PI / 180) * (balls[ballId].getRadius() + balls[windex].getRadius());
        double h = sin(dgr * PI / 180) * (balls[ballId].getRadius() + balls[windex].getRadius());
        int x,y;
        if(balls[ballId].getPoint().x < holes[holeId].x)
        {
            x = balls[ballId].getPoint().x - round(w);
            y = balls[ballId].getPoint().y - round(h);
        }
        else
        {
            x = balls[ballId].getPoint().x + round(w);
            y = balls[ballId].getPoint().y + round(h);
        }
        Point newPoint(x, y);
        return newPoint;
    }

    int strat::checkIfBlocked(int ballId,int holeId) // p1 & p2 = white ball & hole
    {
        int errors = 0;
        Point tempBall = simulateKick(ballId,holeId);
        errors += checkIfCenterOnArea(holeId,tempBall,ballId);
        errors += checkIfBallOnArea(holeId,tempBall,ballId);

        /*
        if(errors == 0)
            cout << "No Balls on road" << endl;
        else
            cout << "Balls on road!" << endl;
        */
        return errors;
    }
    int strat::checkIfCenterOnArea(int holeId,Point tempBall,int ballId)
    {
        int ct = 0;
        Point pts[] = {balls[windex].getPoint(),tempBall,holes[holeId]}; // works only with 3 points
        Point blocksPoints[2][4];
        for(int i = 0;i<2;i++)
        {
            Point *p = findRectPoints(pts[i],pts[i+1]);
            for(int j = 0;j<4;j++)
            {
                blocksPoints[i][j] = *(p + j);
            }
        }
        Mat image = Mat::zeros(clean.size(),CV_8UC1);
        for(int i = 0;i<2;i++)
        {
            const Point* ppt[1] = { blocksPoints[i] };
            int npt[] = { 4 };
            fillPoly( image, ppt, npt, 1, Scalar( 255, 255, 255 ), 8 );
        }
        circle(image,tempBall,balls[windex].getRadius(),Scalar(255,255,255),-1);
        for(int i = 0;i<balls.size();i++)
        {
            if(i != windex && i != ballId)
            {
                Vec3b colour = image.at<uchar>(balls[i].getPoint());
                if(colour.val[0]==255)
                    ct++;
            }
        }
        return ct;
    }
    int strat::checkIfBallOnArea(int holeId,Point tempBall,int ballId)
    {
        int ct = 0;
        vector<vector<Point> > contours;
        vector<Vec4i> hierarchy;
        double firstArea = 0;
        Point pts[] = {balls[windex].getPoint(),tempBall,holes[holeId]};
        Point blocksPoints[2][4];
        //
        // get black and white image
        //
        for(int i = 0;i<2;i++)
        {
            Point *p = findRectPoints(pts[i],pts[i+1]);
            for(int j = 0;j<4;j++)
            {
                blocksPoints[i][j] = *(p + j);
            }
        }
        Mat image = Mat::zeros(clean.size(),CV_8UC1);
        for(int i = 0;i<2;i++)
        {
            const Point* ppt[1] = { blocksPoints[i] };
            int npt[] = { 4 };
            fillPoly( image, ppt, npt, 1, Scalar( 255, 255, 255 ), 8 );
        }
        //
        //find first area
        //
        Mat tempImage = image.clone();
        bitwise_not(tempImage,tempImage);
        blur( tempImage, tempImage, Size(3,3) );
        Canny( tempImage, tempImage,100,200);
        findContours(tempImage, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
        for( int i = 0; i < contours.size(); i++ )
         {
            double d = contourArea(contours[i]);
            if(d > firstArea)
                firstArea = d;
         }
         //
         //find second area
         //
         contours.clear();
         hierarchy.clear();
         for(int i = 0;i<balls.size();i++)
         {
            if(i != windex && i != ballId)
            {
                circle(image,balls[i].getPoint(),balls[i].getRadius(),Scalar(255,255,255),-1);
            }
         }
         tempImage = image.clone();
         bitwise_not(tempImage,tempImage);
         blur( tempImage, tempImage, Size(3,3) );
         Canny( tempImage, tempImage,100,200);
         findContours(tempImage, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
         for( int i = 0; i < contours.size(); i++ )
         {
            double d = contourArea(contours[i]);
            if(d==firstArea)
            {
                ct = 0;
                break;
            }
            else
                ct = 1;
         }
         //imshow("t",image);
         return ct;
    }
    double strat::slope(Point p1,Point p2)
    {
        double res = (double)(p1.y - p2.y) / (double)(p1.x - p2.x);
        return res;
    }
    Point* strat::findRectPoints(Point center1,Point center2)
    {
        double radius = balls[windex].getRadius();
        Point* finalDots = new Point[4];
        Point cen[2] = {center1,center2};
        for(int i = 0;i<2;i++)
        {
            double m1 = -1 / (slope(center1,center2));
            double dgr = atan(m1) * 180.0 / PI;
            double w = cos(dgr * PI / 180) * (radius);
            double h = sin(dgr * PI / 180) * (radius);
            Point p1(cen[i].x + w,cen[i].y + h);
            Point p2(cen[i].x - w,cen[i].y - h);
            if(i==0)
            {
                finalDots[i + (i*1)] = p1;
                finalDots[(i+1) + (i*1)] = p2;
            }
            else
            {
                finalDots[i + (i*1)] = p2;
                finalDots[(i+1) + (i*1)] = p1;
            }
        }
        return finalDots;
    }
    void strat::drawRoute(route r)
    {
        int ballId = r.getBallId(),holeId = r.getHoleId();
        Point ball = balls[ballId].getPoint();
        Point newPoint = simulateKick(ballId,holeId);
        simBall = newPoint;
        Point pts[] = {balls[windex].getPoint(),newPoint,holes[holeId]};
        Point blocksPoints[2][4];
        circle(clean,newPoint,balls[windex].getRadius(),Scalar(255,0,0),2);
        line(clean,ball,holes[holeId],Scalar(255,255,255));
        line(clean,newPoint,holes[holeId],Scalar(0,0,255));
        line(clean,newPoint,balls[windex].getPoint(),Scalar(0,0,255));
        for(int i = 0;i<2;i++)
        {
            Point *p = findRectPoints(pts[i],pts[i+1]);
            for(int j = 0;j<4;j++)
            {
                blocksPoints[i][j] = *(p + j);
            }
            line(clean,blocksPoints[i][0],blocksPoints[i][3],Scalar(153,0,255));
            line(clean,blocksPoints[i][1],blocksPoints[i][2],Scalar(153,0,255));
            line(clean,blocksPoints[i][0],blocksPoints[i][1],Scalar(153,0,255));
            line(clean,blocksPoints[i][2],blocksPoints[i][3],Scalar(153,0,255));
        }
    }
    Mat strat::detectTable(Mat e)
    {
        Mat img = e.clone();
        cvtColor(img, img,COLOR_BGR2HSV);
        inRange(img, Scalar(30, 200, 0), Scalar(88, 255, 255), img);
        GaussianBlur(img, img, Size(9, 9), 2, 2);
        Mat retMat = findRect(img);
        return retMat;
    }
    Mat strat::findRect(Mat img)
    {
        edges.clear();
        system("CLS");
        Mat retMat = Mat::zeros(img.size(),CV_8UC1);;
        vector<Vec3f> circles;
        HoughCircles(img, circles, CV_HOUGH_GRADIENT, 1, img.rows/8, 100, 20, 20,60);
        Point center;
        for(size_t current_circle = 0; current_circle < circles.size(); ++current_circle)
        {
            center = Point(round(circles[current_circle][0]), round(circles[current_circle][1]));
            int radius = round(circles[current_circle][2]);
            circle(img,center,radius,Scalar(255,255,255),3);
            cout << "Point : " << center << " Radius : " << radius << endl;
            if (circles.size() == 2)
            {
                edges.push_back(center);
            }
        }
        if(edges.size() == 2)
        {
            Rect r(min(edges[0].x,edges[1].x), min(edges[0].y,edges[1].y),abs(edges[0].x - edges[1].x),abs(edges[0].y - edges[1].y));
            retMat = frameClone(r);
        }
        return retMat;
    }
    bool strat::cmd(route r1, route r2)
    {
       return (r1.getRating() > r2.getRating());
    }
    void strat::bestRoute()
    {
        vector<route> routes;
        int bsize = balls.size();
        for(int i = 0;i<bsize;i++)
        {
            if(i != windex)
            {
                route r = stat_createRoute(i);
                double ret = stat_distScore(i,r.getHoleId());
                r.addToRating(ret);
                routes.push_back(r);
                //cout << r.getRating() << endl;
            }
        }
        sort(routes.begin(),routes.end(),cmd);
        //cout << routes[0].getRating() << endl;
        drawRoute(routes[0]);
    }
    route strat::stat_createRoute(int ballId)
    {
        Point whiteBall = balls[windex].getPoint();
        Point redBall = balls[ballId].getPoint();
        double bigDgr = 360;
        double bestDgr = 0;
        int hole = -1;
        for(int i = 0;i < 6;i++)
        {
            if(checkIfBlocked(ballId,i) == 0)
            {
                double dgr = calcDegrees(whiteBall,redBall,holes[i]);
                double value = abs(180-dgr);
                if(value < bigDgr)
                {
                    bigDgr = value;
                    bestDgr = dgr;
                    hole = i;
                }
            }
        }
        bestDgr = bestDgr / 180;
        route r(ballId,hole,bestDgr);
        return r;
    }
    double strat::stat_distScore(int ballId,int holeId)
    {
        double d = Distance(balls[ballId].getPoint(),holes[holeId]);
        double diag = sqrt((pow(clean.cols,2) + pow(clean.rows,2)));
        d = (1 - (d / diag));
        return d;
    }




