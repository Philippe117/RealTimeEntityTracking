//
// Created by philippe on 01/11/18.
//

#ifndef WM_ENTITY_TRACKER_ENTITYTRACKER_H
#define WM_ENTITY_TRACKER_ENTITYTRACKER_H

#include <vector>
#include "PerceivedEntity.h"
#include "opencv2/highgui/highgui.hpp"

class EntityTracker {
    std::vector<PerceivedEntity> mEntities;
    ros::Duration mDeleteDelay;
    int mNextID;

    // Check all old entities and delete them.
    void deleteOld();

    // Add an entity to the list and assing a procedural ID.
    void addEntity(sara_msgs::Entity &entity);

public:
    EntityTracker(ros::Duration deleteDelay=ros::Duration(5.f));
    ~EntityTracker();

    // Update the status of all tracked entities.
    void update(ros::Duration deltaTime);

    // Suggest the addition of a new entity to the list.
    void perceiveEntities(std::vector<sara_msgs::Entity> entities);
    void perceiveEntity(sara_msgs::Entity entity); // Same but for a single entity.

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

#endif //WM_ENTITY_TRACKER_ENTITYTRACKER_H
