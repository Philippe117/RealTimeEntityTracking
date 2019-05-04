//
// Created by philippe on 08/11/18.
//

#include "TopicOutput.h"
#include <sara_msgs/Entities.h>

using namespace std;
using namespace sara_msgs;

TopicOutput::TopicOutput(ros::NodeHandle &nh, std::string topic) {

    mPublisher = nh.advertise<Entities>(topic, 100);
}

void TopicOutput::writeEntities(const vector<PerceivedEntity> &entities) {
    Entities msg;
    for (auto & en : entities){
        msg.entities.push_back(en);
    }

    mPublisher.publish(msg);
}

void TopicOutput::writePerceptions(const std::vector<PerceivedEntity> &entities) {

}

TopicOutput::~TopicOutput() = default;
