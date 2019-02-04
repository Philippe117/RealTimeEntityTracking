//
// Created by philippe on 08/11/18.
//

#include "RvizOutput.h"
#include <iostream>

using namespace std;
using namespace sara_msgs;

RvizOutput::RvizOutput(){

}


void RvizOutput::writeEntities(const vector<Entity>& entities) {

    for (auto& entity : entities) {

    }
}


void RvizOutput::writePerceptions(const vector<Entity>& entities) {
    for (auto& entity : entities) {

    }
}


RvizOutput::~RvizOutput() = default;
