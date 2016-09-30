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


int main () {
    int corner_count = 150;
    IplImage *dst_img, *src_img_gray, *eig_img, *temp_img;
    CvPoint2D32f *corners;

    dst_img = cvLoadImage("/Users/kty0515/Documents/Xcode/myOpenCV/myOpenCV/imgres.jpg", CV_LOAD_IMAGE_ANYCOLOR | CV_LOAD_IMAGE_ANYDEPTH);
    src_img_gray = cvLoadImage("/Users/kty0515/Documents/Xcode/myOpenCV/myOpenCV/imgres.jpg", CV_LOAD_IMAGE_GRAYSCALE);
    eig_img = cvCreateImage(cvGetSize(src_img_gray), IPL_DEPTH_32F, 1);
    temp_img = cvCreateImage(cvGetSize(src_img_gray), IPL_DEPTH_32F, 1);
    corners = (CvPoint2D32f *) cvAlloc(corner_count * sizeof (CvPoint2D32f));
    
    // cvCornerMinEigenValを利用したコーナー検出
    cvGoodFeaturesToTrack (src_img_gray, eig_img, temp_img, corners, &corner_count, 0.25, 15);
    cvFindCornerSubPix (src_img_gray, corners, corner_count,
                        cvSize (3, 3), cvSize (-1, -1), cvTermCriteria (CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03));
    // 特徴点を描画
    for (int i = 0; i < corner_count; i++){
        printf("x=%4.0f y=%4.0f\n", corners[i].x, corners[i].y);
        cvCircle(dst_img, cvPointFrom32f(corners[i]), 20, CV_RGB (0, 0, 255), 2);
    }
    
    cv::imshow("test", dst_img);
    
    cvReleaseImage(&dst_img);
    cvReleaseImage(&eig_img);
    cvReleaseImage(&temp_img);
    cvReleaseImage(&src_img_gray);
    
    return 0;
}
