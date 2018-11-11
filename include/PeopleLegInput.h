//
// Created by philippe on 08/11/18.
//

#ifndef PROJECT_PEOPLELEGINPUT_H
#define PROJECT_PEOPLELEGINPUT_H

#include "EntityInput.h"
#include "ros/ros.h"
#include "people_msgs/PositionMeasurementArray.h"
#include <vector>
#include "sara_msgs/Entity.h"

class PeopleLegInput : public EntityInput{
    ros::NodeHandle nodeHandle;
    ros::Subscriber peopleSub;
    void peopleLegCallback(people_msgs::PositionMeasurementArray legArray);
    ros::AsyncSpinner spinner;
public:
    PeopleLegInput(EntityTracker &tracker, ros::NodeHandle &nh, const std::string topic);

    ~PeopleLegInput();
};


#endif //PROJECT_PEOPLELEGINPUT_H
