//
// Created by philippe on 08/11/18.
//

#include "CvOutput.h"
#include <iostream>
#include <opencv-3.3.1-dev/opencv2/imgproc.hpp>

using namespace cv;
using namespace std;
using namespace sara_msgs;

#define rescaleX(x) int((x-mMinX)*mScalingX)

#define rescaleY(y) int((y-mMinY)*mScalingY)

#define drawCross(center, color, d) \
cv::line( img, Point( center.x - d, center.y - d ), Point( center.x + d, center.y + d ), color, 2, CV_AA, 0); \
cv::line( img, Point( center.x + d, center.y - d ), Point( center.x - d, center.y + d ), color, 2, CV_AA, 0 )

#define drawX(center, color, d) \
cv::line( img, Point( center.x, center.y - d ), Point( center.x, center.y + d ), color, 2, CV_AA, 0); \
cv::line( img, Point( center.x + d, center.y ), Point( center.x - d, center.y ), color, 2, CV_AA, 0 )


CvOutput::CvOutput(float minX, float minY, float maxX, float maxY)
        : img(SCREENSIZEY, SCREENSIZEX, CV_8UC3),
          mScalingX(SCREENSIZEX / -(minX - maxX)),
          mScalingY(SCREENSIZEY / -(minY - maxY)),
          mMinX(minX),
          mMinY(minY) {

    namedWindow("mouse kalman", 1);
}


void CvOutput::writeEntities(const vector<PerceivedEntity> &entities) {

    for (auto &entity : entities) {
        adaptScreen(entity);
        Point myEntity(rescaleX(entity.position.x), rescaleY(entity.position.y));


        cv::line(img, Point(rescaleX(entity.position.x), rescaleY(entity.position.y)),
                 Point(rescaleX(entity.position.x + entity.velocity.x * 10),
                       rescaleY(entity.position.y + entity.velocity.y * 10)), Scalar(255, 255, 100), 2, CV_AA, 0);
        drawCross(myEntity, Scalar(0, entity.probability * 255, 255 - entity.probability * 255), 8);
        cv::putText(img, to_string(entity.ID), Point(rescaleX(entity.position.x), rescaleY(entity.position.y) - 10),
                    FONT_HERSHEY_COMPLEX, 1, 255);
        cv::putText(img, entity.name, Point(rescaleX(entity.position.x) + 10, rescaleY(entity.position.y)),
                    FONT_HERSHEY_COMPLEX, 0.5, 255);
    }
    cv::putText(img, "entities = " + to_string(entities.size()), Point(20, 20), FONT_HERSHEY_COMPLEX, 1, 255);

    imshow("mouse kalman", img);
    img = Scalar::all(0);
    waitKey(10);
}

void CvOutput::writePerceptions(const vector<PerceivedEntity> &entities) {
    for (auto &entity : entities) {
        adaptScreen(entity);
        Point myEntity(rescaleX(entity.position.x), rescaleY(entity.position.y));
        drawX(myEntity, Scalar(120, 0, 255), 6);
    }
    cv::putText(img, "perceptions = " + to_string(entities.size()), Point(20, 50), FONT_HERSHEY_COMPLEX, 1, 255);
}

void CvOutput::adaptScreen(const PerceivedEntity &entity) {
    try {
        mMinX = min(float(entity.position.x) - 20 / mScalingX, mMinX);
        mMinY = min(float(entity.position.y) - 20 / mScalingY, mMinY);
        mScalingX = min(SCREENSIZEX / float(entity.position.x - mMinX + 20 / mScalingX), mScalingX);
        mScalingY = min(SCREENSIZEY / float(entity.position.y - mMinY + 20 / mScalingY), mScalingY);
    } catch (cv::Exception) {
        mMinX = 0;
        mMinY = 0;
        mScalingX = 10;
        mScalingY = 10;
    }
}

CvOutput::~CvOutput() = default;
