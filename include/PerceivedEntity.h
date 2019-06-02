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
    static float mXYWeight;
    static float mZWeight;
    static float mProbabilityWeight;
    std::vector<std::string> mAssociatedFaceIDs;
    std::vector<std::string> mAssociatedLegsIDs;

public:
    struct KalmanParams {
        double processNoiseCov{1e-5};
        double measurementNoiseCov{10.};
        double errorCovPost{0.5};
    };

    // Constructors and destructor.
    PerceivedEntity(float x=0, float y=0, float z=0, std::string name = "");

    PerceivedEntity(const sara_msgs::Entity &entity) :
            PerceivedEntity(entity.position.x, entity.position.y, entity.position.z, entity.name) {
        probability = entity.probability;
        size = entity.size;
        ID = entity.ID;
        wonderlandId = entity.wonderlandId;
        weight = entity.weight;
        waypoint = entity.waypoint;
        poseProbability = entity.poseProbability;
        isOperator = entity.isOperator;
        face = entity.face;
        containerId = entity.containerId;
        color = entity.color;
        category = entity.category;
        BoundingBox = entity.BoundingBox;
        aliases = entity.aliases;
        lastUpdateTime = entity.lastUpdateTime;
        pointcloud = entity.pointcloud;
    }
    PerceivedEntity(const PerceivedEntity& entity) :
            PerceivedEntity(static_cast<sara_msgs::Entity>(entity)) {
        mAssociatedFaceIDs = entity.mAssociatedFaceIDs;
        mAssociatedLegsIDs = entity.mAssociatedLegsIDs;
    }

    ~PerceivedEntity();

    // Allow to obtain the weighted difference between two entities
    float compareWith(const PerceivedEntity &en) const;

    // Merge an PerceivedEntity onto this one
    void mergeOnto(PerceivedEntity &source, KalmanParams params);

    // Update the status of the Entity
    void update(const ros::Duration deltaTime);

    // Update the position and velocity of the Entity from the kalman filter.
    void updateStatus();

    // Accesseurs
    static float xYWeight() { return mXYWeight; }

    static void setXYWeight(float value) { mXYWeight = value; }

    static float zWeight() { return mZWeight; }

    static void setZWeight(float value) { mZWeight = value; }

    static float probabilityWeight() { return mProbabilityWeight; }

    static void setProbabilityWeight(float value) { mProbabilityWeight = value; }

    // Verify if a faceID is in the list of associated faceIDs.
    bool checkFaceID(std::string faceID);
    // Add a faceID to the list of associated faceIDs. Return true if the face already exist in the list.
    bool addFaceID(std::string faceID);
    bool addFaceID(std::vector<std::string> faceIDs);
    std::vector<std::string> associatedFaceIDs() const { return mAssociatedFaceIDs; }

    // Verify if a legsID is in the list of associated faceIDs.
    bool checkLegsID(std::string legsID);
    // Add a legsID to the list of associated legsIDs. Return true if the face already exist in the list.
    bool addLegsID(std::string legsID);
    bool addLegsID(std::vector<std::string> legsIDs);
    std::vector<std::string> associatedLegsIDs() const { return mAssociatedLegsIDs; }
};


#endif //WM_ENTITY_TRACKER_PERCEIVEDENTITY_H
