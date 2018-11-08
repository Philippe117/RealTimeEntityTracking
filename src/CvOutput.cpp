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

CvOutput::CvOutput() : img(600, 800, CV_8UC3) {

    namedWindow("mouse kalman", 1);
}


void CvOutput::write(const vector<Entity>& entities) {
    imshow("mouse kalman", img);
    img = Scalar::all(0);

    for (auto& entity : entities) {
        Point myEntity(entity.position.x, entity.position.y);
        drawCross( myEntity, Scalar(255,255,255), 5 );
        cv::putText(img, to_string(entity.ID), Point(entity.position.x, entity.position.y), FONT_HERSHEY_COMPLEX, 1, 255);
    }
    cv::putText(img, "entities = " + to_string(entities.size()), Point(20, 20), FONT_HERSHEY_COMPLEX, 1, 255);

    waitKey(10);
}

CvOutput::~CvOutput() = default;
