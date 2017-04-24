#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <pthread.h>
#include <opencv2/opencv.hpp>
#include <mutex>
#include <time.h>
#include <unistd.h>

#include "CamSettings.h"
#include "ArrayComs.h"
#include "Constants.h"
#include "Proc.hpp"
#include "Thread.h"

#define CAM_BUFF_SIZE 5

using namespace std;
using namespace cv;

int main() {

    char window_name[] = "Display0";
    int env;
    int array_resp;
    int dupe;

    pthread_t tid[2];


    /* initialise threads */
    if (pthread_create(&(tid[0]), NULL, &cameraArrayThread, NULL)) {
        printf("main: Threading error: cameraArray\n");
        return -1;
    }
    printf("main: cameraArrayThread split, starting INIT\n");
    
    while(!(CamInitialised && ArrayInitialised)) {
        if (!CamInitialised)
            printf("main: waiting on camera INIT\n");
        if (!ArrayInitialised)
            printf("main: waiting on array INIT\n");

        pthread_yield();
    }

    printf("Initialisation Complete\n");
        
    /* Initialise windows */
    for (env = 0; env < Num_Env; env++) {
        window_name[7] = 48 + env;
        namedWindow (window_name, 1);
        PhotoSync[env] = 0;
    }
    /* window for the default env (motion detect & ambiance) */
    namedWindow("Ref", 1);

    /* give camera time to populate Img */
    while (!PhotoSync[Num_Env - 1]) {
        printf("main: waiting\n");
        sleep(1);
    }
    while (1) {
     //sleep(2); 
        /* Display image in window corresponding to environment */
        ImgLock.lock();
        if (Array_Reset != 0) {    
            if (EnvLock.try_lock()) {
                for (env = 0; env < Num_Env; env++) {
                    if (PhotoSync[env]) {
                        window_name[7] = 48 + env;
                        imshow(window_name, Img[env]);
                        PhotoSync[env] = 0;
                    }
                }
                EnvLock.unlock();
            }
        } else {
            imshow("Ref", Ref_Img);
        }    
        ImgLock.unlock();
        
        if(waitKey(30) == 27) 
            break;
    }

    /* Signal threads to exit and wait to collect children */
    Shutdown = 1;
    pthread_join(tid[0], NULL);
//    pthread_join(tid[1], NULL);

    return 0;
}




