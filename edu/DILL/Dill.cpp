#include <cstdlib>
#include <cstdio>
#include <iostream>

#include <opencv2/opencv.hpp>


using namespace std;
using namespace cv;

int main(int argc, char **argv) {

    Mat o_img, d_img, e_img, hsv, thrshld, fin;
    bool success;
    int lowH = 0;
    int lowS = 0;
    int lowV = 0;
    int highH = 179;
    int highS = 255;
    int highV = 255;
    VideoCapture cap(0);
    if (!cap.isOpened())
        return -1;

    namedWindow("Origional", CV_WINDOW_AUTOSIZE);
    namedWindow("eroded", CV_WINDOW_AUTOSIZE);
    namedWindow("Dilated", CV_WINDOW_AUTOSIZE);
    namedWindow("HSV", CV_WINDOW_AUTOSIZE);
    namedWindow("threshholded", CV_WINDOW_AUTOSIZE);
    namedWindow("Final", CV_WINDOW_AUTOSIZE);


    cvCreateTrackbar("LowH","Dilated", &lowH, 179);
    cvCreateTrackbar("lowS","Dilated", &lowS, 179);
    cvCreateTrackbar("LowV","Dilated", &lowV, 255);
    cvCreateTrackbar("highH","Dilated", &highH, 255);
    cvCreateTrackbar("highs","Dilated", &highS, 255);
    cvCreateTrackbar("highv","Dilated", &highV, 255);

    while (1) {
        success = cap.read(o_img);
        if (success) {
            
            cvtColor(o_img, hsv, COLOR_BGR2HSV); //why?
            imshow("HSV",hsv);
            inRange(hsv, Scalar(lowH,lowS,lowV),
                           Scalar(highH,highS,highV), thrshld);
            imshow("threshholded",thrshld);               
            
            erode(thrshld,e_img,getStructuringElement(MORPH_ELLIPSE,
                      Size(5,5)));
            imshow("eroded",e_img);

            dilate(thrshld,d_img,getStructuringElement(MORPH_ELLIPSE,
                        Size(5,5)));

            erode(thrshld,fin,getStructuringElement(MORPH_ELLIPSE,
                      Size(5,5)));
            imshow("eroded",e_img);

            dilate(fin,fin,getStructuringElement(MORPH_ELLIPSE,
                        Size(5,5)));

            dilate(fin,fin,getStructuringElement(MORPH_ELLIPSE,
                      Size(5,5)));
            imshow("eroded",e_img);

            erode(fin,fin,getStructuringElement(MORPH_ELLIPSE,
                        Size(5,5)));
            erode(fin,fin,getStructuringElement(MORPH_ELLIPSE,
                      Size(5,5)));
            imshow("eroded",e_img);

            dilate(fin,fin,getStructuringElement(MORPH_ELLIPSE,
                        Size(5,5)));

            dilate(fin,fin,getStructuringElement(MORPH_ELLIPSE,
                      Size(5,5)));
            imshow("eroded",e_img);

            erode(fin,fin,getStructuringElement(MORPH_ELLIPSE,
                        Size(5,5)));
            imshow("Final",fin);
            imshow("Origional", o_img);
            imshow("Dilated",d_img);

            if (waitKey(30) == 27)  //esc key
                break;
         }

    }
    return 0;
}
