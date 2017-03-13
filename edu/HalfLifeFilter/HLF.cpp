#include <iostream>
#include <cstdlib>
#include <opencv2/opencv.hpp>

#define FADE 1.2
#define FRAMES 16 

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

int main (int argc, char** argv) {
    Mat raw, image;
        
    namedWindow("Display",1);

    VideoCapture cap(0);

    if (!cap.isOpened()) {
        fprintf(stderr, "Error: Unable to open video stream\n");
        return -1;
    }

    cap >> image;
    cvtColor(image, image, CV_BGR2GRAY);

    while(1) {
        for (int i = 0; i < FRAMES; i++) {
            cap >> raw;
            cvtColor(raw, raw, CV_BGR2GRAY);
            HalfLifeFilter(raw, image);
        }

        imshow("Display", image);
        if (waitKey(30) == 27) 
            break;
    }
    return 0;
}

