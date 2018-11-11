//
// Created by philippe on 11/11/18.
//

#include "BoundingBoxesInput.h"


using namespace std;
using namespace sara_msgs;


BoundingBoxesInput::BoundingBoxesInput(EntityTracker &tracker, ros::NodeHandle &nh, const string topic)
        : EntityInput(tracker),
          mNodeHandle(nh),
          mSpinner(1),
          mBoundingBoxesSub()
{
    mBoundingBoxesSub = nh.subscribe(topic, 10, &BoundingBoxesInput::BoundingBoxesCallback, this);
    cout << "buildo\n";
    mSpinner.start();
}

BoundingBoxesInput::~BoundingBoxesInput() {
    mSpinner.stop();
}

void BoundingBoxesInput::BoundingBoxesCallback(sara_msgs::BoundingBoxes3D boundingBoxes3D) {


    vector<Entity> entities;

    // Create the entities from the bounding boxes
    for (auto &boundingBox : boundingBoxes3D.boundingBoxes) {

        sara_msgs::Entity en;
        en.BoundingBox = boundingBox;
        en.name = boundingBox.Class;
        en.position = boundingBox.Center;
        en.probability = boundingBox.probability;
        en.lastUpdateTime = boundingBoxes3D.header.stamp;

        if (en.probability > 0.1) {
            // If the entity is a person
            if (en.name.compare("person") == 0) {
                en.position.z = 0;
            }
            entities.push_back(en);
        } else {
            ROS_WARN("rejecting an entity of name : %s because it's probability is %lf", en.name.c_str(), en.probability);
        }
    }


    PerceivedEntity::KalmanParams params;
    params.processNoiseCov = 1e-2;
    params.measurementNoiseCov = 10;
    params.errorCovPre = 0.9;
    params.errorCovPost = 0.9;
    perceive(entities, true, params);
}