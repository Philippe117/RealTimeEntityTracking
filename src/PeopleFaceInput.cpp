//
// Created by philippe on 08/11/18.
//

#include "PeopleFaceInput.h"
#include <iostream>

using namespace std;
using namespace sara_msgs;

PeopleFaceInput::PeopleFaceInput(EntityTracker &tracker, ros::NodeHandle &nh, const string topic)
        : EntityInput(tracker),
          mNodeHandle(nh),
          mSpinner(1),
          mFacesSub()
{
    mFacesSub = nh.subscribe(topic, 10, &PeopleFaceInput::peopleFaceCallback, this);
    cout << "buildo\n";
    mSpinner.start();
}

PeopleFaceInput::~PeopleFaceInput() {
    mSpinner.stop();
}

void PeopleFaceInput::peopleFaceCallback(sara_msgs::Faces faceArray) {
    vector<PerceivedEntity> entities;

    for (auto &face : faceArray.faces) {
        // Creating a new entity with a face
        PerceivedEntity en;
        en.position = face.boundingBox.Center;
        en.position.z = 0;
        en.face = face;
        en.probability = 1;
        en.name = "person";
        en.lastUpdateTime = faceArray.header.stamp;

        // We look for a match in previously detected faces
        for (auto & entity : tracker().entities()){
            if (entity.checkFaceID(en.face.id)) {
                // If a match is found, we force the entity ID.
                en.ID = entity.ID;
                break;
            }
        }
        entities.push_back(en);
    }
    perceive(entities, true);
}
