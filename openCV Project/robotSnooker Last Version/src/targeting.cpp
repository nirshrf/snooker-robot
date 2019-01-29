#include "targeting.h"

targeting::targeting(RS232::SerialConnection serConnx,VideoCapture cap)
{
    serConn = serConnx;
    vector<Point> points;
    if(option == 0)
    {

        cap.set(CV_CAP_PROP_FRAME_WIDTH,1600);
        cap.set(CV_CAP_PROP_FRAME_HEIGHT,900);
        cap.set(CV_CAP_PROP_BRIGHTNESS,87);
        cap.set(CV_CAP_PROP_CONTRAST,126);
        cap.set(CV_CAP_PROP_SATURATION,178);
        cap.set(CV_CAP_PROP_WHITE_BALANCE_BLUE_U,4042);
/*
        cout << cap.get(CV_CAP_PROP_BRIGHTNESS) << endl;
        cout << cap.get(CV_CAP_PROP_WHITE_BALANCE_BLUE_U) << endl;
        cout << cap.get(CV_CAP_PROP_CONTRAST) << endl;
        cout << cap.get(CV_CAP_PROP_SATURATION) << endl;
        system("pause");
        */
        cap.read(frame);
        //frame = st.lightReduce(frame);
        //frame = st.equalizeMat(frame);
        imshow("fr",frame);
        points = st.getPositions(cap);
        serConn.SendByte(port,0u);
        Sleep(100);
        serConn.SendByte(port,11u);

    }
    else if(option == 1)
    {
        frame = imread("pic.png");
        points = st.getPositions(frame); // edit start
    }
    vector<Point> edges = st.edges;
    cout << "Edges: " << edges << endl;
    Point wb = Point(min(edges[0].x,edges[1].x) + points[0].x,min(edges[0].y,edges[1].y) + points[0].y);
    Point sb(min(edges[0].x,edges[1].x) + points[1].x,min(edges[0].y,edges[1].y) + points[1].y);
    int balls = st.getBallsSize();
    cout << "Balls found" <<balls << endl;
    if(balls > 0)
        accurateShot(cap,wb,sb);
}
Point targeting::GetWhileBall()
{
    return wb;
}
void targeting::accurateShot(VideoCapture cap,Point wb,Point sb)
{
    borderPoints = st.loadBorderPoints(frame);
    states CurrentState = looking,nextState = looking;
    bool aiming = true;
    while(aiming)
    {
        system("cls");
        if(option == 0)
            cap.read(frame);
        else if(option == 1)
            frame = imread("pic.png",CV_LOAD_IMAGE_COLOR);
        else
        {
            cout<<"invalid option"<<endl;
            break;
        }
        //
        Mat org = frame.clone();
        robotPos = findRobotPos(wb,sb);
        drawLine(wb,sb);
        drawShapes(wb,sb);
        if(robotPos == Point(-1,-1))
        {
            cout << "could'nt find the right position" << endl;
        }
        else
        {
            double robotSlope = st.slope(robotPos,sb);
            double ballsSlope = st.slope(wb,sb);
            double diff = abs(robotSlope - ballsSlope);
            double dgree = st.calcDegrees(wb,sb,robotPos);
            double degree = calcDegrees(wb,sb,robotPos);
            Point handPoint = findHandCenter(org);
            CurrentState = nextState;
            if(CurrentState == rotating || CurrentState == fbMoving)
                drawHandShapes(handPoint,sb);
            switch(CurrentState)
            {
            case looking:
                cout << "Waiting for the robot to get near the line..." << endl;
                if(dgree < 3)
                    nextState = stopping;
                break;
            case stopping:
                serConn.SendByte(port,b_stop);
                Sleep(100);
                nextState = rlMoving;
                break;
            case rlMoving:
                nextState = states_rlMoving(degree);
                break;
            case rotating:
                nextState = states_rotating(degree,handPoint,sb);
                break;
            case fbMoving:
                nextState = states_fbMoving(handPoint,wb);
                break;
            case target:
                states_target();
                aiming = false;
                break;
            }
        }
        imshow("frame",frame);
        if(waitKey(30) >= 0) break;
    }
}
void targeting::states_target()
{
    serConn.SendByte(port,b_stop);
    Sleep(1000);
    serConn.SendByte(port,ready);
    cout << "Ready to Shoot!" << endl;
}
states targeting::states_fbMoving(Point handPoint,Point wb)
{
    static int delays = 0;
    double dists = st.Distance(wb,handPoint);
    cout << "Distance : " << dists << endl;
    if(dists > 35)
        {
            serConn.SendByte(port,b_up);
            Sleep(50);
            serConn.SendByte(port,b_stop);
        }
    else if(dists < 30)
    {
        serConn.SendByte(port,b_down);
        Sleep(50);
        serConn.SendByte(port,b_stop);
    }
    else
        return target;
    return fbMoving;
}
states targeting::states_rotating(double dgree,Point handPoint,Point sb)
{
    static int delays = 0;
    if(dgree >= range || dgree <= -range)
    {
        cout << "DONE" << endl;
        return rlMoving;
    }
    else
    {
        static int ctr = 0;
        static int delays = 0;
        range = 0.5;
        cout << "Rotating " << endl;
        double handDgr = calcDegrees(sb,robotPos,handPoint);
        cout << "Hand Degree : " <<handDgr << endl;
        //cout << handDiff << endl;
        int tdelay = 5;
        if(handDgr > range)
        {
            cout << "ROTATE CLOCKWISE" << endl;
            serConn.SendByte(port,b_clockWise);
            Sleep(25);
            serConn.SendByte(port,b_stop);
            ctr = 0;
        }
        else if(handDgr < -range)
        {
            cout << "ROTATE COUNTER-CLOCKWISE" << endl;
            serConn.SendByte(port,b_cClockWise);
            Sleep(25);
            serConn.SendByte(port,b_stop);
            ctr = 0;

        }
        else
        {
            ctr++;
            if(ctr > 8)
            {
                return fbMoving;
            }
        }
    }
    return rotating;
}
states targeting::states_rlMoving(double degree)
{
    static int ctr = 0;
    static int delays = 0;
    int tdelay = 5;
    if(degree > range)
    {
        serConn.SendByte(port,b_left);
        Sleep(40);
        serConn.SendByte(port,b_stop);
        cout << "GO LEFT" << endl;
    }
    else if(degree < -range)
    {
        serConn.SendByte(port,b_right);
        Sleep(40);
        serConn.SendByte(port,b_stop);
        cout << "GO RIGHT" << endl;
    }
    else
    {
        ctr++;
        serConn.SendByte(port,b_stop);
        if(ctr > 20)
        {
            return rotating;
            ctr = 0;
        }
    }
    return rlMoving;
}
double targeting::calcDegrees(Point wb,Point tb,Point hole) // wb = whiteball, tb = target ball
{
    double angle = atan2(wb.y-tb.y, wb.x-tb.x) - atan2(hole.y-tb.y, hole.x-tb.x);
    angle = angle * 360 / (2*PI);
    return angle;
}
Point targeting::findHandCenter(Mat img)
{
    cvtColor(img,img,CV_BGR2HSV);
    GaussianBlur(img,img, Size(9, 9), 2, 2);
    inRange(img,Scalar(0,150,150),Scalar(70,255,220),img);
    erode(img, img, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    dilate( img, img, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    dilate( img, img, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    erode(img, img, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    imshow("img",img);

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    threshold(img,img, 200, 255, THRESH_BINARY );
    findContours(img, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    vector<vector<Point> > contours_poly( contours.size() );
    vector<Rect> boundRect( contours.size() );
    vector<Point2f>center( contours.size() );
    vector<float>radius( contours.size() );
    for( int i = 0; i < contours.size(); i++ )
    { approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
        boundRect[i] = boundingRect( Mat(contours_poly[i]) );
        minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
    }
    for(int i = 0; i < contours.size(); i++ )
    {
        if(contourArea(contours[i]) > 500)
        {
            return center[i];
        }
    }
}
void targeting::drawShapes(Point p1,Point p2)
{
    circle(frame,p1,5,Scalar(255,0,0),-1);
    circle(frame,p2,5,Scalar(255,0,0),-1);
    line(frame,p1,p2,Scalar(255,0,0),2,LINE_AA);
}
void targeting::drawHandShapes(Point handPoint,Point sb)
{
    circle(frame,handPoint,20,Scalar(0,0,0),2);
    circle(frame,handPoint,3,Scalar(0,0,255),-1);
    line(frame,handPoint,robotPos,Scalar(0,0,255),2);
    line(frame,sb,robotPos,Scalar(0,0,255),2);
}
struct targeting::sortingBorderAlg
    {
        bool operator()(borderRoute r1, borderRoute r2) const
        {
           return (r1.getRating() < r2.getRating());
        }
    };
bool targeting::drawLine(Point wb,Point sb)
{
    vector<borderRoute> borders;
    double mainSlope = st.slope(wb,sb);
    for(int i=0;i<borderPoints.size();i++)
    {
        Point p = borderPoints[i];
        double localSlope = st.slope(p,wb);
        double cal = abs(mainSlope - localSlope);
        if(cal > -0.1 && cal < 0.1)
        {
            if(wb.x > sb.x)
            {
                if(p.x > wb.x)
                {
                    borderRoute br(p,cal);
                    borders.push_back(br);
                }
            }
            else
            {
                if(p.x < wb.x)
                {
                    borderRoute br(p,cal);
                    borders.push_back(br);
                }
            }
        }
    }
    sort(borders.begin(),borders.end(),sortingBorderAlg());
    if(borders.size() > 0)
        line(frame,borders[0].getBorderPoint(),wb,Scalar(0,0,0),2,LINE_AA);
}
Point targeting::findRobotPos(Point wb,Point sb)
{
    Mat blue = frame.clone();
    cvtColor(blue,blue,CV_BGR2HSV);
    erode(blue, blue, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)) );
    dilate( blue, blue, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)) );
    dilate( blue, blue, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)) );
    erode(blue, blue, getStructuringElement(MORPH_ELLIPSE, Size(10, 10)) );
    GaussianBlur(blue,blue, Size(9, 9), 2, 2);
    inRange(blue,Scalar(100,196,96),Scalar(110,255,217),blue);
    imshow("b",blue);

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    threshold(blue,blue, 200, 255, THRESH_BINARY );
    findContours( blue, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    vector<vector<Point> > contours_poly( contours.size() );
    vector<Rect> boundRect( contours.size() );
    vector<Point2f>center( contours.size() );
    vector<float>radius( contours.size() );
    for( int i = 0; i < contours.size(); i++ )
    { approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
        boundRect[i] = boundingRect( Mat(contours_poly[i]) );
        minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
    }
    Mat blackMat = Mat::zeros(frame.size(),CV_8UC1);
    for( int i = 0; i< contours.size(); i++ )
    {
        double d = contourArea(contours[i]);
        if(d > 500 && d<2500)
        {
            Scalar color = Scalar(0,0,0);
            circle( frame, center[i], (int)radius[i] , color,-1, 8);
            circle( frame, center[i], (int)radius[i]-10 , Scalar(255,0,0),-1, 8);
            circle( blackMat, center[i], (int)radius[i]-10 , Scalar(255,255,255),-1, 8);
            line(frame,center[i],sb,Scalar(0,0,0),3,LINE_AA);
            return center[i];
        }
    }
    return(Point(-1,-1));
}
Point targeting::findY(double x1,double y1,double m,double inX)
{
    double y = m*(inX - x1) + y1;
    Point newp(inX,y);
    return newp;
}
Point targeting::findX(double x1,double y1,double m,double inY)
{
    double x = ((y1 - (m * x1) - inY) / m);
    Point newp (x,inY);
    return newp;
}
double targeting::dgr(Point start,Point p1,Point p2)
{
    double x = abs(start.x - p1.x) * abs(start.x - p2.x);
    double y = abs(start.y - p1.y) * abs(start.y - p2.y);
    double A = st.Distance(p1,start);
    double B = st.Distance(p2,start);
    double alpha = (x+y) / (A*B);
    //cout << alpha << endl;
    double ret = acos(alpha) * 180.0 / PI;
    return ret;
}
int targeting::difference(int x, int y)
{
    if (x>y)
    {
        return (x-y);
    }
    else
    {
       return (y-x);
    }
}
targeting::~targeting()
{
    //dtor
}
