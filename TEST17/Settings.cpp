#include "Settings.h" 


int INIT_CAM_libv4l2(char *port) {
    v4l2_control c;
    int fp;
    
    if (!(fp = v4l2_open(port, O_RDWR))) {
        printf("Camera not accessable to v4l2\n");
        return -1;
    }

    c.id = V4L2_CID_EXPOSURE_AUTO;
    c.value = V4L2_EXPOSURE_MANUAL;
    if ((v4l2_ioctl(fp, VIDIOC_S_CTRL, &c) != 0)) {
        printf("v4l2: Exposure manual FAIL\n");
    }

    c.id = V4L2_CID_EXPOSURE_AUTO_PRIORITY;
    c.value = 0;
    if ((v4l2_ioctl(fp, VIDIOC_S_CTRL, &c) != 0)) {
        printf("v4l2: Exposure Priority FAIL\n");
    }

    printf("Setting Exposure\n");
    c.id = V4L2_CID_EXPOSURE_ABSOLUTE;
    v4l2_ioctl(fp, VIDIOC_G_CTRL, &c);
    printf("V4L2: CID_EXPOSURE_ABSOLUTE = %d\n",c.value);
    c.value = 2046;
    printf("V4L2: CID_EXPOSURE_ABSOLUTE SET = 50\n");

    if ((v4l2_ioctl(fp, VIDIOC_S_CTRL, &c) != 0)) {
        printf("v4l2: Exposure Set FAIL\n");
    }
    
    c.id = V4L2_CID_FOCUS_AUTO;
    c.value = false;
    if ((v4l2_ioctl(fp, VIDIOC_S_CTRL, &c) != 0)) {
        printf("v4l2: Focus Manual set fail\n");
    }

    c.id = V4L2_CID_FOCUS_ABSOLUTE;
    c.value = 0;
    if ((v4l2_ioctl(fp, VIDIOC_S_CTRL, &c) != 0)) {
        printf("v4l2: Focus set fail\n");
    }
    
    c.id = V4L2_CID_ZOOM_ABSOLUTE;
    c.value = 0;
    if ((v4l2_ioctl(fp, VIDIOC_S_CTRL, &c) != 0)) {
        printf("v4l2: Zoom set fail\n");
    }


    v4l2_close(fp);

    return 0;
}







