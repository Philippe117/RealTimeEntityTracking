//
// Created by philippe on 08/11/18.
//

#ifndef PROJECT_CVOUTPUT_H
#define PROJECT_CVOUTPUT_H

#include "EntityOutput.h"
#include <vector>
#include "sara_msgs/Entity.h"
#include "opencv2/highgui/highgui.hpp"

class CvOutput : public EntityOutput {
    cv::Mat img;
    float mScaling;
    float mCenterX;
    float mCenterY;
public:
    CvOutput(float scaling=1, float centerX=0, float centerY=0);
    ~CvOutput() override ;
    void writeEntities(const std::vector<sara_msgs::Entity>& entities) override ;
    void writePerceptions(const std::vector<sara_msgs::Entity>& entities) override ;

};


#endif //PROJECT_CVOUTPUT_H
