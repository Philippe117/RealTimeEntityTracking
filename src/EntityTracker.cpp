//
// Created by philippe on 01/11/18.
//
#include "EntityTracker.h"

using namespace std;
using namespace sara_msgs;

EntityTracker::EntityTracker() :
        mNextID{1}{}

EntityTracker::~EntityTracker() {
    mEntities.clear();
}

void EntityTracker::update(ros::Duration deltaTime) {

    // Delete all old entities.
    deleteDeads();

    // Update all entities.
    for (auto &entity : mEntities){
        entity.update(deltaTime);
    }

    vector<Entity> entities;
    for (auto &entity : mEntities){
        entities.push_back(Entity(entity));
    }

    // Write into all outputs
    for (auto output : mEntitiesOutput){
        output->writeEntities(entities);
    }
}

bool entityIsDead (PerceivedEntity & entity){
    return entity.probability < 0;
}

void EntityTracker::deleteDeads(){
    mEntities.erase(std::remove_if(mEntities.begin(),mEntities.end(),entityIsDead), mEntities.end());
}

void EntityTracker::perceiveEntity(Entity entity, bool canCreate, PerceivedEntity::KalmanParams params){

    // Create a list of entities to call perceiveEntities.
    vector<Entity> entities;
    entities.push_back(entity);
    return perceiveEntities(entities, canCreate, params);
}

void EntityTracker::perceiveEntities(std::vector<Entity> entities, bool canCreate, PerceivedEntity::KalmanParams params){
    //cout << "perceiving\n";
    // Write into all outputs
    for (auto output : mEntitiesOutput){
        output->writePerceptions(entities);
    }

    for (auto &perceived : entities){
        cout << "prob= " << to_string(perceived.probability) << "\n";

        // Initialise the flag that tells if a match has been found.
        bool notFoundYet{true};

        // If the ID is defined, we try to update the corresponting entity.
        if (perceived.ID != 0) {
            for (auto &entity : mEntities) {
                if (entity.ID == perceived.ID) {
                    notFoundYet = false;
                    entity.mergeOnto(perceived, params);
                    break;
                }
            }
            if (notFoundYet && canCreate){
                addEntity(perceived);
            }
        } else {

            // Use an outside loop to find the closest entity and an inside loop to validate that the match is the best around.
            // The inside loop will stop if there is already a better match.
            float minDiff{10000000.f};
            PerceivedEntity *closest{nullptr};
            for (auto &entity : mEntities) {
                auto diff{entity.compareWith(perceived)};
                if (diff < minDiff) {
                    float minDiff2{diff};
                    Entity *closest2{nullptr};
                    for (auto &perceived2 : entities) {
                        auto diff2{entity.compareWith(perceived2)};
                        if (diff2 < minDiff2) {
                            closest2 = &perceived;
                            minDiff2 = diff2;
                        }
                    }
                    if (closest2 != &perceived) {
                        minDiff = diff;
                        closest = &entity;
                    }
                }
            }
            if (closest) {
                // If the match is found, we update the entity.
                closest->mergeOnto(perceived, params);
            } else if (canCreate) {
                // If not, we create the new entity and add it to the list.
                // TODO: Maybe should not always result in creation. (maybe timer based IDK)
                addEntity(perceived);
            }
        }
    }
}


void EntityTracker::addEntity(Entity &newEntity){
    PerceivedEntity entity{PerceivedEntity(newEntity)};

    // Initialise the ID if needed.
    if (entity.ID == 0) entity.ID = mNextID++;

    entity.probability *= 0.1;  // TODO ajouter paramètre
    mEntities.push_back(entity);
    return;
}

void EntityTracker::addOutput(EntityOutput &output) {
    mEntitiesOutput.push_back(&output);
}

void EntityTracker::clearOutputs() {
    mEntitiesOutput.clear();
}
