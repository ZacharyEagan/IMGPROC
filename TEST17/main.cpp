#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <pthread.h>
#include <opencv2/opencv.hpp>
#include <mutex>


#include "Settings.h"
#include "Serial.h"
#include "Constants.h"


#define LED_ENV 8
#define CAM_BUFF_SIZE 5

using namespace std;
using namespace cv;

/* Global Camera vars */
int CamInitialised = 0;
Mat Img;
mutex ImgLock;
int PhotoSync = 0;

/* Global Array vars */
int ArrayInitialised = 0;
int Env;
mutex EnvLock;
int EnvSync = 0;

/* Global Control var */
int Shutdown = 0;


int INIT_CAM (VideoCapture *cap) {
    CamSet cs;

    /* Access low level variables for camera */
    INIT_CAM_libv4l2((char *)DefaultCamera);

    /* Open opencv Capture */
    if (!cap->open(1)) {
        printf("Cannot Access Cammera\n");
        return -1;
    }

    /* Get opencv settings */
    INIT_CAM_GET(*cap, &cs);
    /* ajust opencv settings to match template */


    /* Set opencv settings */
    INIT_CAM_SET(*cap, &cs);

    return 0;
}

void *camThread(void *arg) {

    VideoCapture cap;
    Mat tosser;

    printf("camThread: Top\n");
    
    if (!INIT_CAM(&cap)) {
        printf("Camera Inittialised\n");
        CamInitialised = 1;

        while (1) {
            if (Shutdown) {
                printf("Camera Powering Down\n");
                break;
            }

            if (ImgLock.try_lock()) {
                printf("Taking photo\n");
                cap >> Img;
                PhotoSync = 1;
                printf("Photo Taken\n");
                ImgLock.unlock();
                pthread_yield();
            } else {
                cap >> tosser;
            }
        }
   }

   pthread_exit(NULL); 
}

int INIT_ARRAY(int *fd_array) {
    if ((*fd_array = Find_Array_Coms()) <= 0)
        return -1;
    if (INIT_Port(*fd_array))
        return -2;
    if (INIT_Array(*fd_array))
        return -3;
    return 0;
}


void *arrayThread(void *arg) {
    int fd_array;
    int env;

    printf("arrayThread: Top\n");

    /* Initialise LED Array */
    if (!INIT_ARRAY(&fd_array)) {
        ArrayInitialised = 1;
        env = 0;

        while(1) {
            if (Shutdown) {
                printf("Array, Powering Down\n");
                break;
            }

            EnvLock.lock();
            while (Env != env) {
                env = Array_Next(fd_array);
            }
            EnvSync = 1;
            EnvLock.unlock();
            pthread_yield();
        }
    }

    pthread_exit(NULL);
}


int main() {

    
    Mat image[LED_ENV];
    

    char window_name[] = "Display0";
    int env;
    int array_resp;
    int dupe;

    pthread_t tid[2];

    ImgLock.lock();
    EnvLock.lock();

    /* initialise threads */
    if (pthread_create(&(tid[0]), NULL, &arrayThread, NULL)) {
        printf("Threading error: array\n");
        return -5;
    }
    
    if (pthread_create(&(tid[0]), NULL, &camThread, NULL)) {
        printf("Threading error: cam\n");
        return -5;
    }

    while(!CamInitialised) ;
    while(!ArrayInitialised) ;
    printf("Initialisation Complete\n");

    /* Initialise windows */
    for (env = 0; env < LED_ENV; env++) {
        window_name[7] = 48 + env;
        namedWindow (window_name, 1);
    }

    while (1) {
        /* get image for current environment */
        PhotoSync = 0;
        ImgLock.unlock();
        while (!PhotoSync) {
            pthread_yield();
        }
        while (!ImgLock.try_lock()) ;
        imshow("Display0", Img);
        
        /* change to next environment */
        Env++;
        Env %= LED_ENV;
        EnvSync = 0;
        EnvLock.unlock();
        while (!EnvSync) {
            pthread_yield();
        }
        while (!EnvLock.try_lock()) ;


        if(waitKey(30) == 27) 
            break;
    }

    Shutdown = 1;
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);

    return 0;
}




