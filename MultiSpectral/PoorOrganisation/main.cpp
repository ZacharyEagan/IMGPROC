#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <opencv2/opencv.hpp>



#include "Settings.h"
#include "Serial.h"
#include "Constants.h"


#define LED_ENV 8
#define CAM_BUFF_SIZE 5
using namespace std;
using namespace cv;

int Array_Zero(int fd) {
    char buff[Buff_Max];

    if (!fd) {
        printf("Array_Zero: port not found\n");
        return -1;
    }
    write_Port (fd, (char *) "I");
    if (waitSelect(fd, 1000000) == 0) {
        printf("Array_Zero: Array took too long\n");
        return -2;
    }

    if ((readln_Port(fd, buff, Buff_Max)) > 0)
        if ((buff[0] == '0'))
            return 0;

    return -3;
}


int Array_Next(int fd) {
    char buff[Buff_Max];

    if (!fd) {
        printf("Array_Zero: port not found\n");
        return -1;
    }
    write_Port (fd, (char *) "N");
    if (waitSelect(fd, 1000000) == 0) {
        printf("Array_Zero: Array took too long\n");
        return -2;
    }

    if ((readln_Port(fd, buff, Buff_Max)) > 0)
            return ((int)buff[0] - 48);

    return -3;
}

/* subbtracts in from out */
void DIFF_RGB(Mat out, Mat in) {
    int i = 0;
    int j = 0;
    int val1, val2;

    for (i = 0; i < out.rows; i++) {
        for (j = 0; j < out.cols; j++) {
            val1 = out.at<Vec3b>(i,j).val[0];
            val2 = in.at<Vec3b>(i,j).val[0];

            if (val1 > val2) 
                out.at<Vec3b>(i,j).val[0] = val1 - val2;
            else 
                out.at<Vec3b>(i,j).val[0] = 0;
                
            val1 = out.at<Vec3b>(i,j).val[1];
            val2 = in.at<Vec3b>(i,j).val[1];

            if (val1 > val2) 
                out.at<Vec3b>(i,j).val[1] = val1 - val2;
            else 
                out.at<Vec3b>(i,j).val[1] = 0;

            val1 = out.at<Vec3b>(i,j).val[2];
            val2 = in.at<Vec3b>(i,j).val[2];

            if (val1 > val2) 
                out.at<Vec3b>(i,j).val[2] = val1 - val2;
            else 
                out.at<Vec3b>(i,j).val[2] = 0;
        }
    }

}

void MULTIPLY(Mat io, int mult) {
    int i = 0;
    int j = 0;

    for (i = 0; i < io.rows; i++) {
        for (j = 0; j < io.cols; j++) {
            if ((io.at<Vec3b>(i,j).val[0] *= mult) > 512)
                io.at<Vec3b>(i,j).val[0] = 512;
            if ((io.at<Vec3b>(i,j).val[1] *= mult) > 512)
                io.at<Vec3b>(i,j).val[1] = 512;
            if ((io.at<Vec3b>(i,j).val[2] *= mult) > 512)
                io.at<Vec3b>(i,j).val[2] = 512;
        }
    }
    
}


int main() {
    int fd_array;
    
    Mat image[LED_ENV];
    
    CamSet cs;
    VideoCapture cap;
    char window_name[] = "Display0";
    int env;
    int array_resp;
    int dupe;

    /* Initialise LED Array */
    if ((fd_array = Find_Array_Coms()) <= 0)
        return -1;
    if (INIT_Port(fd_array))
        return -2;
    if (INIT_Array(fd_array))
        return -3;
    printf("LED Array Initialised\n");    


    /* Initialise Camera */
    INIT_CAM_libv4l2((char *)DefaultCamera);
    if (!cap.open(1)) {
        printf("Cannot Access Cammera\n");
        return -4;
    }
    INIT_CAM_GET(cap, &cs);
    INIT_CAM_SET(cap, &cs);
    printf("Camera Inittialised\n");
    printf("Camera frame-rate = %f\n",cs.FPS);

    /* Initialise windows */
    for (env = 0; env < LED_ENV; env++) {
        window_name[7] = 48 + env;
        namedWindow (window_name, 1);
    }

    while (1) {
        while (Array_Zero(fd_array)) 
         ;

        for (env = 0; env < LED_ENV; env++) {
            
            //if(waitKey(10) == 27) 
            //    break;
            for (dupe = 0; dupe < CAM_BUFF_SIZE; dupe++)
                cap >> image[env];

            //if(waitKey(20) == 27) 
              //  break;

          if((array_resp = Array_Next(fd_array)) != (env + 1)%LED_ENV)
                printf("main: env=%d\tresp=%d\n", env, array_resp);
            
            //sleep (1);
        }
        
        for (env = 0; env < LED_ENV; env++) {
            window_name[7] = 48 + env;
            
            DIFF_RGB(image[env], image[LED_ENV - 1]);
            MULTIPLY(image[env], 100);
            imshow(window_name, image[env]);
        }

        if (waitKey(5) == 27)
            break;
    }

    return 0;
}




