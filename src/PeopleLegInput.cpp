//
// Created by philippe on 08/11/18.
//

#include "PeopleLegInput.h"
#include <iostream>

using namespace std;
using namespace sara_msgs;
using namespace people_msgs;

PeopleLegInput::PeopleLegInput(EntityTracker &tracker, ros::NodeHandle &nh, const string topic)
        : EntityInput(tracker),
          mNodeHandle(nh),
          mSpinner(1),
          mPeopleSub() {
    mPeopleSub = nh.subscribe(topic, 10, &PeopleLegInput::peopleLegCallback, this);
    mSpinner.start();
}

PeopleLegInput::~PeopleLegInput() {
    mSpinner.stop();
}

void PeopleLegInput::peopleLegCallback(PositionMeasurementArray legArray) {
    vector<PerceivedEntity> entities;

    for (auto legs : legArray.people) {
        if (legs.reliability > 0) {
            PerceivedEntity en;

            // We look for a match in previously detected legs
            for (auto & entity : tracker().entities()){
                if (entity.checkLegsID(legs.object_id)) {
                    // If a match is found, we force the entity ID.
                    en.ID = entity.ID;
                    break;
                }
            }

            en.addLegsID(legs.object_id);
            en.position = legs.pos;
            en.face.boundingBox.Center.z = 1.5;
            en.probability = legs.reliability * 0.25; // TODO fixit
            en.name = "person";
            en.lastUpdateTime = legs.header.stamp;

            entities.push_back(en);
        }
    }
    perceive(entities, false);
}
