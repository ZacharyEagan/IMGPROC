#include <opencv2/opencv.hpp>
#include <mutex>
#include <pthread.h>

using namespace std;
using namespace cv;
/* Cammera Communications */
extern char *DefaultCamera;
extern int OCV_Camera;
extern const double FrameRate;

/* Arduino Serial Communications */ 
extern const int Buff_Max;


/*Global Variables*/

/* Global Camera vars */
extern int CamInitialised;
extern Mat Img;
extern std::mutex ImgLock;
extern int PhotoSync;

/* Global Array vars */
extern int ArrayInitialised;
extern int Env;
extern std::mutex EnvLock;
extern int EnvSync;

/* Global Control var */
extern int Shutdown;

