//
// Created by philippe on 06/11/18.
//

#ifndef PROJECT_ENTITYOUTPUT_H
#define PROJECT_ENTITYOUTPUT_H

#include <vector>
#include "sara_msgs/Entity.h"

class EntityOutput {
    EntityOutput();
    virtual ~EntityOutput()= 0;
    virtual void write(const std::vector<sara_msgs::Entity>& entities)= 0;
};


#endif //PROJECT_ENTITYOUTPUT_H
