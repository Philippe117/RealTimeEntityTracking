//
// Created by philippe on 08/11/18.
//

#ifndef PROJECT_ENTITYPUBLISHER_H
#define PROJECT_ENTITYPUBLISHER_H

#include "EntityOutput.h"
#include <vector>
#include <ros/ros.h>
#include "PerceivedEntity.h"

class TopicOutput : public EntityOutput {

    ros::Publisher mPublisher;

public:
    TopicOutput(ros::NodeHandle &nh, std::string topic);

    ~TopicOutput() override;

    void writeEntities(const std::vector<PerceivedEntity> &entities) override;

    void writePerceptions(const std::vector<PerceivedEntity> &entities) override;


};


#endif //PROJECT_ENTITYPUBLISHER_H
