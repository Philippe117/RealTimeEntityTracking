//
// Created by philippe on 06/11/18.
//

#ifndef PROJECT_ENTITYINPUT_H
#define PROJECT_ENTITYINPUT_H

#include <vector>
#include "PerceivedEntity.h"
#include "EntityTracker.h"

// Classe parent pour toutes les entrées possible d'un tracker.
class EntityInput {
    EntityTracker &mTracker;
    PerceivedEntity::KalmanParams mKalmanParams;
public:
    EntityInput(EntityTracker &tracker) : mTracker{tracker} {};

    EntityTracker &tracker() const { return mTracker; }

    void perceive(std::vector<PerceivedEntity> &entities, bool canCreate) {
        tracker().perceiveEntities(entities, canCreate, mKalmanParams);
    }

    PerceivedEntity::KalmanParams kalmanParams() { return mKalmanParams; }

    void setKalmanParams(PerceivedEntity::KalmanParams params) { mKalmanParams = params; }
};


#endif //PROJECT_ENTITYINPUT_H
