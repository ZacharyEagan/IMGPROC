#include <cstdlib>
#include <cstdio>
#include <iostream>

#include <opencv2/opencv.hpp>


using namespace std;
using namespace cv;

int main(int argc, char **argv) {

    vector<vector<Point>> contours;
    vector<vector<Point>> aprox;

    vector<Vec4i> hierarchy;

    Mat o_img, hsv, thrshld, fin;
    bool success;
    int lowH = 52;
    int lowS = 31;
    int lowV = 68;
    int highH = 114;
    int highS = 255;
    int highV = 255;
    int thresh = 100;
    long unsigned int *fuck;
    Scalar *dis;
    VideoCapture cap(0);
    if (!cap.isOpened())
        return -1;

    namedWindow("Origional", CV_WINDOW_AUTOSIZE);
    //namedWindow("HSV", CV_WINDOW_AUTOSIZE);
    namedWindow("threshholded", CV_WINDOW_AUTOSIZE);
    namedWindow("Canny",CV_WINDOW_AUTOSIZE);
    namedWindow("Final", CV_WINDOW_AUTOSIZE);


    cvCreateTrackbar("LowH","Origional", &lowH, 179);
    cvCreateTrackbar("lowS","Origional", &lowS, 179);
    cvCreateTrackbar("LowV","Origional", &lowV, 255);
    cvCreateTrackbar("highH","Origional", &highH, 255);
    cvCreateTrackbar("highs","Origional", &highS, 255);
    cvCreateTrackbar("highv","Origional", &highV, 255);
    cvCreateTrackbar("Canny Thresh", "Origional", &thresh, 255);
    while (1) {
        success = cap.read(o_img);
        if (success) {
            
            cvtColor(o_img, hsv, COLOR_BGR2HSV); //why?
            //imshow("HSV",hsv);
            inRange(hsv, Scalar(lowH,lowS,lowV),
                           Scalar(highH,highS,highV), thrshld);
            imshow("threshholded",thrshld);               
            
            erode(thrshld,fin,getStructuringElement(MORPH_ELLIPSE,
                      Size(5,5)));
            

            dilate(fin,fin,getStructuringElement(MORPH_ELLIPSE,
                        Size(5,5)));

            dilate(fin,fin,getStructuringElement(MORPH_ELLIPSE,
                      Size(5,5)));
            erode(fin,fin,getStructuringElement(MORPH_ELLIPSE,
                      Size(5,5)));


            erode(fin,fin,getStructuringElement(MORPH_ELLIPSE,
                      Size(2,2)));

            dilate(fin,fin,getStructuringElement(MORPH_ELLIPSE,
                      Size(2,2)));


            dilate(fin,fin,getStructuringElement(MORPH_ELLIPSE,
                      Size(2,2)));
            erode(fin,fin,getStructuringElement(MORPH_ELLIPSE,
                      Size(2,2)));

/*
            erode(fin,fin,getStructuringElement(MORPH_ELLIPSE,
                      Size(1,1)));

            dilate(fin,fin,getStructuringElement(MORPH_ELLIPSE,
                      Size(1,1)));


            dilate(fin,fin,getStructuringElement(MORPH_ELLIPSE,
                      Size(1,1)));
            erode(fin,fin,getStructuringElement(MORPH_ELLIPSE,
                      Size(1,1)));
*/
/*            CvSeq *contours;
            CvSeq *result;
            CvMemStorage *store = cvCreateMemStorage(0);
  */          //IPLImage *imgGrayScale = cvCreateImage(cvGetSize(fin),8,1);
            
            Canny(fin, fin, thresh, thresh*2, 3);
            imshow("Canny", fin);
           findContours(fin, contours, hierarchy, CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));
            for (int i = 0; i < contours.size(); i++) {       
                approxPolyDP(contours[i], aprox, 2.2, true);
                if (aprox[i].size() == 3 ) {
                
                 
                int npt[] = {3};
                Point pt[3] = {Point(5,3), Point(3,6), Point(6,5)};
                const Point *ppt[1] = {pt};
              fillPoly(fin,ppt , npt, 1,Scalar(50, 60, 70) , 8); //,0,Point(0,0)
            
                }
            }
            imshow("Final",fin);
            imshow("Origional", o_img);

            if (waitKey(30) == 27)  //esc key
                break;
         }

    }
    return 0;
}
