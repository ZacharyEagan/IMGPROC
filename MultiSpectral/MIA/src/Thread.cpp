#include "Thread.h"
#include "ArrayComs.h"
#include "CamSettings.h"
#include "Constants.h"
#include <time.h>

using namespace std;
using namespace cv;

/*
void *camThread(void *arg) {

    VideoCapture cap;
    Mat tosser;

    printf("camThread: Top\n");

    if (!INIT_CAM(&cap)) {
        printf("camThread: Camera Inittialised\n");
        CamInitialised = 1;

        while (1) {
            if (Shutdown) {
                printf("camThread: Camera Powering Down\n");
                break;
            }

            if (ImgLock.try_lock()) {
                printf("camThread: Taking photo\n");
                cap >> Img;
                PhotoSync = 1;
                printf("camThread: Photo Taken\n");
                ImgLock.unlock();
                pthread_yield();
            } else {
                cap >> tosser;
            }
        }
   }

   pthread_exit(NULL);
}
*/
/*
void *arrayThread(void *arg) {
    int fd_array;
    int env;

    printf("arrayThread: Top\n");

    // Initialise LED Array 
    while (INIT_ARRAY(&fd_array)) ;
    printf("arrayThread: fd_array initialised\n");

    ArrayInitialised = 1;   
    env = 0;

    while(1) {
        if (Shutdown) {
            printf("arrayThread: Array, Powering Down\n");
            break;
        }

        while(!EnvLock.try_lock()) {
            pthread_yield();
        }
        while (Env != env) {
            env = Array_Next(fd_array);
        }
        EnvSync = 1;
        EnvLock.unlock();
        pthread_yield();
    }

    pthread_exit(NULL);
}
*/


void *cameraArrayThread(void *arg) {
    /* Array variables */
    int fd_array;
    int env = 0;
    int dev;

    /* Camera Variables */
    VideoCapture cap;
    Mat tosser;

    /* Timing Variables */
    clock_t time;
    double elapsed;
    

    printf("cameraArrayThread: Top\n");

    /* Initialise LED Array */
    printf("cameraArrayThread: initialising Array\n");
    while (INIT_ARRAY(&fd_array)) ;
    ArrayInitialised = 1;   
    env = 0;
    printf("cameraArrayThread: Array initialised\n");

    /* Initialise Camera */
    printf("cameraArrayThread: initialising Camera\n");
    while(INIT_CAM(&cap)) ;
    CamInitialised = 1;
    printf("cameraArrayThread: Camera Inittialised\n");
   
    for (env = 0; env < Num_Env; env++) { 
        cap >> Img[env];
    }
    cap >> Ref_Img;
    env = 0; 
    
    /* Check for shutdown command and stop thread */
    while(!Shutdown) {
        
        /* Save Frame to perminant location */
        /* check lock so don't change image while main is displaying */
        if (ImgLock.try_lock()) {
            /* copy current frame to Appropriate env mat */
            if (Array_Reset) {
                tosser.copyTo(Img[Env]);
                PhotoSync[Env] = 1;
            } else {
                tosser.copyTo(Ref_Img);
            } 
            /* unlock the image for access to other threads */
            ImgLock.unlock();
            /* yield thread here to use before Env changes */
            pthread_yield();

            /* oscilate between environments and referance frame */
            if (Array_Reset >= Intermix_Frames) {
                while ((dev = Array_Zero(fd_array))) ;
                
                while (Array_Refresh(fd_array) != Num_Env) ;
                Array_Reset = 0;
            } else {
                if (EnvLock.try_lock()) {
                    Env++;
                    Env %= Num_Env;

                    while ((env = Array_Next(fd_array)) != Env); 
                    while (Array_Refresh(fd_array) != Env) ;
                    EnvLock.unlock();
                }
                    Array_Reset++;
            }

        } 

        /* empty camera buffer, last image should take longer */
        /* image which will be used as it is the most recent */
        do {
            cap >> tosser;
            time = clock();
            cap >> tosser;
            elapsed = ((double)(clock() - time) / CLOCKS_PER_SEC);
        }while (elapsed < FrameDelay);


    

    }

    printf("arrayThread: Array, Powering Down\n");
    pthread_exit(NULL);
}

