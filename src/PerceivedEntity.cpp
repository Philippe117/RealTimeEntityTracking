//
// Created by philippe on 31/10/18.
//

#include "PerceivedEntity.h"

using namespace cv;

float PerceivedEntity::mXYWeight = 1.0;
float PerceivedEntity::mZWeight = 1.0;
float PerceivedEntity::mProbabilityWeight = 1.0;
float PerceivedEntity::mSizeWeight = 1.0;


PerceivedEntity::PerceivedEntity(float x, float y, float z, std::string name) :
        mKF(6, 3, 0) {

    // Initialise the kalman filter.
    setIdentity(mKF.measurementMatrix);

    // Initialise the transition matrix
    float F{0.98f};
    mKF.transitionMatrix = (cv::Mat_<float>(6, 6) << 1, 0, 0, 1, 0, 0, \
                                                    0, 1, 0, 0, 1, 0, \
                                                    0, 0, 1, 0, 0, 1, \
                                                    0, 0, 0, F, 0, 0, \
                                                    0, 0, 0, 0, F, 0, \
                                                    0, 0, 0, 0, 0, F*F*F);

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
    updateStatus();
}

PerceivedEntity::~PerceivedEntity() {
    mAssociatedLegsIDs.clear();
    mAssociatedFaceIDs.clear();
}

float PerceivedEntity::compareWith(const PerceivedEntity &en) const {

    // Obtain the distance on each axis.
    float dX{float(position.x - en.position.x)};
    float dY{float(position.y - en.position.y)};
    float dZ{float(position.z - en.position.z)};

    // Calculate the hypotenuse.
    float distance{sqrt(dX * dX * mXYWeight + dY * dY * mXYWeight + dZ * dZ * mZWeight)};

    // Initialise the difference.
    float difference{0.0f};


    if (ID != en.ID) {
        // Prend compte des probabilitées
        difference +=
                probability > 0 ? probabilityWeight() / probability : 100000;  // TODO Utiliser un paramêtre de weight
        difference +=
                en.probability > 0 ? probabilityWeight() / en.probability
                                   : 100000;  // TODO Utiliser un paramêtre de weight

        if (name.compare(mUnknownName) != 0 && en.name.compare(mUnknownName) != 0
		    && name.compare(en.name) != 0) difference += 100000;
        if ((name.compare(mUnknownName) == 0 | en.name.compare(mUnknownName))
            && (name.compare("person") == 0 | en.name.compare("person"))) difference += 100000;


        double sx1{BoundingBox.Width*BoundingBox.Width};
        double sy1{BoundingBox.Height*BoundingBox.Height};
        double sz1{BoundingBox.Depth*BoundingBox.Depth};
        double size1{sqrt(sx1+sy1+sz1)};

        double sx2{en.BoundingBox.Width*en.BoundingBox.Width};
        double sy2{en.BoundingBox.Height*en.BoundingBox.Height};
        double sz2{en.BoundingBox.Depth*en.BoundingBox.Depth};
        double size2{sqrt(sx2+sy2+sz2)};

        if (size1 != 0 && size2 != 0){
            difference += abs(size1-size2)*mSizeWeight;
        }
    }

    return distance + difference;

}

void PerceivedEntity::mergeOnto(PerceivedEntity &source, KalmanParams params) {


    // Update the filter.
    mKF.predict();

    // Apply the kalman filter to the position
    Mat_<float> measurement(3, 1);
    measurement.setTo(cv::Scalar(0));
    measurement(0) = float(source.position.x);
    measurement(1) = float(source.position.y);
    measurement(2) = float(source.position.z);
    // TODO éssayer avec d'autres propriétées ex: hauteur, couleur, etc.

    mergeOnto(source);

    // Apply the input parameters and do the kalman correction
    setIdentity(mKF.processNoiseCov, cv::Scalar::all(params.processNoiseCov));
    setIdentity(mKF.measurementNoiseCov, cv::Scalar::all(params.measurementNoiseCov));
    //setIdentity(mKF.errorCovPre, cv::Scalar::all(params.errorCovPre));
    setIdentity(mKF.errorCovPost, cv::Scalar::all(params.errorCovPost));
    mKF.correct(measurement);

}

void PerceivedEntity::mergeOnto(PerceivedEntity &source) {

    // Add the face if needed
    if (source.associatedFaceIDs().size()){
        addFaceID(source.associatedFaceIDs() );
        face = source.face;
    }

    // Add the legs if needed
    if (source.associatedLegsIDs().size()){
        addLegsID(source.associatedLegsIDs() );
    }

    if (source.lastUpdateTime > lastUpdateTime && source.pointcloud.header.stamp != ros::Time(0)){
        pointcloud = source.pointcloud;
    }

    // Move the head
    face.boundingBox.Center.x = position.x;
    face.boundingBox.Center.y = position.y;
    if (face.id.empty()) face.boundingBox.Center.z = position.z;

    // Associate a new name if needed
    if (name.compare(mUnknownName) == 0){
        name = source.name;
    }

    lastUpdateTime = ros::Time::now();

    if (probability < source.probability)
        probability += (source.probability - probability) * 0.5; // TODO ajouter un taux paramétrable
}

void PerceivedEntity::update(const ros::Duration deltaTime) {
    // Predict the next position.
    mKF.predict();
    // Update the Entities
    updateStatus();
}

void PerceivedEntity::updateStatus() {
    // Transfert the position from the state matrix to the Entity
    //std::cout << mKF.statePost << "\n";
    position.x = mKF.statePost.at<float>(0);
    position.y = mKF.statePost.at<float>(1);
    position.z = mKF.statePost.at<float>(2);
    velocity.x = mKF.statePost.at<float>(3);
    velocity.y = mKF.statePost.at<float>(4);
    velocity.z = mKF.statePost.at<float>(5);
}

bool PerceivedEntity::checkFaceID(std::string faceID) {
    for (auto & ID : mAssociatedFaceIDs){
        if (ID == faceID){
            return true;
        }
    }
    return false;
}

bool PerceivedEntity::addFaceID(std::string faceID) {
    if (!checkFaceID(faceID)){
        mAssociatedFaceIDs.push_back(faceID);
    }
    return true;
}

bool PerceivedEntity::addFaceID(std::vector<std::string> faceIDs) {
    for (auto &id : faceIDs){
        addFaceID(id);
    }
    return true;
}

bool PerceivedEntity::checkLegsID(std::string legID) {
    for (auto & ID : mAssociatedLegsIDs){
        if (ID == legID){
            return true;
        }
    }
    return false;
}

bool PerceivedEntity::addLegsID(std::string legsID) {
    if (!checkLegsID(legsID)){
        mAssociatedLegsIDs.push_back(legsID);
    }
    return true;
}

bool PerceivedEntity::addLegsID(std::vector<std::string> legsIDs) {
    for (auto &id : legsIDs){
        addLegsID(id);
    }
    return true;
}
