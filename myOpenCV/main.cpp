//
//  main.cpp
//  myOpenCV
//
//  Created by Katsuya Fujii on 9/30/16.
//  Copyright © 2016 Katsuya Fujii. All rights reserved.
//

#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"

#include <iostream>
#include <vector>

int main(int argc, char** argv)
{
     CvPoint2D32f *corners;
    
    cv::VideoCapture cap(0);
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
    
    const int cycle = 30;
     int corner_count = 150;
    cv::Mat prevFrame;
    cv::Size frameSize = prevFrame.size();
    cap >> prevFrame;
    
    cv::waitKey(cycle);
    
     IplImage dst_img, src_img_gray, *eig_img, *temp_img;
    
    while (1) {
        cv::Mat frame;
        cap >> frame;
        
        cv::Mat prevFrameGray;
        cv::Mat currFrameGray;
        
        cv::cvtColor(prevFrame, prevFrameGray, CV_RGB2GRAY);
        cv::cvtColor(frame, currFrameGray, CV_RGB2GRAY);
        
        // 特徴点抽出
        std::vector<cv::Point2f> prevCorners;
        std::vector<cv::Point2f> currCorners;
        
        dst_img = frame;
        src_img_gray =currFrameGray;
        eig_img = cvCreateImage(cvGetSize(&src_img_gray), IPL_DEPTH_32F, 1);
        temp_img = cvCreateImage(cvGetSize(&src_img_gray), IPL_DEPTH_32F, 1);
        
        corners = (CvPoint2D32f *) cvAlloc(corner_count * sizeof (CvPoint2D32f));
        
        cvGoodFeaturesToTrack (&src_img_gray, eig_img, temp_img, corners, &corner_count, 0.05, 15);
   
    
        cvFindCornerSubPix (&src_img_gray, corners, corner_count,
                            cvSize (3, 3), cvSize (-1, -1), cvTermCriteria (CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03));
        // 特徴点を描画
        for (int i = 0; i < corner_count; i++){
            printf("x=%4.0f y=%4.0f\n", corners[i].x, corners[i].y);
            cvCircle(&dst_img, cvPointFrom32f(corners[i]), 2, CV_RGB (0, 0, 255), 2);
        }

        cvShowImage("image2", &dst_img);
              prevFrame = frame;
        if (cv::waitKey(cycle) == 27) { break; }
    }
    return 0;
}


