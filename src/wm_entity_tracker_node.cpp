
#include "EntityTracker.h"
#include <iostream>
#include <vector>
#include <ros/ros.h>
#include "SimulatedInput.h"
#include "CvOutput.h"
#include "PeopleLegInput.h"
#include "BoundingBoxesInput.h"
#include <dynamic_reconfigure/server.h>
#include "wm_entity_tracker/wm_entity_trackerConfig.h"

#define drawCross( center, color, d ) \
line( img, Point( center.x - d, center.y - d ), Point( center.x + d, center.y + d ), color, 2, CV_AA, 0); \
line( img, Point( center.x + d, center.y - d ), Point( center.x - d, center.y + d ), color, 2, CV_AA, 0 )

namespace wm_entity_tracker {

    EntityTracker * tracker;
    EntityOutput * cvOutput;
    EntityOutput * rvizOutput;
    EntityOutput * topicOutput;

    EntityInput * boundingBoxesInput;
    EntityInput * simulatedinput;
    EntityInput * peopleLegInput;
}


using namespace cv;
using namespace std;
using namespace sara_msgs;
using namespace wm_entity_tracker;


void callback(wm_entity_tracker::wm_entity_trackerConfig &config, uint32_t level) {

    // Set the kalman parameters for the tracker
    PerceivedEntity::KalmanParams params;
    params.processNoiseCov =  config.bounding_boxes_input_processNoiseCov;
    params.measurementNoiseCov = config.bounding_boxes_input_measurementNoiseCov;
    params.errorCovPost = config.bounding_boxes_input_errorCovPost;
    boundingBoxesInput->setKalmanParams(params);

    PerceivedEntity::setXY(config.weights_XY);
    PerceivedEntity::setZ(config.weights_Z);

    cout << "reconfig\n" << boundingBoxesInput->kalmanParams().processNoiseCov << "\n"\
                         << boundingBoxesInput->kalmanParams().measurementNoiseCov << "\n"\
                         << boundingBoxesInput->kalmanParams().errorCovPost << "\n";

}

int main(int argc, char **argv) {

    ros::init(argc, argv, "wm_entity_tracker");
    ros::NodeHandle nh;


    // Prepare the pointers
    tracker = new EntityTracker;

    boundingBoxesInput = new BoundingBoxesInput(*tracker, nh, "/darknet_ros/bounding_boxes3D");
//    simulatedinput = new SimulatedInput(*tracker, 10);
//    peopleLegInput = new PeopleLegInput(*tracker, nh, "/people_tracker_measurements");

    cvOutput = new CvOutput(0, 0, 1, 1);
    tracker->addOutput(*cvOutput);


    // Configure the dynamic reconfigure thigny
    dynamic_reconfigure::Server<wm_entity_tracker::wm_entity_trackerConfig> server;
    dynamic_reconfigure::Server<wm_entity_tracker::wm_entity_trackerConfig>::CallbackType f;
    f = boost::bind(&callback, _1, _2);
    server.setCallback(f);


    ros::Rate rate(30);
    while(ros::ok())
    {

        // Update the tracker.
        tracker->update(ros::Duration(0));

        // Update ros
        ros::spinOnce();

        // Maintain framerate
        rate.sleep();
    }

    delete tracker;
    delete boundingBoxesInput;
    delete cvOutput;

    return 0;
}


