//
// Created by philippe on 31/10/18.
//

#include "PerceivedEntity.h"

using namespace sara_msgs;
using namespace cv;


PerceivedEntity::PerceivedEntity(float x, float y, float z, std::string name): mKF(6, 3, 0) {
    std::cout << "build\n";
    // Initialise the kalman filter.
    setIdentity(mKF.measurementMatrix);
    setIdentity(mKF.processNoiseCov, cv::Scalar::all(1e-5));
    setIdentity(mKF.measurementNoiseCov, cv::Scalar::all(10));
    setIdentity(mKF.errorCovPre, cv::Scalar::all(.5));
    setIdentity(mKF.errorCovPost, cv::Scalar::all(.5));

    // Initialise the transition matrix
    float F{1.f};
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

void PerceivedEntity::mergeOnto(Entity &source){

    // Apply the kalman filter to the position
    Mat_<float> measurement(3,1); measurement.setTo(cv::Scalar(0));
    measurement(0) = source.position.x;
    measurement(1) = source.position.y;
    measurement(2) = source.position.z;
    // TODO éssayer avec d'autres propriétées ex: hauteur, couleur, etc.
    mKF.correct(measurement);

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
