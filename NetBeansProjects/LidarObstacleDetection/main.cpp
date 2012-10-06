/* 
 * File:   main.cpp
 * Author: bhuvnesh
 *
 * Created on 18 September, 2012, 6:53 PM
 */

#include <cstdlib>
#include <opencv/cv.h>
#include <opencv/cvaux.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>
#include "LidarData.h"
#define MAP_X 1000
#define MAP_Y 1000

using namespace std;
using namespace ::cv;
char** clearLocalMap(char **local_map);
void displayObstaclesOnTerminal(char **localmap);
void plotMap(IplImage* mapImg,char **local_map);
/*
 * 
 */
int main() {
    char **local_map;
    IplImage *mapImg = cvCreateImage(cvSize(MAP_Y, MAP_X), IPL_DEPTH_8U, 1);
    
    int i, count = 0;
    local_map = (char**) malloc(MAP_Y * sizeof (char*));
    for (i = 0; i < MAP_Y; i++) {
        local_map[i] = (char*) malloc(MAP_X * sizeof (char));
    }

    LidarData *laser = new LidarData("ttyACM0");
    while (1) {
        local_map = clearLocalMap(local_map);
        laser->plotLaserScan(local_map);
        plotMap(mapImg, local_map);
        printf("loop count: %d \n", count++);
        //::cvWaitKey(100);
        //TODO: add appropiate sigint handler
        //TODO: add appropiate window to show obstacles.
    }
    laser->~LidarData();
    return 0;
}

char** clearLocalMap(char **local_map) {
    int i, j;
    for (i = 0; i < MAP_Y; i++) {
        for (j = 0; j < MAP_X; j++) {
            local_map[i][j] = 0;
        }
    }
    return local_map;
}

void displayObstaclesOnTerminal(char **localmap) {
    int i, j;
    for (i = 0; i < MAP_Y; i++) {
        for (j = 0; j < MAP_X; j++) {
            if (localmap[i][j] == 0)
                printf(" ");
            else
                printf("*");
        }
        printf("\n");
    }
}

void plotMap(IplImage *mapImg, char **local_map) {
    for (int i = 0; i < MAP_Y; i++) {
        uchar* ptr = (uchar *) (mapImg->imageData + i * mapImg->widthStep);
        for (int j = 0; j < MAP_X; j++) {
            if (local_map[j][MAP_Y - i - 1] == 0) {
                ptr[j] = 0;
            } else {
                ptr[j] = 200;
            }
        }
    }
    cvNamedWindow("Global Map", 0);
    cvShowImage("Global Map", mapImg);
    cvSaveImage("map.png", mapImg);
}
