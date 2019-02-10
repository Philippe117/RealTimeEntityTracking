//
// Created by philippe on 08/11/18.
//

#ifndef PROJECT_PEOPLEFACEINPUT_H
#define PROJECT_PEOPLEFACEINPUT_H

#include "EntityInput.h"
#include "ros/ros.h"
#include <vector>
#include "sara_msgs/Entity.h"
#include "sara_msgs/Faces.h"

class PeopleFaceInput : public EntityInput{
    ros::NodeHandle mNodeHandle;
    ros::Subscriber mFacesSub;
    void peopleFaceCallback(sara_msgs::Faces faceArray);
    ros::AsyncSpinner mSpinner;
public:
    PeopleFaceInput(EntityTracker &tracker, ros::NodeHandle &nh, std::string topic);

    ~PeopleFaceInput();
};


#endif //PROJECT_PEOPLEFACEINPUT_H
