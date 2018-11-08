
#include "EntityTracker.h"
#include <iostream>
#include <vector>
#include <ros/ros.h>
#include "SimulatedInput.h"
#include "CvOutput.h"

#define drawCross( center, color, d ) \
line( img, Point( center.x - d, center.y - d ), Point( center.x + d, center.y + d ), color, 2, CV_AA, 0); \
line( img, Point( center.x + d, center.y - d ), Point( center.x - d, center.y + d ), color, 2, CV_AA, 0 )


using namespace cv;
using namespace std;
using namespace sara_msgs;


int main(int argc, char **argv) {

    ros::init(argc, argv, "wm_entity_tracker");
    ros::NodeHandle nh;

    CvOutput output;
    EntityTracker tracker;
    tracker.addOutput(output);
    SimulatedInput sim(tracker);



    ros::Rate rate(30);
    while(ros::ok())
    {

        // Update the tracker.
        tracker.update(ros::Duration(0));

        rate.sleep();
    }

    return 0;
}
