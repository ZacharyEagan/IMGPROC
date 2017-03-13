#include <iostream>
#include <cstdlib>
#include <opencv2/opencv.hpp>

#define FADE 1.2
#define FRAMES 16 
#define DVAL 8 
using namespace cv;

void HalfLifeFilter(Mat raw, Mat image) {
    uint16_t pixel;
    for(int i = 0; i < raw.rows; i++) {
        for(int j = 0; j < raw.cols; j++) {
           pixel = (image.at<uchar>(i,j) / FADE + raw.at<uchar>(i,j)) / 2 ;
           image.at<uchar>(i,j) = (pixel > 0xFF ? 0xFF : pixel);
        }
    }       
}

void Diff(Mat out, Mat in1, Mat in2, uint8_t uncert) {
    uint16_t pixel;
    for(int i = 0; i < in1.rows; i++) {
        for(int j = 0; j < in1.cols; j++) {
           pixel = (in1.at<uchar>(i,j) + in2.at<uchar>(i,j)) >> 1;
           out.at<uchar>(i,j) = (pixel > (in1.at<uchar>(i,j) + uncert) || 
                    pixel < (in1.at<uchar>(i,j) - uncert)) ? in2.at<uchar>(i,j)
                    : 0;
        }
    }       
    
}

int main (int argc, char** argv) {
    Mat raw, rawL, image;
        
    namedWindow("Display",1);

    VideoCapture cap(0);

    if (!cap.isOpened()) {
        fprintf(stderr, "Error: Unable to open video stream\n");
        return -1;
    }

    cap >> rawL;
    cvtColor(rawL, rawL, CV_BGR2GRAY);
    image = rawL;
    while(1) {

        cap >> raw;
        cvtColor(raw, raw, CV_BGR2GRAY);
        Diff(image, rawL, raw, DVAL);
        rawL = raw;

        imshow("Display", image);
        if(waitKey(30) == 27) 
            break;
    }
    return 0;
}

