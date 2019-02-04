//
// Created by philippe on 08/11/18.
//

#ifndef PROJECT_PEOPLEFACEINPUT_H
#define PROJECT_PEOPLEFACEINPUT_H

#include "EntityInput.h"
#include "ros/ros.h"
#include "people_msgs/PositionMeasurementArray.h"
#include <vector>
#include "sara_msgs/Entity.h"

class PeopleFaceInput : public EntityInput{
    ros::NodeHandle mNodeHandle;
    ros::Subscriber mPeopleSub;
    void peopleLegCallback(people_msgs::PositionMeasurementArray legArray);
    ros::AsyncSpinner mSpinner;
public:
    PeopleFaceInput(EntityTracker &tracker, ros::NodeHandle &nh, std::string topic);

    ~PeopleFaceInput();
};


#endif //PROJECT_PEOPLEFACEINPUT_H
