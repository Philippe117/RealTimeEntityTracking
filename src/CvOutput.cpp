//
// Created by philippe on 08/11/18.
//

#include "CvOutput.h"
#include <iostream>
#include <opencv-3.3.1-dev/opencv2/imgproc.hpp>

using namespace cv;
using namespace std;
using namespace sara_msgs;

#define drawCross( center, color, d ) \
cv::line( img, Point( center.x - d, center.y - d ), Point( center.x + d, center.y + d ), color, 2, CV_AA, 0); \
cv::line( img, Point( center.x + d, center.y - d ), Point( center.x - d, center.y + d ), color, 2, CV_AA, 0 )

#define drawX( center, color, d ) \
cv::line( img, Point( center.x, center.y - d ), Point( center.x, center.y + d ), color, 2, CV_AA, 0); \
cv::line( img, Point( center.x + d, center.y ), Point( center.x - d, center.y ), color, 2, CV_AA, 0 )


CvOutput::CvOutput(float scaling, float centerX, float centerY)
        : img(600, 800, CV_8UC3),
          mScaling(scaling),
          mCenterX(centerX),
          mCenterY(centerY){

    namedWindow("mouse kalman", 1);
}


void CvOutput::writeEntities(const vector<Entity>& entities) {


    for (auto& entity : entities) {
        Point myEntity(entity.position.x*mScaling-mCenterX, entity.position.y*mScaling-mCenterY);
        drawCross( myEntity, Scalar(255,255,255), 5 );
        cv::putText(img, to_string(entity.ID), Point(entity.position.x*mScaling-mCenterX, entity.position.y*mScaling-mCenterY-10), FONT_HERSHEY_COMPLEX, 1, 255);
        cv::putText(img, entity.name, Point(entity.position.x*mScaling-mCenterX+10, entity.position.y*mScaling-mCenterY), FONT_HERSHEY_COMPLEX, 0.5, 255);
    }
    cv::putText(img, "entities = " + to_string(entities.size()), Point(20, 20), FONT_HERSHEY_COMPLEX, 1, 255);

    imshow("mouse kalman", img);
    waitKey(10);
}

void CvOutput::writePerceptions(const vector<Entity>& entities) {
    img = Scalar::all(0);
    for (auto& entity : entities) {
        Point myEntity(entity.position.x*mScaling-mCenterX, entity.position.y*mScaling-mCenterY);
        drawX( myEntity, Scalar(40,40,200), 6 );
    }
    cv::putText(img, "perceptions = " + to_string(entities.size()), Point(20, 50), FONT_HERSHEY_COMPLEX, 1, 255);

}


CvOutput::~CvOutput() = default;