int INIT_CAM_SET(VideoCapture cap, CamSet *cs) {
    int answer = 0;
    int err;

                                            /* timestap of capture  */
   printf("set CV_CAP_PROP_POS_MSEC ");
   err = cap.set(CV_CAP_PROP_POS_MSEC, cs->POS_MSEC);
   if (err) {
       printf("Fail\n");
       answer++;
   } else {
        printf("Success\n");
   }

                                            /* index next capture   */
   printf("set CV_CAP_PROP_POS_FRAMES ");
   err = cap.set(CV_CAP_PROP_POS_FRAMES, cs->POS_FRAMES);     
   if (err) {
       printf("Fail\n");
       answer++;
   } else {
        printf("Success\n");
   }

                                            /* somthing simler ???  */
   printf("set CV_CAP_PROP_POS_AVI_RATIO ");
   err = cap.set(CV_CAP_PROP_POS_AVI_RATIO, cs->POS_AVI_RATIO);  
   if (err) {
       printf("Fail\n");
       answer++;
   } else {
        printf("Success\n");
   }

                                            /* frame width in strm  */
   printf("set CV_CAP_PROP_FRAME_WIDTH ");
   err = cap.set(CV_CAP_PROP_FRAME_WIDTH, cs->FRAME_WIDTH);    
   if (err) {
       printf("Fail\n");
       answer++;
   } else {
        printf("Success\n");
   }

                                            /* frame height in strm */
   printf("set CV_CAP_PROP_FRAME_HEIGHT ");
   err = cap.set(CV_CAP_PROP_FRAME_HEIGHT, cs->FRAME_HEIGHT);
   if (err) {
       printf("Fail\n");
       answer++;
   } else {
        printf("Success\n");
   }

                                           /* frame rate           */ 
   printf("set CV_CAP_PROP_FPS ");
   err = cap.set(CV_CAP_PROP_FPS, cs->FPS); 
   if (err) {
       printf("Fail\n");
       answer++;
   } else {
        printf("Success\n");
   }

                                            /* image codec ident 4b */
   printf("set CV_CAP_PROP_FORCC ");
   err = cap.set(CV_CAP_PROP_FOURCC, cs->FOURCC);
   if (err) {
       printf("Fail\n");
       answer++;
   } else {
        printf("Success\n");
   }

                                            /* frames in video file */
   printf("set CV_CAP_PROP_FRAME_COUNT ");
   err = cap.set(CV_CAP_PROP_FRAME_COUNT, cs->FRAME_COUNT);    
   if (err) {
       printf("Fail\n");
       answer++;
   } else {
        printf("Success\n");
   }

                                            /* format of Mat obj    */
   printf("set CV_CAP_PROP_FORMAT ");
   err = cap.set(CV_CAP_PROP_FORMAT, cs->FORMAT);         
   if (err) {
       printf("Fail\n");
       answer++;
   } else {
        printf("Success\n");
   }

                                            /* Backend Capture mode */
   printf("set CV_CAP_PROP_MODE ");
   err = cap.set(CV_CAP_PROP_MODE, cs->MODE);
   if (err) {
       printf("Fail\n");
       answer++;
   } else {
        printf("Success\n");
   }

                                            /* Brightness of image  */
   printf("set CV_CAP_PROP_BRIGHTNESS ");
   err = cap.set(CV_CAP_PROP_BRIGHTNESS, cs->BRIGHTNESS);
   if (err) {
       printf("Fail\n");
       answer++;
   } else {
        printf("Success\n");
   }

                                            /* Contrast of image    */
   printf("set CV_CAP_PROP_CONTRAST ");
   err = cap.set(CV_CAP_PROP_CONTRAST, cs->CONTRAST);
   if (err) {
       printf("Fail\n");
       answer++;
   } else {
        printf("Success\n");
   }

                                            /* Saturation of image  */
   printf("set CV_CAP_SATURATION ");
   err = cap.set(CV_CAP_PROP_SATURATION, cs->SATURATION);
   if (err) {
       printf("Fail\n");
       answer++;
   } else {
        printf("Success\n");
   }

                                            /* Hue of the image     */
   printf("set CV_CAP_PROP_HUE ");
   err = cap.set(CV_CAP_PROP_HUE, cs->HUE);
   if (err) {
       printf("Fail\n");
       answer++;
   } else {
        printf("Success\n");
   }

                                            /* Gain of image        */
   printf("set CV_CAP_PROP_GAIN ");
   err = cap.set(CV_CAP_PROP_GAIN, cs->GAIN);
   if (err) {
       printf("Fail\n");
       answer++;
   } else {
        printf("Success\n");
   }

                                            /* Exposure of image    */
   printf("set CV_CAP_PROP_EXPOSURE ");
   err = cap.set(CV_CAP_PROP_EXPOSURE, cs->EXPOSURE);
   if (err) {
       printf("Fail\n");
       answer++;
   } else {
        printf("Success\n");
   }

                                            /* Boolean to RGB?      */
   printf("set CV_CAP_PROP_CONVERT_RGB ");
   err = cap.set(CV_CAP_PROP_CONVERT_RGB, cs->CONVERT_RGB);
   if (err) {
       printf("Fail\n");
       answer++;
   } else {
        printf("Success\n");
   }

                                            /* U value whitebal ??? */
   printf("set CV_CAP_PROP_WHITE_BALANCE_U ");
   err = cap.set(CV_CAP_PROP_WHITE_BALANCE_U, cs->WHITE_BALANCE_U);
   if (err) {
       printf("Fail\n");
       answer++;
   } else {
        printf("Success\n");
   }

                                            /* V value whitebal ??? */
   printf("set CV_CAP_PROP_WHITE_BALANCE_V ");
   err = cap.set(CV_CAP_PROP_WHITE_BALANCE_V, cs->WHITE_BALANCE_V);
   if (err) {
       printf("Fail\n");
       answer++;
   } else {
        printf("Success\n");
   }

                                            /* rect flag for stereo */
   printf("set CV_CAP_PROP_RECTIFICATION ");
   err = cap.set(CV_CAP_PROP_RECTIFICATION, cs->RECTIFICATION);
   if (err) {
       printf("Fail\n");
       answer++;
   } else {
        printf("Success\n");
   }

                                            /* sets iso             */
   printf("set CV_CAP_PROP_ISO_SPEED ");
   err = cap.set(CV_CAP_PROP_ISO_SPEED, cs->ISO_SPEED);
   if (err) {
       printf("Fail\n");
       answer++;
   } else {
        printf("Success\n");
   }

                                            /* numframes in buff    */
   printf("set CV_CAP_PROP_BUFFERSIZE ");
   err = cap.set(CV_CAP_PROP_BUFFERSIZE, cs->BUFFERSIZE);  
   if (err) {
       printf("Fail\n");
       answer++;
   } else {
        printf("Success\n");
   }

    
    return answer;
}


