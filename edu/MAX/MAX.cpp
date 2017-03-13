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

void Max(Mat image) {
    int chanel, val, temp;
    Mat colors[3];
    Vec3b pixel;

    for(int i = 0; i < image.rows; i++) {
        for(int j = 0; j < image.cols; j++) {
            pixel = image.at<Vec3b>(i,j);
            chanel = 0;
            if(pixel.val[0] > pixel.val[1]) {
                pixel.val[1] = 0;
                chanel = 0;
            } else {
                if(pixel.val[1] > pixel.val[0])
                    pixel.val[0] = 0;
                    chanel = 1;
            }
            if (chanel == 1) {
                if(pixel.val[2] > pixel.val[1]) {
                    pixel.val[1] = 0;
                } else { 
                    if(pixel.val[1] > pixel.val[2]) {
                        pixel.val[2] = 0;
                    }
                }
            } else {
                if(pixel.val[2] > pixel.val[0]) {
                    pixel.val[0] = 0;
                } else { 
                    if(pixel.val[0] > pixel.val[2]) {
                        pixel.val[2] = 0;
                    }
                }
           }     
           image.at<Vec3b>(i,j) = pixel;
        }
    }       
}
   

int main (int argc, char** argv) {
    Mat image;
    Mat colors[3];
        
    namedWindow("Display",1);
    namedWindow("Red",1);
    namedWindow("Gre",1);
    namedWindow("Blu",1);



    VideoCapture cap(0);

    if (!cap.isOpened()) {
        fprintf(stderr, "Error: Unable to open video stream\n");
        return -1;
    }


////    cap.set(0, 1);
////    cap.set(1, 1);
////    cap.set(2, 1);
    cap.set(3, .5);
    cap.set(4, .5);
    cap.set(5, .5);
////    cap.set(6, 1);
////    cap.set(7, 1);
////    cap.set(8, 1);
////    cap.set(9, 1);
    cap.set(10, .5);
    cap.set(11, .5);
    cap.set(12, .5);
    cap.set(13, .5);
    cap.set(14, .5);
////    cap.set(15, 1);
////    cap.set(16, 1);
////    cap.set(17, 1);
////    cap.set(18, 1);

    while(1) {
        cap >> image; 
        Max(image);
        split(image, colors);

        imshow("Display", image);
        imshow("Red",colors[2]);
        imshow("Gre",colors[1]);
        imshow("Blu",colors[0]);

        if (waitKey(30) == 27)
            break;
    }
    return 0;
}

