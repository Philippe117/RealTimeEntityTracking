#include "opencv2/highgui/highgui.hpp"
#include "PerceivedEntity.h"
#include <iostream>

#define drawCross( center, color, d ) \
line( img, Point( center.x - d, center.y - d ), Point( center.x + d, center.y + d ), color, 2, CV_AA, 0); \
line( img, Point( center.x + d, center.y - d ), Point( center.x - d, center.y + d ), color, 2, CV_AA, 0 )

using namespace cv;
using namespace std;

Point mousePos;

void mouseCallback(int event, int x, int y, int flags, void* userdata) {
    if ( event == EVENT_MOUSEMOVE ) {
        mousePos.x = x;
        mousePos.y = y;
    }
}

int main( )
{
    vector<PerceivedEntity> entities;

    sara_msgs::Entity mouse;


// Image to show mouse tracking
    RNG rng;
    Mat img(600, 800, CV_8UC3);
    namedWindow("mouse kalman", 1);
    setMouseCallback("mouse kalman", mouseCallback, NULL);

    while(1)
    {

        // Get mouse point
        mouse.position.x = mousePos.x+rng.gaussian(20.f);
        mouse.position.y = mousePos.y+rng.gaussian(20.f);

        for (auto& entity : entities) {
            entity.update(ros::Duration(0));
        }



        PerceivedEntity *closest{nullptr};
        float closestDistance{180.f};
        for (auto& entity : entities){
            float diff{entity.compareWith(mouse)};
            if (diff < closestDistance){
                closest = &entity;
                closestDistance = diff;
            }
        }
        if (closest){
            closest->mergeOnto(mouse);
        } else {
            entities.push_back(PerceivedEntity(mouse.position.x, mouse.position.y, mouse.position.z));
        }


        imshow("mouse kalman", img);
        img = Scalar::all(0);


        for (auto& entity : entities) {
            Point myEntity(entity.position.x, entity.position.y);
            drawCross( myEntity, Scalar(255,255,255), 5 );
        }

        Point measPt(mouse.position.x, mouse.position.y);
        drawCross( measPt, Scalar(0,0,255), 5 );


        waitKey(20);
    }

    return 0;
}
