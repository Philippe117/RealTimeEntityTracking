//
// Created by philippe on 06/11/18.
//

#ifndef PROJECT_ENTITYOUTPUT_H
#define PROJECT_ENTITYOUTPUT_H

#include <vector>
#include "PerceivedEntity.h"

class EntityOutput {
public:
    EntityOutput() = default;

    virtual ~EntityOutput() = default;

    virtual void writeEntities(const std::vector<PerceivedEntity> &entities)= 0;

    virtual void writePerceptions(const std::vector<PerceivedEntity> &perceptions)= 0;
};


#endif //PROJECT_ENTITYOUTPUT_H
