//
//  main.cpp
//  myOpenCV
//
//  Created by Katsuya Fujii on 9/30/16.
//  Copyright © 2016 Katsuya Fujii. All rights reserved.
//

#include "opencv2/opencv.hpp"
#include <iostream>
#include <vector>
using namespace std;
void PinP_tr(const cv::Mat &srcImg, const cv::Mat &smallImg, const int tx, const int ty);


int main(int argc, char** argv)
{
    CvPoint2D32f *corners;
    int crop_x=240;
    int crop_y=80;
    
    cv::VideoCapture cap(0);
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
    
    int corner_count = 500;
   

    IplImage dst_img, src_img_gray, *eig_img, *temp_img;
    
    while (1) {
        cv::Mat frame;
        cap >> frame;
     
        
        cv::Rect myROI(crop_x, crop_y, 200, 200);
        cv::Mat cut_img(frame,myROI);
        
        cv::Mat currFrameGray;

        
        cv::cvtColor(cut_img, currFrameGray, CV_RGB2GRAY);
        
        // 特徴点抽出
        dst_img=cut_img;
        src_img_gray =currFrameGray;
        eig_img = cvCreateImage(cvGetSize(&src_img_gray), IPL_DEPTH_32F, 1);
        temp_img = cvCreateImage(cvGetSize(&src_img_gray), IPL_DEPTH_32F, 1);
        
        corners = (CvPoint2D32f *) cvAlloc(corner_count * sizeof (CvPoint2D32f));
        
        cvGoodFeaturesToTrack (&src_img_gray, eig_img, temp_img, corners, &corner_count, 0.15, 20);
        
        
        cvFindCornerSubPix (&src_img_gray, corners, corner_count,
                            cvSize (3, 3), cvSize (-1, -1), cvTermCriteria (CV_TERMCRIT_ITER | CV_TERMCRIT_EPS, 20, 0.03));
        
        
        // 特徴点を描画
        for (int i = 0; i < corner_count; i++){
           // printf("x=%4.0f y=%4.0f\n", corners[i].x, corners[i].y);
            cvCircle(&dst_img, cvPointFrom32f(corners[i]), 2, CV_RGB (0, 0, 255), 2);
        }
       
        PinP_tr(frame, cv::cvarrToMat(&dst_img), 10,10);
     
        int key = cv::waitKey(30);
        if(key == 113)
        {
            break;
        }
        else if(key == 'w'){
            crop_y=crop_y-10;
        }
        else if(key == 'x'){
            crop_y=crop_y+10;
        }
        else if(key == 'a'){
           crop_x=crop_y-10;
        }
        else if(key =='d'){
            crop_x=crop_x+10;
        }
      //  cout <<crop_x<<","<<crop_y<<endl;
        
        }
    return 0;
}

void PinP_tr(const cv::Mat &srcImg, const cv::Mat &smallImg, const int tx, const int ty)
{
    //背景画像の作成
    cv::Mat dstImg;
    srcImg.copyTo(dstImg);
    
    //前景画像の変形行列
    cv::Mat mat = (cv::Mat_<double>(2,3)<<1.0, 0.0, tx, 0.0, 1.0, ty);
    
    //アフィン変換の実行
    cv::warpAffine(smallImg, dstImg, mat, dstImg.size(), CV_INTER_LINEAR, cv::BORDER_TRANSPARENT);
    imshow("affine", dstImg);
}

