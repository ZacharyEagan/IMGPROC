#include "Thread.h"
#include "ArrayComs.h"
#include "CamSettings.h"
#include "Constants.h"


using namespace std;
using namespace cv;

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
    }

    pthread_exit(NULL);
}

