#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;




void bLevel(Mat img) {

    uint32_t min = 1024;
    uint32_t max = 0;

    if (img.isContinuous()) {
        //cout << "img is cont" << endl;
        for (int i = 0; i < img.rows; i++) {
            for (int j = 0; j < img.cols; j++) {
                if (min > img.data[i*img.cols + j])
                    min = img.data[i*img.cols + j];
                if (max < img.data[i*img.cols + j])
                    max = img.data[i*img.cols + j];
            }
        }
        cout << "Min: "<< min << "   Max: " << max << endl;
        for (int i = 0; i < img.rows; i++) {
            for (int j = 0; j < img.cols; j++) {
                img.data[i*img.cols + j] = ((img.data[i*img.cols + j] - min)); 
                                        // *((double) (((double)max) / 256)));
            }
        }
    }

}

int main(int argc, char **argv) {
    VideoCapture cap(0);
    if(!cap.isOpened())
        return -1;

    Mat o_img, l_img;

    namedWindow("Ori", 1);
    namedWindow("B_adj", 1);

    while (1) {
        cap >> o_img;
        
        o_img.copyTo(l_img);
        
        cvtColor(o_img, o_img, CV_BGR2GRAY);
        cvtColor(l_img, l_img, CV_BGR2GRAY);        
        bLevel(l_img);

        imshow("Ori",o_img);
        imshow("B_adj",l_img);

        if (waitKey(30) == 27)
            break;
    }

    return 0;
}
