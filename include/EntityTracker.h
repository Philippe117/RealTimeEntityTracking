//
// Created by philippe on 01/11/18.
//

#ifndef WM_ENTITY_TRACKER_ENTITYTRACKER_H
#define WM_ENTITY_TRACKER_ENTITYTRACKER_H

#include <vector>
#include "PerceivedEntity.h"
#include <ros/ros.h>
#include "EntityOutput.h"
#include <mutex>

class EntityInput;

class EntityTracker {
    std::vector<PerceivedEntity> mEntities;
    int mNextID;
    std::vector<EntityOutput *> mEntitiesOutput;
    float mDecayRate{0.0025};
    float mMaximumDifference{50.f};
    float mPublicationTreashold{0.5};

    // Check all unlikely entities and delete them.
    void deleteDeads();

    // Add an entity to the list and assing a procedural ID.
    void addEntity(sara_msgs::Entity &entity);

    // Give privileges to EntityInput class so it can call perceptions.
    friend class EntityInput;

    // Suggest the addition of new entities to the list.
    void
    perceiveEntities(std::vector<sara_msgs::Entity> entities, bool canCreate, PerceivedEntity::KalmanParams params);

    void perceiveEntity(sara_msgs::Entity entity, bool canCreate,
                        PerceivedEntity::KalmanParams params); // Same but for a single entity.

    std::mutex perceptionMutex;
public:
    EntityTracker();

    ~EntityTracker();

    // Update the status of all tracked entities.
    void update(ros::Duration deltaTime);

    float publicationTreashold() { return mPublicationTreashold; };

    void setPublicationTreashold(float value) { mPublicationTreashold = value; };

    float maximumDifference() { return mMaximumDifference; };

    void setMaximumDifference(float value) { mMaximumDifference = value; };

    float decayRate() { return mDecayRate; };

    void setDecayRate(float value) { mDecayRate = value; };

    // Accessors for outputs.
    void addOutput(EntityOutput &output);

    void clearOutputs();
};

#endif //WM_ENTITY_TRACKER_ENTITYTRACKER_H
