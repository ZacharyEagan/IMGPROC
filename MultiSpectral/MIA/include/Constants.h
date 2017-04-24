#include <opencv2/opencv.hpp>
#include <mutex>
#include <pthread.h>

#define NUM_ENV 7

using namespace std;
using namespace cv;
/* Cammera Communications */
extern char *DefaultCamera;
extern int OCV_Camera;
extern const double FrameRate;

/* Arduino Serial Communications */ 
extern const int Buff_Max;


/*Global Threading Variables*/

/* Global Array vars */
extern int Num_Env;
extern int ArrayInitialised;
extern int Array_Reset;
extern int Env;
extern std::mutex EnvLock;
extern int EnvSync;

/* Global Camera vars */
extern int CamInitialised;
extern Mat Img[NUM_ENV];
extern Mat Ref_Img;
extern std::mutex ImgLock;
extern int PhotoSync[NUM_ENV];
extern double FrameDelay;

/* Global Control var */
extern int Shutdown;
extern int Intermix_Frames;

