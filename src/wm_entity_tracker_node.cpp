#include "opencv2/highgui/highgui.hpp"
#include "PerceivedEntity.h"
#include "EntityTracker.h"
#include <iostream>
#include <vector>
#include <ros/ros.h>
#include "SimulatedInput.h"

#define drawCross( center, color, d ) \
line( img, Point( center.x - d, center.y - d ), Point( center.x + d, center.y + d ), color, 2, CV_AA, 0); \
line( img, Point( center.x + d, center.y - d ), Point( center.x - d, center.y + d ), color, 2, CV_AA, 0 )


using namespace cv;
using namespace std;
using namespace sara_msgs;


int main(int argc, char **argv) {

    ros::init(argc, argv, "wm_entity_tracker");
    ros::NodeHandle nh;


    EntityTracker tracker;

    SimulatedInput sim(tracker);
    // Image to show mouse tracking

    Mat img(600, 800, CV_8UC3);
    namedWindow("mouse kalman", 1);
//    setMouseCallback("mouse kalman", mouseCallback, NULL);

    while(ros::ok())
    {

        // Update the tracker.
        tracker.update(ros::Duration(0));


        imshow("mouse kalman", img);
        img = Scalar::all(0);


        // Draw the result.
        tracker.opencvDraw(img);


//        putText(img, "detections = " + to_string(input.size()), Point(20, 40), FONT_HERSHEY_COMPLEX, 1, 255);

        waitKey(10);
    }

    return 0;
}
