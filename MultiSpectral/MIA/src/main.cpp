#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <pthread.h>
#include <opencv2/opencv.hpp>
#include <mutex>


#include "CamSettings.h"
#include "ArrayComs.h"
#include "Constants.h"
#include "Proc.hpp"
#include "Thread.h"

#define LED_ENV 8
#define CAM_BUFF_SIZE 5

using namespace std;
using namespace cv;

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
        window_name[7] = 48 + Env;
        imshow(window_name, Img);
        
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




