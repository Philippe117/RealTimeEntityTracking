//
// Created by philippe on 31/10/18.
//

#include "PerceivedEntity.h"

using namespace sara_msgs;
using namespace cv;

PerceivedEntity::PerceivedEntity(float x, float y, float z, std::string name): KF(6, 3, 0) {

    // Initialise the kalman filter.
    KF.statePre.at<float>(0) = x;
    KF.statePre.at<float>(1) = y;
    KF.statePre.at<float>(2) = z;
    KF.statePre.at<float>(3) = 0;
    KF.statePre.at<float>(4) = 0;
    KF.statePre.at<float>(5) = 0;
    setIdentity(KF.measurementMatrix);
    setIdentity(KF.processNoiseCov, cv::Scalar::all(1e-4));
    setIdentity(KF.measurementNoiseCov, cv::Scalar::all(10));
    setIdentity(KF.errorCovPost, cv::Scalar::all(.1));

    // Initialise the transition matrix
    float F{0.98f};
    KF.transitionMatrix = (cv::Mat_<float>(6, 6) << F,  0,  0,  1,  0,  0,  \
                                                    0,  F,  0,  0,  1,  0,  \
                                                    0,  0,  F,  0,  0,  1,  \
                                                    0,  0,  0,  1,  0,  0,  \
                                                    0,  0,  0,  0,  1,  0,  \
                                                    0,  0,  0,  0,  0,  1   );

    // Set the Entitie property
    // TODO do all properties
    this->name = name;


    // Initialise the status of the Entity
    updateStatus();
}

PerceivedEntity::~PerceivedEntity() {
}

float PerceivedEntity::compareWith(Entity &en) const{

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
    KF.correct(measurement);

    // Update the position
    updateStatus();
}

void PerceivedEntity::update(ros::Duration deltaTime){
    // Predict the next position.
    KF.predict();
    // Update the Entity
    updateStatus();
}

void PerceivedEntity::updateStatus(){
    // Transfert the position from the state matrix to the Entity
    position.x = KF.statePost.at<double>(0);
    position.y = KF.statePost.at<double>(1);
    position.z = KF.statePost.at<double>(2);
    velocity.x = KF.statePost.at<double>(3);
    velocity.y = KF.statePost.at<double>(4);
    velocity.z = KF.statePost.at<double>(5);
}
