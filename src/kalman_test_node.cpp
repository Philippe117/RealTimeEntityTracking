#include "opencv2/highgui/highgui.hpp"
#include "PerceivedEntity.h"
#include <iostream>

#define drawCross( center, color, d ) \
line( img, Point( center.x - d, center.y - d ), Point( center.x + d, center.y + d ), color, 2, CV_AA, 0); \
line( img, Point( center.x + d, center.y - d ), Point( center.x - d, center.y + d ), color, 2, CV_AA, 0 )

using namespace cv;
using namespace std;

Point mousePos;

void mouseCallback(int event, int x, int y, int flags, void* userdata) {
    if ( event == EVENT_MOUSEMOVE ) {
        mousePos.x = x;
        mousePos.y = y;
    }
}

int main( )
{
    PerceivedEntity entity(100.f, 100.f, 0.f);
    PerceivedEntity mouse(100.f, 100.f, 0.f);


// Image to show mouse tracking
    RNG rng;
    Mat img(600, 800, CV_8UC3);
    vector<Point> kalmanv;
    kalmanv.clear();
    namedWindow("mouse kalman", 1);
    setMouseCallback("mouse kalman", mouseCallback, NULL);

    while(1)
    {

        entity.update(ros::Duration(0));

        // Get mouse point
        mouse.position.x = mousePos.x;//+rng.gaussian(40.f);
        mouse.position.y = mousePos.y;//+rng.gaussian(40.f);

        entity.mergeOnto(mouse);

        imshow("mouse kalman", img);
        img = Scalar::all(0);


        // plot points
        Point myEntity(entity.position.x, entity.position.y);
        drawCross( myEntity, Scalar(255,255,255), 5 );
        kalmanv.push_back(myEntity);

        Point measPt(mouse.position.x, mouse.position.y);
        drawCross( measPt, Scalar(0,0,255), 5 );


        for (int i = 0; i < kalmanv.size()-1; i++)
            line(img, kalmanv[i], kalmanv[i+1], Scalar(0,155,255), 1);

        waitKey(20);
    }

    return 0;
}
