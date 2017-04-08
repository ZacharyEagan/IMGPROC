#ifndef CAMSETTINGS_DEF
#define CAMSETTINGS_DEF

#include <opencv2/opencv.hpp>
#include <libv4l2.h>
#include <linux/videodev2.h>
#include <sys/ioctl.h>
#include <fcntl.h>


using namespace cv;

typedef struct CamSet {
    double POS_MSEC; 
    double POS_FRAMES;
    double POS_AVI_RATIO;
    double FRAME_WIDTH;
    double FRAME_HEIGHT;
    double FPS;
    double FOURCC;
    double FRAME_COUNT;
    double FORMAT;
    double MODE;
    double BRIGHTNESS;
    double CONTRAST;
    double SATURATION;
    double HUE;
    double GAIN;
    double EXPOSURE;
    double CONVERT_RGB;
    double WHITE_BALANCE_U;
    double WHITE_BALANCE_V;
    double RECTIFICATION;
    double ISO_SPEED;
    double BUFFERSIZE;
} CamSet;



int INIT_CAM_libv4l2(char *port);
void INIT_CAM_DEFAULTS(CamSet *cs);
int INIT_CAM_SET(VideoCapture cap, CamSet *cs);
void INIT_CAM_GET(VideoCapture cap, CamSet *cs);

#endif
