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
using namespace sara_msgs;

//Point mousePos;

//void mouseCallback(int event, int x, int y, int flags, void* userdata) {
//    if ( event == EVENT_MOUSEMOVE ) {
//        mousePos.x = x;
//        mousePos.y = y;
//    }
//}

int main( )
{
    ros::Time::init();

    EntityTracker tracker;
    vector<Entity> input;

    Entity en;


    // Image to show mouse tracking
    RNG rng;
    Mat img(600, 800, CV_8UC3);
    namedWindow("mouse kalman", 1);
//    setMouseCallback("mouse kalman", mouseCallback, NULL);

    int nb{10};
    while(1)
    {
        if (--nb == 0){
            input.push_back(en);
            nb = 400;
        }



        // Get mouse point.
        for (auto &in : input){
            in.position.x += rng.gaussian(6.f);
            in.position.y += rng.gaussian(6.f);
            in.position.x += (400-in.position.x)/80;
            in.position.y += (300-in.position.y)/80;
            for (auto & in2 : input){
                float dist{1.f+float(hypot(in.position.x-in2.position.x, in.position.y-in2.position.y))};
                in.position.x -= (in2.position.x-in.position.x)/dist/dist*150;
                in.position.y -= (in2.position.y-in.position.y)/dist/dist*150;
            }
        }


        // Update the tracker.
        tracker.update(ros::Duration(0));


        // perceive the mouse
        tracker.perceiveEntities(input);


        imshow("mouse kalman", img);
        img = Scalar::all(0);


        // Draw the result.
        tracker.opencvDraw(img);




        for (auto &in : input){
            Point measPt(in.position.x, in.position.y);
            drawCross( measPt, Scalar(0,0,255), 5 );
        }


        waitKey(20);
    }

    return 0;
}
