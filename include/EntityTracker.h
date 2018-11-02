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
public:
    EntityTracker() = default;
    ~EntityTracker();

    // Update the status of all tracked entities.
    void update(ros::Duration deltaTime);

    // Suggest the addition of a new entity to the list.
    bool perceiveEntities(std::vector<PerceivedEntity> perceivedEntities);
    bool perceiveEntity(PerceivedEntity perceivedEntity); // Same but for a single entity.

    // Publish the list of entities on a ros topic;
    void publishOnTopic();

    // Publish the list of entities as markers for rviz;
    void publishMarkers();

    // Draw on an opencv screen for test purpose.
    void opencvDraw(cv::Mat img);
};

#endif //KALMAN_TEST_ENTITYTRACKER_H
