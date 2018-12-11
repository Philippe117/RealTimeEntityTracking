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
          mPeopleSub()
{
    mPeopleSub = nh.subscribe(topic, 10, &PeopleLegInput::peopleLegCallback, this);
    cout << "buildo\n";
    mSpinner.start();
}

PeopleLegInput::~PeopleLegInput() {
    mSpinner.stop();
}

void PeopleLegInput::peopleLegCallback(PositionMeasurementArray legArray) {
    cout << "test\n";
    vector<Entity> entities;

    for (auto legs : legArray.people) {
        if (legs.reliability > 0) {
            sara_msgs::Entity en;
            en.position = legs.pos;
            en.probability = 0.5;
            en.name = "person";
            en.lastUpdateTime = legs.header.stamp;

            entities.push_back(en);
        }
    }
    perceive(entities, false);
}
