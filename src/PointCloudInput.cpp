//
// Created by philippe on 11/11/18.
//

#include "PointCloudInput.h"


using namespace std;


PointCloudInput::PointCloudInput(EntityTracker &tracker, ros::NodeHandle &nh, const string topic)
        : EntityInput(tracker),
          mNodeHandle(nh),
          mSpinner(1),
          mPointCloudSub() {
    // Subscribe to the bounding box topic
    mPointCloudSub = nh.subscribe(topic, 10, &PointCloudInput::PointCloudCallback, this);

    // Start the ros async spinner.
    mSpinner.start();
}

PointCloudInput::~PointCloudInput() {
    mSpinner.stop();
}

void PointCloudInput::PointCloudCallback(sara_msgs::PointClouds PCs) {


    vector<PerceivedEntity> entities;

    // Create the entities from the bounding boxes
    for (auto &PC : PCs.pointClouds) {

        PerceivedEntity en;
        en.BoundingBox = PC.boundingBox;
        en.pointcloud = PC.pointCloud;
        en.position = PC.boundingBox.Center;
        en.name = "unknown";
        en.probability = 0.8;
        en.lastUpdateTime = PCs.header.stamp;

        if (en.probability > 0) {
            // If the entity is a person
            entities.push_back(en);
        } else {
            ROS_WARN("rejecting an entity of name : %s because it's probability is %lf", en.name.c_str(),
                     en.probability);
        }
    }


    perceive(entities, true);
}