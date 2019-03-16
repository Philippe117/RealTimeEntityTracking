//
// Created by philippe on 08/11/18.
//

#ifndef PROJECT_RVIZOUTPUT_H
#define PROJECT_RVIZOUTPUT_H

#include "EntityOutput.h"
#include <vector>
#include <ros/ros.h>
#include "PerceivedEntity.h"

class RvizOutput : public EntityOutput {

    ros::Publisher markerPublisher;

public:
    RvizOutput(ros::NodeHandle &nh);

    ~RvizOutput() override;

    void writeEntities(const std::vector<PerceivedEntity> &entities) override;

    void writePerceptions(const std::vector<PerceivedEntity> &entities) override;

};


#endif //PROJECT_RVIZOUTPUT_H
