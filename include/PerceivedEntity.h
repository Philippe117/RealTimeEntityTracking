//
// Created by philippe on 31/10/18.
//

#ifndef KALMAN_TEST_PERCEIVEDENTITY_H
#define KALMAN_TEST_PERCEIVEDENTITY_H

#include "sara_msgs/Entity.h"
#include "opencv2/video/tracking.hpp"


/// A child class for the Entity.msg
/// Contain a kalman filter and some utilities used for multitracking
class PerceivedEntity : public sara_msgs::Entity {
//    cv::KalmanFilter KF;

public:
    cv::KalmanFilter KF;


    PerceivedEntity(float x, float y, float z, std::string name="");
    ~PerceivedEntity();

    // Allow to obtain the weighted difference between two entities
    float compareWith(sara_msgs::Entity &en) const;

    // Merge an PerceivedEntity onto this one
    void mergeOnto(sara_msgs::Entity &source);

    // Update the status of the Entity
    void update(ros::Duration deltaTime);

    // Update the position and velocity of the Entity from the kalman filter.
    void updateStatus();

};


#endif //KALMAN_TEST_PERCEIVEDENTITY_H
