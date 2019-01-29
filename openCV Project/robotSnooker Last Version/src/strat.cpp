#include "strat.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/opencv.hpp"
#include <windows.h>
#include <math.h>
#include <sstream>
#include <algorithm>
#include <math.h>
# define PI           3.14159265358979323846
#include "borderRoute.h"

using namespace std;
using namespace cv;

    strat::strat()
    {
        namedWindow("frames",1);
        whiteBallFound = false;
        holesFound = false;
    }
    vector<Point> strat::getPositions(VideoCapture cap)
    {
        printf("DEBUG: %s\n", "getPositions");
        vector<Point> pts;
        while(whiteBallFound == false || holesFound == false)
        {
            static int counter = 0;
            cap.read(frame);
            //frame = equalizeMat(frame);
            frameClone = frame.clone();
            frame = detectTable(frame);

            clean = frame.clone();
            if(counter > 10)
            {
                try
                {

                    detectRed(clean);

                    imshow("after", frame);
                    detectHoles(frame);
                    imshow("before", frame);
                    detectWhite(frame);
                    detectWhiteBall();

                    if(whiteBallFound)
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
      //  cout << "White Ball : " << balls[windex].getPoint() << endl;
       // cout << "Simulated Point : " << simBall << endl;
        pts.push_back(balls[windex].getPoint());
        pts.push_back(simBall);

        return pts;
    }
    vector<Point> strat::getPositions(Mat img)
    {
        vector<Point> pts;
        whiteBallFound = false;
        holesFound = false;
        namedWindow("frames",1);
        while(whiteBallFound == false || holesFound == false)
        {
            static int counter = 0;
            frame = img.clone();
            frameClone = frame.clone();
            frame = detectTable(frame);
            clean = frame.clone();
            try
            {

                detectRed(frame);
                detectHoles(frame);
                detectWhite(frame);
                detectWhiteBall();
                bestRoute();
            }
            catch (cv::Exception)
            {
                cout << "exception" << endl;
            }
            imshow("frames", clean);
            balls.clear();
            counter++;
            if(waitKey(10) >= 0) break;

        }
        pts.push_back(balls[windex].getPoint());
        pts.push_back(simBall);
        return pts;
    }
    Mat strat::equalizeMat(Mat inputMat)
    {
        Mat temp;
        vector<Mat> colors;
        split(inputMat,colors);
        for(int i=0;i<colors.size();i++)
        {
            equalizeHist(colors[i],colors[i]);
        }
        merge(colors,temp);
        imshow("equlize", temp);
        return temp;
    }
    void strat::detectRed(Mat e)
    {
        Mat hsv = e.clone();
        Mat part1,part2,redScreen;
        cvtColor(hsv,hsv,CV_BGR2HSV);
        printf("DEBUG: %s\n", "cvtColor");
        inRange(hsv, Scalar(0, 100, 100), Scalar(10, 255, 255), part1);
        inRange(hsv, Scalar(160, 100, 100), Scalar(179, 255, 255), part2);
        addWeighted(part1, 1.0, part2, 1.0, 0.0, redScreen);
        GaussianBlur(redScreen,redScreen, Size(9, 9), 2, 2);
        ballHandler(redScreen,"red");
        imshow("red",redScreen);
    }
    void strat::detectWhite(Mat e)
    {
        Mat img;

        cvtColor(e, img,COLOR_BGR2HSV);
        inRange(img, Scalar(0, 190, 0), Scalar(180,255,254), img);//Scalar(0,230,200),Scalar(70,255,220)
        erode(img, img, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        dilate( img, img, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        dilate( img, img, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        erode(img, img, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        //bitwise_not(img,img);
        GaussianBlur(img, img, Size(9, 9), 2, 2);
        imshow("White", img);
        ballHandler(img,"white");
    }
    void strat::detectHoles(Mat _img)
    {
        Mat img = _img.clone();
        cvtColor(img, img,COLOR_BGR2HSV);
        inRange(img, Scalar(0,0,61), Scalar(179,255,255), img);
        bitwise_not(img,img);
        GaussianBlur(img, img, Size(9, 9), 2, 2);
        imshow("holes",img);
        vector<Vec3f> circles;
        HoughCircles(img, circles, CV_HOUGH_GRADIENT, 1, img.rows/8, 100, 20, 0, 0);
        if(circles.size() == 6)
            holesFound = true;
        else
        {
            cout << "couldnt find 6 holes!" << endl;
            holesFound = false;
        }
        for(int current_circle = 0; current_circle < circles.size(); current_circle++)
        {
                Point center = Point(round(circles[current_circle][0]), round(circles[current_circle][1]));
                int radius = round(circles[current_circle][2]);
                circle(clean, center, radius, Scalar(255, 255, 255), 1);
                circle(clean, center, 1, Scalar(0, 255, 0), 3);
                if(holesFound)
                    holes[current_circle] = center;
        }

    }
    void strat::detectWhiteBall()
    {
        int isize = balls.size();
        windex = -1;
        for(int i = 0; i < isize; i++)
        {
           if(balls[i].getName() == "white1")
            windex = i;
        }
        if(windex >= 0)
        {
            if(balls[windex].getPoint() != Point(0,0))
            {
                if(balls[windex].getRadius() >= 0)
                    whiteBallFound = true;
                else
                    whiteBallFound = false;
            }
            else
                whiteBallFound = false;

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
        HoughCircles(img, circles, CV_HOUGH_GRADIENT, 1, 10, 100,20,8,20);
        Point center;
        for(size_t current_circle = 0; current_circle < circles.size(); current_circle++)
        {
            center = Point(round(circles[current_circle][0]), round(circles[current_circle][1]));
            int radius = round(circles[current_circle][2]);
            if(color != "white" || (color == "white" && circles.size() == 1))
            {
                circle(clean, center, radius, Scalar(0, 0, 0), 1);
                circle(clean, center, 1, Scalar(0, 0, 0), 3);
                ball b(center,color + intToString(current_circle+1),radius);
                balls.push_back(b);
                putText(clean,color + intToString(current_circle+1),Point(center.x - 14,center.y - 8),1,0.5,Scalar(0, 0, 0),1);
                if(color != "white")
                    ballCount++;
            }
            else if(color == "white" && circles.size() > 1)
                cout << circles.size() << " was found!" << endl;
        }
    }
    double strat::calcDegrees(Point wb,Point tb,Point hole) // wb = whiteball, tb = target ball
    {
        double a = Distance(wb,tb);
        double b = Distance(tb,hole);
        double c = Distance(wb,hole);
        double cosinAlpha = ((a * a ) + (b * b) - (c * c)) / (2 * a * b);
        double dgr = acos(cosinAlpha) * 180.0 / PI;
       // cout<< cosinAlpha << endl;
        return dgr;
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

    bool strat::checkIfBlocked(int ballId,int holeId) // p1 & p2 = white ball & hole
    {
        int errors = 0;
        Point tempBall = simulateKick(ballId,holeId);
        errors += checkIfCenterOnArea(holeId,tempBall,ballId);
        errors += checkIfBallOnArea(holeId,tempBall,ballId);
        if(errors == 0)
            return false;
        else
            return true;
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
                Vec3b color = image.at<uchar>(balls[i].getPoint());
                if(color.val[0]==255 && color.val[1]==255 && color.val[2]==255)
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
        // get black and white image
        for(int i = 0;i<2;i++)
        {
            Point *p = findRectPoints(pts[i],pts[i+1]);
            for(int j = 0;j<4;j++)
            {
                blocksPoints[i][j] = *(p + j);
            }
        }
        Mat image = Mat::zeros(clean.size(),CV_8UC1);
        //find first area
        for(int i = 0;i<2;i++)
        {
            const Point* ppt[1] = { blocksPoints[i] };
            int npt[] = { 4 };
            fillPoly( image, ppt, npt, 1, Scalar( 255, 255, 255 ), 8 );
        }
        circle(image,tempBall,balls[windex].getRadius(),Scalar(255,255,255),-1);
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
         //find second area
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
    void strat::drawRoute(route r,double multi,int winner)
    {
        Scalar routeColor;
        int ballId = r.getBallId(),holeId = r.getHoleId();
        Point ball = balls[ballId].getPoint();
        Point newPoint = simulateKick(ballId,holeId);
        if(winner == 1)
        {
            simBall = newPoint;
            routeColor = Scalar(0,255,0);
        }
        else
            routeColor = Scalar(255,255,255);
        Point pts[] = {balls[windex].getPoint(),newPoint,holes[holeId]};
        Point blocksPoints[2][4];
        circle(clean,newPoint,balls[windex].getRadius(),routeColor,-1);
        newPoint = Point(newPoint.x - 10,newPoint.y + 10);
        putText(clean,intToString(r.getBallId()),newPoint,1,2,Scalar(0, 0, 0),1);
       // line(clean,ball,holes[holeId],Scalar(255,255,255));
       // line(clean,newPoint,holes[holeId],Scalar(0,0,255));
       // line(clean,newPoint,balls[windex].getPoint(),Scalar(0,0,255));
        for(int i = 0;i<2;i++)
        {
            Point *p = findRectPoints(pts[i],pts[i+1]);
            for(int j = 0;j<4;j++)
            {
                blocksPoints[i][j] = *(p + j);
            }
            line(clean,blocksPoints[i][0],blocksPoints[i][3],routeColor,1.5);
            line(clean,blocksPoints[i][1],blocksPoints[i][2],routeColor,1.5);
            line(clean,blocksPoints[i][0],blocksPoints[i][1],routeColor,1.5);
            line(clean,blocksPoints[i][2],blocksPoints[i][3],routeColor,1.5);
        }
    }
    Mat strat::detectTable(Mat e)
    {
        Mat img = e.clone();
        cvtColor(img, img,COLOR_BGR2HSV);
        inRange(img, Scalar(70, 220, 0), Scalar(88, 255, 255), img);
        erode(img, img, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        dilate( img, img, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        dilate( img, img, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        erode(img, img, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
        GaussianBlur(img, img, Size(9, 9), 2, 2);
        Mat retMat = findRect(img);
        imshow("s",retMat);
        return retMat;
    }
    Mat strat::findRect(Mat img)
    {
        edges.clear();
        Mat retMat = Mat::zeros(img.size(),CV_8UC1);;
        vector<Vec3f> circles;
        HoughCircles(img, circles, CV_HOUGH_GRADIENT, 1, img.rows/8, 100, 20, 20,60);
        Point center;
        for(size_t current_circle = 0; current_circle < circles.size(); ++current_circle)
        {
            center = Point(round(circles[current_circle][0]), round(circles[current_circle][1]));
            int radius = round(circles[current_circle][2]);
            circle(img,center,radius,Scalar(255,255,255),3);
            if (circles.size() == 2)
            {
                edges.push_back(center);
            }
        }
        if(edges.size() == 2)
        {
            Rect r(min(edges[0].x,edges[1].x),
                   min(edges[0].y,edges[1].y),abs(edges[0].x - edges[1].x),abs(edges[0].y - edges[1].y));
            retMat = frameClone(r);
        }
        return retMat;
    }
    struct strat::sortingAlg
    {
        bool operator()(route r1, route r2) const
        {
           return (r1.getRating() > r2.getRating());
        }
    };
    struct strat::sortingBorderAlg
    {
        bool operator()(borderRoute r1, borderRoute r2) const
        {
           return (r1.getRating() < r2.getRating());
        }
    };
    void strat::bestRoute()
    {
        vector<route> routes;
        int bsize = balls.size();
        borderPoints = loadBorderPoints(frame);
        for(int i = 0;i<bsize;i++)
        {
            if(i != windex)
            {
                for(int j = 0;j<6;j++)
                {
                    route r = stat_createRoute(i,j,false);
                    if(r.getBallId() != -1)
                    {
                        double ret = stat_distScore(i,r.getHoleId());
                        ret += stat_whiteRedDist(i,r.getHoleId());
                        ret += stat_relatedDegree(i,r.getHoleId())*2;
                        ret += stat_whiteDist(i,r.getHoleId())*2;
                        r.addToRating(ret);
                        routes.push_back(r);
                    }
                }
            }
        }
        sort(routes.begin(),routes.end(),sortingAlg());
        int rsize = routes.size();
        cout << "Routes size : "<<rsize << endl;
        if(rsize == 0)
        {
            int randomBall = windex;
            while(randomBall == windex)
                randomBall = rand() % balls.size();
            routes.push_back(stat_createRoute(randomBall,rand() % 6,true));
            rsize = routes.size();
        }
        for(int i = 0;i<rsize;i++)
        {
            double def = (double(rsize - i-1)/double(rsize-1));
            cout << "Route [" << routes[i].getBallId() << "] " << routes[i].getRating() << " - "  << i<< endl;
            drawRoute(routes[i],def,(rsize-i)/rsize);
        }
    }
    route strat::stat_createRoute(int ballId,int holeId,bool failureRoute)
    {
        Point whiteBall = balls[windex].getPoint();
        Point redBall = balls[ballId].getPoint();
        double bigDgr = 360;
        double dgr=0;
        int hole = -1;
        route fail(-1,-1,0);
        if(failureRoute)
            return route(ballId,holeId,dgr);
        if(checkIfBlocked(ballId,holeId) == false)
        {
            dgr = calcDegrees(whiteBall,redBall,holes[holeId]);
            if(dgr > 120)
            {

                dgr = dgr / 180;
                route r(ballId,holeId,dgr);
                return r;
            }
        }
        return fail;
    }
    double strat::stat_distScore(int ballId,int holeId)
    {
        double d = Distance(balls[ballId].getPoint(),holes[holeId]);
        double siz;
            if(frame.cols > frame.rows)
                siz = frame.rows/3;
            else
                siz = frame.cols/3;
        d = (1 - (d / siz));
        return d;
    }
    double strat::stat_whiteRedDist(int ballId,int holeId)
    {
        double d = Distance(balls[ballId].getPoint(),balls[windex].getPoint());
        double diag = sqrt((pow(clean.cols,2) + pow(clean.rows,2)));
        d = (1 - (d / diag));
        return d;
    }

    double strat::stat_whiteDist(int ballId,int holeId)
    {
        double d = 0;
        Point whiteBall = balls[windex].getPoint();
        simBall = simulateKick(ballId,holeId);
        double mainSlope = slope(whiteBall,simBall);
        vector<borderRoute> borders;
        for(int i=0;i<borderPoints.size();i++)
        {
            Point p = borderPoints[i];
            double localSlope = slope(p,whiteBall);
            double cal = abs(mainSlope - localSlope);
            if(cal < 1)
            {
                if(whiteBall.x > simBall.x)
                {
                    if(p.x > whiteBall.x)
                    {
                        borderRoute br(p,cal);
                        borders.push_back(br);
                    }
                }
                else
                {
                    if(p.x < whiteBall.x)
                    {
                        borderRoute br(p,cal);
                        borders.push_back(br);
                    }
                }
            }
        }
        sort(borders.begin(),borders.end(),sortingBorderAlg());
        if(borders.size() > 0)
        {
            double siz;
            if(frame.cols > frame.rows)
                siz = frame.rows/4;
            else
                siz = frame.cols/4;
            arrowedLine(clean,whiteBall,borders[0].getBorderPoint(),Scalar(255,0,0),2);
            double dist = Distance(borders[0].getBorderPoint(),whiteBall);
            dist = (1 - (dist / siz));
            d = dist;
        }
        cout << ballId << " - " << d << endl;
        return d;
    }

    double strat::stat_relatedDegree(int ballId,int holeId)
    {
        Point whiteBall = balls[windex].getPoint();
        simBall = simulateKick(ballId,holeId);
        double slp = slope(whiteBall,simBall);
        double dgr = atan(slp) * 180.0 / PI;
        double relative = abs(45-abs(dgr)) / 45;
        //cout << slp << endl;
        imshow("frames",frame);
        cout << "DGR : "  << dgr << endl;
        cout << "RLTV : " << relative << endl;
        return relative;
    }
    vector<Point> strat::loadBorderPoints(Mat fr)
    {
        vector<Point> brd;
        int w = fr.cols,h=fr.rows;
        for(int i = 0;i<h;i++)
        {
            brd.push_back(Point(0,i));
            brd.push_back(Point(w,i));
        }
        for(int i = 0;i<w;i++)
        {
            brd.push_back(Point(i,0));
            brd.push_back(Point(i,h));
        }
        return brd;
    }
    Point strat::findY(double x1,double y1,double m,double inX)
    {
        double y = m*(inX - x1) + y1;
        Point newp(inX,y);
        return newp;
    }
    Point strat::findX(double x1,double y1,double m,double inY)
    {
        double x = ((y1 - (m * x1) - inY) / m);
        Point newp (x,inY);
        return newp;
    }
    int strat::getBallsSize()
    {
        return ballCount;
    }
    strat::~strat()
    {
        //dtor
    }
