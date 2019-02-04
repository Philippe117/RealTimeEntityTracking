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
    vector<Entity> entities;

    for (auto &face : faceArray.faces) {
        sara_msgs::Entity en;
        en.position = face.boundingBox.Center;
        en.position.z = 0;
        en.face = face;
        en.probability = 0.5;
        en.name = "person";
        en.lastUpdateTime = faceArray.header.stamp;

        entities.push_back(en);
    }
    perceive(entities, true);
}
