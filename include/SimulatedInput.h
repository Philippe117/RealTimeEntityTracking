//
// Created by philippe on 06/11/18.
//

#ifndef PROJECT_SIMULATEDINPUT_H
#define PROJECT_SIMULATEDINPUT_H

#include "EntityInput.h"
#include <thread>

class SimulatedInput : public EntityInput {
    std::thread mTread;
    void *run();
    std::vector<sara_msgs::Entity> input;

//    void perceive( std::vector<sara_msgs::Entity>& entities ) override ;

public:
    SimulatedInput(EntityTracker &tracker, int quantity);
    ~SimulatedInput();


};


#endif //PROJECT_SIMULATEDINPUT_H
