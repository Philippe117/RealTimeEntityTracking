#include "opencv2/highgui/highgui.hpp"
#include "PerceivedEntity.h"
#include "EntityTracker.h"
#include <iostream>
#include <vector>

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
    EntityTracker tracker;

    PerceivedEntity mouse(0.f, 0.f, 0.f);


    // Image to show mouse tracking
    RNG rng;
    Mat img(600, 800, CV_8UC3);
    namedWindow("mouse kalman", 1);
    setMouseCallback("mouse kalman", mouseCallback, NULL);

    while(1)
    {

        // Get mouse point.
        mouse.position.x = mousePos.x+rng.gaussian(0.f);
        mouse.position.y = mousePos.y+rng.gaussian(0.f);

        // Update the tracker.
        tracker.update(ros::Duration(0));


        // perceive the mouse
        tracker.perceiveEntity(mouse);


        imshow("mouse kalman", img);
        img = Scalar::all(0);


        // Draw the result.
        tracker.opencvDraw(img);


        Point measPt(mouse.position.x, mouse.position.y);
        drawCross( measPt, Scalar(0,0,255), 5 );


        waitKey(20);
    }

    return 0;
}
