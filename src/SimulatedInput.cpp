//
// Created by philippe on 06/11/18.
//

#include "SimulatedInput.h"
#include <iostream>
#include <ros/ros.h>
#include "opencv2/highgui/highgui.hpp"
#include "PerceivedEntity.h"

using namespace sara_msgs;
using namespace cv;
using namespace std;

SimulatedInput::SimulatedInput(EntityTracker &tracker, int quantity)
        : EntityInput(tracker),
          mTread(&SimulatedInput::run, this){
    for (int i{0}; i<quantity; ++i){
        PerceivedEntity en;
        en.position.x = 300;
        en.position.y = 300;
        input.push_back(en);
    }
}

SimulatedInput::~SimulatedInput() {
    std::cout << "Wait for thread to die.\n";
    mTread.join();
}

void *SimulatedInput::run() {
    ros::Rate rate(20);
    RNG rng;


    while (ros::ok()){

        // Calculate positions
        for (auto &in : input){
            in.position.x += rng.gaussian(9.f);
            in.position.y += rng.gaussian(9.f);
            in.position.x += (400-in.position.x)/80;
            in.position.y += (300-in.position.y)/80;
            for (auto & in2 : input){
                float dist{1.f+float(hypot(in.position.x-in2.position.x, in.position.y-in2.position.y))};
                in.position.x -= (in2.position.x-in.position.x)/dist/dist*200;
                in.position.y -= (in2.position.y-in.position.y)/dist/dist*200;
            }
        }

        vector<PerceivedEntity> en;
        for (auto &in : input) {
            if (rng.uniform(0., 1.) > 0.5){
                en.push_back(in);
            }
        }

        // Apply the detection.
        perceive(en,true);
        rate.sleep();
    }
    std::cout << "The thread died.\n";
}

