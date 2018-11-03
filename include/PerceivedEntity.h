//
// Created by philippe on 31/10/18.
//

#ifndef KALMAN_TEST_PERCEIVEDENTITY_H
#define KALMAN_TEST_PERCEIVEDENTITY_H

#include "sara_msgs/Entity.h"
#include "opencv2/video/tracking.hpp"
#include <iostream>


/// A child class for the Entity.msg
/// Contain a kalman filter and some utilities used for multitracking and stuff.
class PerceivedEntity : public sara_msgs::Entity {
    cv::KalmanFilter mKF;

public:

    // Constructors and destructor.
    PerceivedEntity(float x, float y, float z, std::string name="");
    PerceivedEntity(sara_msgs::Entity &entity) :
            PerceivedEntity(entity.position.x, entity.position.y, entity.position.z, entity.name){std::cout << "ttt\n";}
    ~PerceivedEntity();

    // Allow to obtain the weighted difference between two entities
    float compareWith(const sara_msgs::Entity &en) const;

    // Merge an PerceivedEntity onto this one
    void mergeOnto(sara_msgs::Entity &source);

    // Update the status of the Entity
    void update(const ros::Duration deltaTime);

    // Update the position and velocity of the Entity from the kalman filter.
    void updateStatus();

};


#endif //KALMAN_TEST_PERCEIVEDENTITY_H
