//
// Created by philippe on 11/11/18.
//

#ifndef PROJECT_BOUNDINGBOXESINPUT_H
#define PROJECT_BOUNDINGBOXESINPUT_H

#include "EntityInput.h"
#include "ros/ros.h"
#include <vector>
#include "sara_msgs/BoundingBoxes3D.h"
#include <string>

class BoundingBoxesInput : public EntityInput{
    ros::NodeHandle mNodeHandle;
    ros::Subscriber mBoundingBoxesSub;
    void BoundingBoxesCallback(sara_msgs::BoundingBoxes3D BBs);
    ros::AsyncSpinner mSpinner;
public:
    BoundingBoxesInput(EntityTracker &tracker, ros::NodeHandle &nh, std::string topic);

    ~BoundingBoxesInput();
};


#endif //PROJECT_BOUNDINGBOXESINPUT_H
