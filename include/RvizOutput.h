//
// Created by philippe on 08/11/18.
//

#ifndef PROJECT_RVIZOUTPUT_H
#define PROJECT_RVIZOUTPUT_H

#include "EntityOutput.h"
#include <vector>
#include "sara_msgs/Entity.h"
#include <ros/ros.h>

class RvizOutput : public EntityOutput {

    ros::Publisher markerPublisher;

public:
    RvizOutput(ros::NodeHandle &nh);

    ~RvizOutput() override;

    void writeEntities(const std::vector<sara_msgs::Entity> &entities) override;

    void writePerceptions(const std::vector<sara_msgs::Entity> &entities) override;

};


#endif //PROJECT_RVIZOUTPUT_H
