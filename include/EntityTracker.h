//
// Created by philippe on 01/11/18.
//

#ifndef KALMAN_TEST_ENTITYTRACKER_H
#define KALMAN_TEST_ENTITYTRACKER_H

#include <vector>
#include "PerceivedEntity.h"
#include "opencv2/highgui/highgui.hpp"

class EntityTracker {
    std::vector<PerceivedEntity> mEntities;
    ros::Duration mDeleteDelay;
    int mNextID;

    // Check all old entities and delete them.
    void deleteOld();

    // Add an entity to the list and assing a procedural ID. If the ID is specified, do we merge instead.
    void addEntity(float x, float y, float z, std::string name, int ID = -1);

public:
    EntityTracker(ros::Duration deleteDelay=ros::Duration(5.f));
    ~EntityTracker();

    // Update the status of all tracked entities.
    void update(ros::Duration deltaTime);

    // Suggest the addition of a new entity to the list.
    bool perceiveEntities(std::vector<sara_msgs::Entity> entities);
    bool perceiveEntity(sara_msgs::Entity entity); // Same but for a single entity.

    // Publish the list of entities on a ros topic;
    void publishOnTopic() const;

    // Publish the list of entities as markers for rviz;
    void publishMarkers() const;

    // Draw on an opencv screen for test purpose.
    void opencvDraw(cv::Mat img) const;

    // Accessors for deleteDelayé
    ros::Duration deleteDelay() const;
    ros::Duration setDeleteDelay(ros::Duration value);
};

#endif //KALMAN_TEST_ENTITYTRACKER_H
