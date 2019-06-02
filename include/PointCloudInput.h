//
// Created by philippe on 11/11/18.
//

#ifndef PROJECT_POINTCLOUDINPUT_H
#define PROJECT_POINTCLOUDINPUT_H

#include "EntityInput.h"
#include "ros/ros.h"
#include <vector>
#include "sara_msgs/PointClouds.h"
#include <string>

class PointCloudInput : public EntityInput{
    ros::NodeHandle mNodeHandle;
    ros::Subscriber mPointCloudSub;
    void PointCloudCallback(sara_msgs::PointClouds PCs);
    ros::AsyncSpinner mSpinner;
public:
    PointCloudInput(EntityTracker &tracker, ros::NodeHandle &nh, std::string topic);

    ~PointCloudInput();
};


#endif //PROJECT_POINTCLOUDINPUT_H
