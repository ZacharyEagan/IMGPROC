#include "Thread.h"
#include "ArrayComs.h"
#include "CamSettings.h"
#include "Constants.h"
#include <time.h>

using namespace std;
using namespace cv;


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
   
    cap >> Ref_Img;
    
    /* Save Variables */
    /*
    char save_name[] = "Display0.avi";
    Size FrSize(Ref_Img.cols, Ref_Img.rows);
    int FrRate = 15;
    VideoWriter saveEnv[Num_Env];
    VideoWriter saveRef;

    for (env = 0; env < Num_Env; env++) { 
        save_name[7] = env + 48;
        saveEnv[env].open(save_name, 
            CV_FOURCC('M','J','P','G'), FrRate, FrSize, true);

        if (!saveEnv[env].isOpened()) {
            printf("CamArrayThread: failed open saveEnv file: %s\n", save_name);
            Shutdown = 1;
        }
        
    }
    saveRef.open("Ref.avi", 
            CV_FOURCC('M','J','P','G'), FrRate, FrSize, true);
    if (!saveRef.isOpened()) {
        printf("CamArrayThread: unable to open save file for reference\n");
        Shutdown = 1;
    }
    */
    for (env = 0; env < Num_Env; env++) { 
        cap >> Img[env];
    }
    env = 0; 
    //printf("CameraArrayThread: Post INIT\n"); 

    /* Check for shutdown command and stop thread */
    while(!Shutdown) {
        //printf("cameraArrayThread: In loop\n");
        /* Save Frame to perminant location */
        /* check lock so don't change image while main is displaying */
        if (ImgLock[0].try_lock()) {
            /* copy current frame to Appropriate env mat */
            if (Array_Reset) {
                //printf("Env: %d\n",Env);
                tosser.copyTo(Img[Env]);
                PhotoSync[Env] = 1;
              //  saveEnv[Env].write(tosser);
            } else {
                tosser.copyTo(Ref_Img);
                //printf("cameraArrayThread: wrote another ref\n");
            //    saveRef.write(tosser);
            } 
            /* unlock the image for access to other threads */
            ImgLock[0].unlock();
            /* yield thread here to use before Env changes */
            //printf("yield\n");
            pthread_yield();
            //printf("return\n");

            /* oscilate between environments and referance frame */
            if (Array_Reset >= Intermix_Frames) {
                //printf("Zeroing\n");
                while ((dev = Array_Zero(fd_array))) ;
                
                while (Array_Refresh(fd_array) != Num_Env) ;
                Array_Reset = 0;
            } else {
                if (EnvLock.try_lock()) {
                    Env++;
                    Env %= Num_Env;
                    
                    //printf("incrementing ENV\n");
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
            //printf("Taking pic\n");
            cap >> tosser;
            time = clock();
            cap >> tosser;
            elapsed = ((double)(clock() - time) / CLOCKS_PER_SEC);
        }while (elapsed < FrameDelay);


    

    }
    
    /*for (env = 0; env < Num_Env; env++) {
        saveEnv[env].release();
    }*/
    //saveRef.release();

    printf("arrayThread: Array, Powering Down\n");
    pthread_exit(NULL);
}

//Thread to continuously save the images to file
//Must not be called until after Ref_Img is filled
void *ImgSaveThread(void *arg) {
    int env;

    char *save_name;// "Display000.avi";

    Size FrSize(Ref_Img.cols, Ref_Img.rows);
    int FrRate = 15;
    VideoWriter saveEnv[Num_Env];
    VideoWriter saveRef;

    for (env = 0; env < Num_Env; env++) {
        sprintf(save_name, "Display%d.avi", env);
        saveEnv[env].open(save_name,
            CV_FOURCC('M', 'J', 'P', 'G'), FrRate, FrSize, true);

        if(!saveEnv[env].isOpened()) {
            printf("main: failed open saveEnv file: %s\n", save_name);
            Shutdown = 1;
        }
    }
    saveRef.open("Ref.avi",
        CV_FOURCC('M', 'J', 'P', 'G'), FrRate, FrSize, true);
    if (!saveRef.isOpened()) {
        printf("main: unable to open saveRef\n");
        Shutdown = 1;
    }

	while (!Shutdown) {

		for (env = 0; env < Num_Env; env++) {
        	sprintf(save_name, "Display%d.avi", env);

			while (PhotoSync[env]) {
				if (ImgLock[env].try_lock()) {
				    saveEnv[env].write(Img[env]);

					PhotoSync[env]--;
					ImgLock[env].unlock();
				} else {
					pthread_yield();
				}

			}
	    }
		while (PhotoRefSync) {
			if (ImgRefLock.try_lock()) {
				saveRef.write(Ref_Img);
				PhotoRefSync--;
				ImgRefLock.unlock();
			} else {
				pthread_yield();
			}
		}
	} ///need to remember to unlock everything when shutdown is 
	  //called otherwise could lock threads
	for (env = 0; env < Num_Env; env++) {
		saveEnv[env].release();
	}
	saveRef.release();
	pthread_exit(NULL);
}


void *ImgProcThread(void *arg) {
    int env;
    int env_count;
    while(!Shutdown) {
        env_count = 0;
        for (env = 0; env < Num_Env; env++) {
            env_count += PhotoSync[env];
        }
        if (env_count == Num_Env) {
            while(!ImgLock[0].try_lock()) ;
            //do image processing here
            printf("process\n");

            //save result to out;
            for (env = 0; env < Num_Env; env++) 
                PhotoSync[env] = 0;
            ImgLock[0].unlock();
        }

    } 

    printf("ImgProcThread, Powering Down\n");
    pthread_exit(NULL);
}