void INIT_CAM_DEFAULTS(CamSet *cs) {
    cs->POS_MSEC = cs->POS_MSEC;
    cs->POS_FRAMES = cs->POS_FRAMES;
    cs->POS_AVI_RATIO = cs->POS_AVI_RATIO;
    cs->FRAME_WIDTH = cs->FRAME_WIDTH;
    cs->FRAME_HEIGHT = cs->FRAME_HEIGHT;
    cs->FPS = cs->FPS;
    cs->FOURCC = cs->FOURCC ;
    cs->FRAME_COUNT = 1;
    cs->FORMAT = cs->FORMAT;
    cs->MODE = cs->MODE;
    cs->BRIGHTNESS = cs->BRIGHTNESS;
    cs->CONTRAST = cs->CONTRAST;
    cs->SATURATION = cs->SATURATION;
    cs->HUE = cs->HUE;
    cs->GAIN = cs->GAIN;
    cs->EXPOSURE = cs->EXPOSURE;
    cs->CONVERT_RGB = cs->CONVERT_RGB;
    cs->WHITE_BALANCE_U = cs->WHITE_BALANCE_U;
    cs->WHITE_BALANCE_V = cs->WHITE_BALANCE_V;
    cs->RECTIFICATION = cs->RECTIFICATION;
    cs->ISO_SPEED = cs->ISO_SPEED;
    cs->BUFFERSIZE = cs->BUFFERSIZE;
}

void INIT_CAM_GET(VideoCapture cap, CamSet *cs) {
     cs->POS_MSEC = cap.get(CV_CAP_PROP_POS_MSEC);
     cs->POS_FRAMES = cap.get(CV_CAP_PROP_POS_FRAMES);
     cs->POS_AVI_RATIO = cap.get(CV_CAP_PROP_POS_AVI_RATIO);
     cs->FRAME_WIDTH = cap.get(CV_CAP_PROP_FRAME_WIDTH);
     cs->FRAME_HEIGHT = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
     cs->FPS = cap.get(CV_CAP_PROP_FPS);
     cs->FOURCC = cap.get(CV_CAP_PROP_FOURCC);
     cs->FRAME_COUNT = cap.get(CV_CAP_PROP_FRAME_COUNT);
     cs->FORMAT = cap.get(CV_CAP_PROP_FORMAT);
     cs->MODE = cap.get(CV_CAP_PROP_MODE);
     cs->BRIGHTNESS = cap.get(CV_CAP_PROP_BRIGHTNESS);
     cs->CONTRAST = cap.get(CV_CAP_PROP_CONTRAST);
     cs->SATURATION = cap.get(CV_CAP_PROP_SATURATION);
     cs->HUE = cap.get(CV_CAP_PROP_HUE);
     cs->GAIN = cap.get(CV_CAP_PROP_GAIN);
     cs->EXPOSURE = cap.get(CV_CAP_PROP_EXPOSURE);
     cs->CONVERT_RGB = cap.get(CV_CAP_PROP_CONVERT_RGB);
     cs->WHITE_BALANCE_U = cap.get(CV_CAP_PROP_WHITE_BALANCE_U);
     cs->WHITE_BALANCE_V = cap.get(CV_CAP_PROP_WHITE_BALANCE_V);
     cs->RECTIFICATION = cap.get(CV_CAP_PROP_RECTIFICATION);
     cs->ISO_SPEED = cap.get(CV_CAP_PROP_ISO_SPEED);
     cs->BUFFERSIZE = cap.get(CV_CAP_PROP_BUFFERSIZE);
}
