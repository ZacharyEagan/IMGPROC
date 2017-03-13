#include <cstdlib>
#include <cstdio>
#include <iostream>

#include <opencv2/opencv.hpp>


using namespace std;
using namespace cv;



void AV(Mat raw, Mat image) {
    uint16_t pixel;
        for(int i = 0; i < raw.rows; i++) {
           for(int j = 0; j < raw.cols; j++) {
              pixel = (image.at<uchar>(i,j) + raw.at<uchar>(i,j)) / 2 ;
              image.at<uchar>(i,j) = (pixel > 0xFF ? 0xFF : pixel);
           }
    }
}



int main(int argc, char **argv) {

    vector<vector<Point>> contours;
    vector<Point> aprox;

    vector<Vec4i> hierarchy;

    Mat o_img, hsv, hsv2, thrshld, fin;
    Mat colors[3];
    bool success;
    bool first = true;
    int lowH = 52;
    int lowS = 31;
    int lowV = 68;
    int highH = 114;
    int highS = 255;
    int highV = 255;
    int thresh = 100;
    int ed_slid = 4;
    long unsigned int *fuck;
    Scalar *dis;
    VideoCapture cap(0);
    if (!cap.isOpened())
        return -1;

    namedWindow("Origional", CV_WINDOW_AUTOSIZE);
    namedWindow("threshholded", CV_WINDOW_AUTOSIZE);
    namedWindow("Final", CV_WINDOW_AUTOSIZE);
    namedWindow("OrigionalS", CV_WINDOW_AUTOSIZE);
          cvCreateTrackbar("LowH","OrigionalS", &lowH, 179);
          cvCreateTrackbar("lowS","OrigionalS", &lowS, 179);
          cvCreateTrackbar("LowV","OrigionalS", &lowV, 255);
          cvCreateTrackbar("highH","OrigionalS", &highH, 255);
          cvCreateTrackbar("highs","OrigionalS", &highS, 255);
          cvCreateTrackbar("highv","OrigionalS", &highV, 255);
          cvCreateTrackbar("Canny Thresh", "OrigionalS", &thresh, 255);
          cvCreateTrackbar("ED slider", "OrigionalS", &ed_slid, 10);



    while (1) {
        success = cap.read(o_img);
        if (success) {
            split(o_img, colors);
            hsv = colors[0];
        //    cvtColor(hsv, colors[2], COLOR_GRE2BGR);
        //   cvtColor(o_img, hsv, COLOR_BGR2HSV); //why?
            
            inRange(hsv, Scalar(lowH,lowS,lowV),
                           Scalar(highH,highS,highV), thrshld);
            if(first) {
                hsv.copyTo(hsv2);  
                first = false;
            }
            AV(hsv2, hsv);
            hsv.copyTo(hsv2);

            if(ed_slid < 3) ed_slid = 3;
         erode(thrshld,thrshld,getStructuringElement(MORPH_ELLIPSE,
            Size(ed_slid,ed_slid)));
        dilate(thrshld,thrshld,getStructuringElement(MORPH_ELLIPSE, 
            Size(ed_slid,ed_slid)));
        dilate(thrshld,thrshld,getStructuringElement(MORPH_ELLIPSE, 
            Size(ed_slid,ed_slid)));
         erode(thrshld,thrshld,getStructuringElement(MORPH_ELLIPSE, 
            Size(ed_slid,ed_slid)));

         erode(thrshld,thrshld,getStructuringElement(MORPH_ELLIPSE, 
            Size(ed_slid-1,ed_slid-1)));
        dilate(thrshld,thrshld,getStructuringElement(MORPH_ELLIPSE, 
            Size(ed_slid-1,ed_slid-1)));
        dilate(thrshld,thrshld,getStructuringElement(MORPH_ELLIPSE, 
            Size(ed_slid-1,ed_slid-1)));
         erode(thrshld,thrshld,getStructuringElement(MORPH_ELLIPSE, 
            Size(ed_slid-1,ed_slid-1)));

         erode(thrshld,thrshld,getStructuringElement(MORPH_ELLIPSE,
            Size(ed_slid-2,ed_slid-2)));
        dilate(thrshld,thrshld,getStructuringElement(MORPH_ELLIPSE, 
            Size(ed_slid-2,ed_slid-2)));
        dilate(thrshld,thrshld,getStructuringElement(MORPH_ELLIPSE, 
            Size(ed_slid-2,ed_slid-2)));
         erode(thrshld,thrshld,getStructuringElement(MORPH_ELLIPSE, 
            Size(ed_slid-2,ed_slid-2)));
            
            Canny(thrshld, thrshld, thresh, thresh*2, 3);


            imshow("threshholded",thrshld);               



            thrshld.copyTo(fin);



           findContours(thrshld, contours, hierarchy,
            CV_RETR_TREE,CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));

            for (int i = 0; i < contours.size(); i++) {
                approxPolyDP(contours[i], aprox,
                    arcLength(Mat(contours[i]), true)*0.04, true);
                drawContours(fin, contours, i, Scalar(255,0,0), CV_FILLED, 0);
                if(aprox.size() == 3) {
                //   fillConvexPoly(o_img, ((Point *)aprox), 3, Scalar(0,0,255), CV_FILLED, 0);
                  drawContours(o_img, contours, i, 
                    Scalar(255,0,0), CV_FILLED, 0);
                } /*else {
                    if (aprox.size() == 4) {
                        drawContours(o_img, contours, i,
                            Scalar(0,255,0), CV_FILLED, 0);
                    } else {
                        
                        cout << aprox.size() << endl;
                        drawContours(o_img, contours, i, 
                            Scalar(0,0,255), CV_FILLED, 0);
                    }
                //   fillConvexPoly(o_img, aprox, aprox.size(), Scalar(255,0,0), CV_FILLED);
                }*/
            }
                
/*
            CvSeq *contours;
            CvSeq *result;
            CvMemStorage *store = cvCreateMemStorage(0);
           //IPLImage *imgGrayScale = cvCreateImage(cvGetSize(fin),8,1);
            

            for (int i = 0; i < contours.size(); i++) {       
                approxPolyDP(contours[i], aprox, 2.2, true);
                if (aprox[i].size() == 3 ) {
                
                 
                int npt[] = {3};
                Point pt[3] = {Point(5,3), Point(3,6), Point(6,5)};
                const Point *ppt[1] = {pt};
              fillPoly(fin,ppt , npt, 1,Scalar(50, 60, 70) , 8); //,0,Point(0,0)
  */          
                //}
//            }
            imshow("Final",fin);
            imshow("Origional", o_img);

            if (waitKey(30) == 27)  //esc key
                break;
         }

    }
    return 0;
}
