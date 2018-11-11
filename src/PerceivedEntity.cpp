//
// Created by philippe on 31/10/18.
//

#include "PerceivedEntity.h"

using namespace sara_msgs;
using namespace cv;


PerceivedEntity::PerceivedEntity(float x, float y, float z, std::string name): mKF(6, 3, 0) {

    // Initialise the kalman filter.
    setIdentity(mKF.measurementMatrix);

    // Initialise the transition matrix
    float F{0.99f};
    mKF.transitionMatrix = (cv::Mat_<float>(6, 6) <<1,  0,  0,  1,  0,  0,  \
                                                    0,  1,  0,  0,  1,  0,  \
                                                    0,  0,  1,  0,  0,  1,  \
                                                    0,  0,  0,  F,  0,  0,  \
                                                    0,  0,  0,  0,  F,  0,  \
                                                    0,  0,  0,  0,  0,  F   );

    // Set the Entitie property
    // TODO do all properties
    this->name = name;

    mKF.statePre.at<float>(0) = x;
    mKF.statePre.at<float>(1) = y;
    mKF.statePre.at<float>(2) = z;
    mKF.statePost.at<float>(0) = x;
    mKF.statePost.at<float>(1) = y;
    mKF.statePost.at<float>(2) = z;

    lastUpdateTime = ros::Time::now();
}

PerceivedEntity::~PerceivedEntity() {
}

float PerceivedEntity::compareWith(const Entity &en) const{

    // Obtain the distance on each axis.
    float dX{float(position.x-en.position.x)};
    float dY{float(position.y-en.position.y)};
    float dZ{float(position.z-en.position.z)};

    // Calculate the hypotenuse.
    float distance{sqrt(dX*dX + dY*dY + dZ*dZ)};

    // Initialise the difference.
    float difference{0.0f};

    // TODO Compare the two Entities using the weighted difference.

    return distance+difference;

}

void PerceivedEntity::mergeOnto(Entity &source, KalmanParams params){

    // Update the filter.
    mKF.predict();

    // Apply the kalman filter to the position
    Mat_<float> measurement(3,1); measurement.setTo(cv::Scalar(0));
    measurement(0) = float(source.position.x);
    measurement(1) = float(source.position.y);
    measurement(2) = float(source.position.z);
    // TODO éssayer avec d'autres propriétées ex: hauteur, couleur, etc.

    // Apply the input parameters and do the kalman correction
    setIdentity(mKF.processNoiseCov, cv::Scalar::all(params.processNoiseCov));
    setIdentity(mKF.measurementNoiseCov, cv::Scalar::all(params.measurementNoiseCov));
    setIdentity(mKF.errorCovPre, cv::Scalar::all(params.errorCovPre));
    setIdentity(mKF.errorCovPost, cv::Scalar::all(params.errorCovPost));
    mKF.correct(measurement);

    lastUpdateTime = ros::Time::now();

    // Update the position
    updateStatus();
}

void PerceivedEntity::update(const ros::Duration deltaTime){
    // Predict the next position.
    mKF.predict();
    // Update the Entities
    updateStatus();
}

void PerceivedEntity::updateStatus(){
    // Transfert the position from the state matrix to the Entity
    position.x = mKF.statePost.at<float>(0);
    position.y = mKF.statePost.at<float>(1);
    position.z = mKF.statePost.at<float>(2);
    velocity.x = mKF.statePost.at<float>(3);
    velocity.y = mKF.statePost.at<float>(4);
    velocity.z = mKF.statePost.at<float>(5);
}
