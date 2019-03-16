//
// Created by philippe on 08/11/18.
//

#include "RvizOutput.h"
#include <iostream>
#include "visualization_msgs/Marker.h"

using namespace std;
using namespace sara_msgs;

RvizOutput::RvizOutput(ros::NodeHandle &nh) {


    markerPublisher = nh.advertise<visualization_msgs::Marker>("entity_tracker/markers", 100);
}


void RvizOutput::writeEntities(const vector<PerceivedEntity> &entities) {
    visualization_msgs::Marker m;
    m.header.stamp = ros::Time::now();
    m.lifetime = ros::Duration(0.5);
    m.header.frame_id = "/map";
    m.ns = "entities";
    m.type = m.SPHERE;
    m.scale.x = 0.15;
    m.scale.y = 0.15;
    m.scale.z = 0.15;
    m.color.b = 0.1;
    m.color.a = 0.5;

    visualization_msgs::Marker face;
    face.header.stamp = ros::Time::now();
    face.lifetime = ros::Duration(0.5);
    face.header.frame_id = "/map";
    face.ns = "face";
    face.type = m.SPHERE;
    face.scale.x = 0.15;
    face.scale.y = 0.15;
    face.scale.z = 0.15;
    face.color.r = 0.1;
    face.color.g = 0.1;
    face.color.b = 0.1;
    face.color.a = 0.4;

    visualization_msgs::Marker faceID;
    faceID.header.stamp = ros::Time::now();
    faceID.lifetime = ros::Duration(0.5);
    faceID.header.frame_id = "/map";
    faceID.ns = "faceID";
    faceID.type = faceID.TEXT_VIEW_FACING;
    faceID.scale.x = 0.06;
    faceID.scale.y = 0.06;
    faceID.scale.z = 0.06;
    faceID.color.r = 1;
    faceID.color.g = 1;
    faceID.color.b = 0.0;
    faceID.color.a = 1;
    
    visualization_msgs::Marker nametag;
    nametag.header.stamp = ros::Time::now();
    nametag.lifetime = ros::Duration(0.5);
    nametag.header.frame_id = "/map";
    nametag.ns = "nametags";
    nametag.type = nametag.TEXT_VIEW_FACING;
    nametag.scale.x = 0.08;
    nametag.scale.y = 0.08;
    nametag.scale.z = 0.08;
    nametag.color.r = 1;
    nametag.color.g = 1;
    nametag.color.b = 0.0;
    nametag.color.a = 1;

    visualization_msgs::Marker idTag;
    idTag.header.stamp = ros::Time::now();
    idTag.lifetime = ros::Duration(0.5);
    idTag.header.frame_id = "/map";
    idTag.ns = "idTags";
    idTag.type = idTag.TEXT_VIEW_FACING;
    std::string temp;
    idTag.text = temp;
    idTag.scale.x = 0.12;
    idTag.scale.y = 0.12;
    idTag.scale.z = 0.12;
    idTag.color.r = 1;
    idTag.color.g = 1;
    idTag.color.b = 0.0;
    idTag.color.a = 1;

    for (auto &entity : entities) {
        m.id = entity.ID;
        m.color.r = float(1 - entity.probability);
        m.color.g = float(entity.probability);
        m.pose.position = entity.position;
        markerPublisher.publish(m);

        nametag.id = entity.ID;
        nametag.text = entity.color + " " + entity.name;
        nametag.pose.position.x = entity.position.x;
        nametag.pose.position.y = entity.position.y;
        nametag.pose.position.z = entity.position.z + 0.2;
        markerPublisher.publish(nametag);

        idTag.id = entity.ID;
        idTag.text = std::to_string(entity.ID);
        idTag.pose.position.x = entity.position.x;
        idTag.pose.position.y = entity.position.y;
        idTag.pose.position.z = entity.position.z + 0.10;
        markerPublisher.publish(idTag);

        face.id = entity.ID;
        face.pose.position.x = entity.face.boundingBox.Center.x;
        face.pose.position.y = entity.face.boundingBox.Center.y;
        face.pose.position.z = entity.face.boundingBox.Center.z;
        markerPublisher.publish(face);

        faceID.id = entity.ID;
        faceID.text = entity.face.id.c_str();
        faceID.pose.position.x = entity.face.boundingBox.Center.x;
        faceID.pose.position.y = entity.face.boundingBox.Center.y;
        faceID.pose.position.z = entity.face.boundingBox.Center.z+0.1;
        markerPublisher.publish(faceID);
    }
}


void RvizOutput::writePerceptions(const vector<PerceivedEntity> &entities) {

    visualization_msgs::Marker m;
    m.header.stamp = ros::Time::now();
    m.lifetime = ros::Duration(0.2);
    m.header.frame_id = "/map";
    m.ns = "perceptions";
    m.type = m.SPHERE;
    m.scale.x = 0.05;
    m.scale.y = 0.05;
    m.scale.z = 0.05;
    m.color.r = 1;
    m.color.g = 0;
    m.color.b = 0;
    m.color.a = 1;
    for (auto &entity : entities) {
        int i{0};
        {
            m.id = ros::Time::now().toNSec();
            m.pose.position = entity.position;
            markerPublisher.publish(m);
        }
    }
}


RvizOutput::~RvizOutput() = default;
