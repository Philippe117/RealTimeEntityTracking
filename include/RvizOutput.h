//
// Created by philippe on 08/11/18.
//

#ifndef PROJECT_CVOUTPUT_H
#define PROJECT_CVOUTPUT_H

#include "EntityOutput.h"
#include <vector>
#include "sara_msgs/Entity.h"

class RvizOutput : public EntityOutput {

public:
    RvizOutput();
    ~RvizOutput() override ;
    void writeEntities(const std::vector<sara_msgs::Entity>& entities) override ;
    void writePerceptions(const std::vector<sara_msgs::Entity>& entities) override ;

};


#endif //PROJECT_CVOUTPUT_H
