//
// Created by philippe on 06/11/18.
//

#ifndef PROJECT_ENTITYOUTPUT_H
#define PROJECT_ENTITYOUTPUT_H

#include <vector>
#include "sara_msgs/Entity.h"

class EntityOutput {
public:
    EntityOutput() = default;

    virtual ~EntityOutput() = default;

    virtual void writeEntities(const std::vector<sara_msgs::Entity> &entities)= 0;

    virtual void writePerceptions(const std::vector<sara_msgs::Entity> &perceptions)= 0;
};


#endif //PROJECT_ENTITYOUTPUT_H
