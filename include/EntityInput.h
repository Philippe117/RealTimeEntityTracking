//
// Created by philippe on 06/11/18.
//

#ifndef PROJECT_ENTITYINPUT_H
#define PROJECT_ENTITYINPUT_H

#include <vector>
#include "sara_msgs/Entity.h"
#include "EntityTracker.h"

// Classe parent pour toutes les entrées possible d'un tracker.
class EntityInput {
    EntityTracker &mTracker;
public:
    EntityInput(EntityTracker &tracker) : mTracker{tracker} {};
    EntityTracker & tracker(){return mTracker;}
    void perceive( std::vector<sara_msgs::Entity>& entities ) {
        tracker().perceiveEntities(entities);
    }
};



#endif //PROJECT_ENTITYINPUT_H
