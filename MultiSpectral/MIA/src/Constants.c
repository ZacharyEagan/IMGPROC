#ifndef CONSTANT_DEF
#define CONSTANT_DEF

#include <opencv2/opencv.hpp>
#include <mutex>
#include <pthread.h>

#define NUM_ENV 16 

/* Cammera Communications */
const char *DefaultCamera = "/dev/video1";
int OCV_Camera = 0;
const double FrameRate = 29.411;

/* Arduino Serial Communications */ 
const int Buff_Max = 64;



/* Global Threading Variables */

/* Global Array vars */
int Num_Env = NUM_ENV;
int ArrayInitialised = 0;
int Array_Reset = 0;
int Env;
std::mutex EnvLock;
int EnvSync = 0;

/* Global Camera vars */
int CamInitialised = 0;
cv::Mat Img[NUM_ENV];
cv::Mat Ref_Img;
std::mutex ImgLock[NUM_ENV];
std::mutex ImgRefLock;
int PhotoSync[NUM_ENV];
int PhotoRefSync;
double FrameDelay = 0.009; //actual value was 0.01 

/* Global Control var */
int Shutdown = 0;
int Intermix_Frames = 1;

#endif
