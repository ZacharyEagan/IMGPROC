#ifndef CONSTANT_DEF
#define CONSTANT_DEF

#include <opencv2/opencv.hpp>
#include <mutex>
#include <pthread.h>

/* Cammera Communications */
const char *DefaultCamera = "/dev/video1";
int OCV_Camera = 0;
const double FrameRate = 29.411;

/* Arduino Serial Communications */ 
const int Buff_Max = 64;



/* Global Threading Variables */

/* Global Camera vars */
int CamInitialised = 0;
cv::Mat Img;
std::mutex ImgLock;
int PhotoSync = 0;
double FrameDelay = 0.01;

/* Global Array vars */
int Num_Env = 8;
int ArrayInitialised = 0;
int Env;
std::mutex EnvLock;
int EnvSync = 0;

/* Global Control var */
int Shutdown = 0;


#endif
