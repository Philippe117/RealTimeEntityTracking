//
// Created by philippe on 31/10/18.
//

#ifndef WM_ENTITY_TRACKER_PERCEIVEDENTITY_H
#define WM_ENTITY_TRACKER_PERCEIVEDENTITY_H

#include "sara_msgs/Entity.h"
#include "opencv2/video/tracking.hpp"
#include <iostream>
#include <ros/ros.h>


/// A child class for the Entity.msg
/// Contain a kalman filter and some utilities used for multitracking and stuff.
class PerceivedEntity : public sara_msgs::Entity {
    cv::KalmanFilter mKF;

public:
    struct KalmanParams{
        double processNoiseCov{1e-5};
        double measurementNoiseCov{10.};
        double errorCovPost{0.5};
    };

    // Constructors and destructor.
    PerceivedEntity(float x, float y, float z, std::string name="");
    PerceivedEntity(sara_msgs::Entity &entity) :
            PerceivedEntity(entity.position.x, entity.position.y, entity.position.z, entity.name){}
    ~PerceivedEntity();

    // Allow to obtain the weighted difference between two entities
    float compareWith(const sara_msgs::Entity &en) const;

    // Merge an PerceivedEntity onto this one
    void mergeOnto(sara_msgs::Entity &source, KalmanParams params);

    // Update the status of the Entity
    void update(const ros::Duration deltaTime);

    // Update the position and velocity of the Entity from the kalman filter.
    void updateStatus();

};


#endif //WM_ENTITY_TRACKER_PERCEIVEDENTITY_H
