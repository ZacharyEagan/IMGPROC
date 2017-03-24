#include "Proc.hpp"



/* Subtracts in from out */
/* Note NEEDS HEAVY OPTIMISATION */

void DIFF_RGB(Mat out, Mat in) {
    int i = 0;
    int j = 0;
    int val1, val2;
//    printf("A\n");
//    for (i = 200; i < out.rows - 200; i++) {
//        for (j = 200; j < out.cols - 200; j++) {
    for (i = 0; i < out.rows; i++) {
        for (j = 0; j < out.cols; j++) {
  //          printf("i = %d, j = %d\n", i, j);
            val1 = out.at<Vec3b>(i,j).val[0];
            val2 = in.at<Vec3b>(i,j).val[0];

            if (val1 > val2)
                out.at<Vec3b>(i,j).val[0] = val1 - val2;
            else
                out.at<Vec3b>(i,j).val[0] = 0;

            val1 = out.at<Vec3b>(i,j).val[1];
            val2 = in.at<Vec3b>(i,j).val[1];

            if (val1 > val2)
                out.at<Vec3b>(i,j).val[1] = val1 - val2;
            else
                out.at<Vec3b>(i,j).val[1] = 0;

            val1 = out.at<Vec3b>(i,j).val[2];
            val2 = in.at<Vec3b>(i,j).val[2];

            if (val1 > val2)
                out.at<Vec3b>(i,j).val[2] = val1 - val2;
            else
                out.at<Vec3b>(i,j).val[2] = 0;
        }
    }

}


/* Scales the image by factor of mult */
/*Also needs to be optimised for speed by working as array */
void MULTIPLY(Mat io, int mult) {
    int i = 0;
    int j = 0;

    for (i = 0; i < io.rows; i++) {
        for (j = 0; j < io.cols; j++) {
            if ((io.at<Vec3b>(i,j).val[0] *= mult) > 255)
                io.at<Vec3b>(i,j).val[0] = 255;
            if ((io.at<Vec3b>(i,j).val[1] *= mult) > 255)
                io.at<Vec3b>(i,j).val[1] = 255;
            if ((io.at<Vec3b>(i,j).val[2] *= mult) > 255)
                io.at<Vec3b>(i,j).val[2] = 255;
        }
    }

}

