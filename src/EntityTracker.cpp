//
// Created by philippe on 01/11/18.
//
#include "EntityTracker.h"

#define drawCross( center, color, d ) \
line( img, Point( center.x - d, center.y - d ), Point( center.x + d, center.y + d ), color, 2, CV_AA, 0); \
line( img, Point( center.x + d, center.y - d ), Point( center.x - d, center.y + d ), color, 2, CV_AA, 0 )

using namespace std;
using namespace cv;

EntityTracker::~EntityTracker() {
    mEntities.clear();
}

void EntityTracker::update(ros::Duration deltaTime) {
    for (auto &entity : mEntities){
        entity.update(deltaTime);
    }
}

void EntityTracker::publishMarkers() {

}

void EntityTracker::publishOnTopic() {

}

bool EntityTracker::perceiveEntity(PerceivedEntity perceivedEntity){
    vector<PerceivedEntity> entities;
    entities.push_back(perceivedEntity);
    return perceiveEntities(entities);
}

bool EntityTracker::perceiveEntities(std::vector<PerceivedEntity> perceivedEntities){

    // Over complicated and sluggish way to match detections and entities together.
    for (auto &perceived : perceivedEntities){

        // Use an outside loop to find the closest entity and an inside loop to validate that the match is the best around.
        // The inside loop will stop as soon as it sees a better match.
        float minDiff{100.f};
        PerceivedEntity *closest{nullptr};
        for (auto &entity : mEntities){
            auto diff{perceived.compareWith(entity)};
            if (diff < minDiff){
                bool ok{true};
                for (auto &perceived2 : perceivedEntities){
                    auto diff2{entity.compareWith(perceived2)};
                    if (diff2 < diff){
                        ok = false;
                        break;
                    }
                }
                if (ok){
                    minDiff = diff;
                    closest = &entity;
                }
            }
        }
        if (closest){
            closest->mergeOnto(perceived);
        } else {
            mEntities.push_back(PerceivedEntity(perceived));
        }
    }

    return true;
}


void EntityTracker::opencvDraw(Mat img){
    for (auto& entity : mEntities) {
        Point myEntity(entity.position.x, entity.position.y);
        drawCross( myEntity, Scalar(255,255,255), 5 );
    }
}