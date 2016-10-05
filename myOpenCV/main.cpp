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
using namespace std;
void PinP_tr(const cv::Mat &srcImg, const cv::Mat &smallImg, const int tx, const int ty);


int main(int argc, char** argv)
{
    
    cv::VideoCapture cap(0);
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

    int crop_x=320;
    int crop_y=50;
    int crop_w=150;
    int crop_h=150;
    
    int cycle=100;
    //crop image and save it as the fisrt prev image
    cv::Mat prevFrame;
    cap >> prevFrame;
    
    //give it a break...
    cv::waitKey(cycle);
    
    int was_head_down=0;
    int head_down_count=0;
    while (1) {
        cv::Mat frame;
        cap >> frame;
        cv::Rect myROI(crop_x, crop_y, crop_w, crop_h);
        cv::Mat cut_img(frame,myROI);
        cv::Mat prev_cut_img(prevFrame,myROI);
   
        
        cv::Mat prevFrameGray;
        cv::Mat currFrameGray;
        
        cv::cvtColor(prev_cut_img, prevFrameGray, CV_RGB2GRAY);
        cv::cvtColor(cut_img, currFrameGray, CV_RGB2GRAY);
        
        // 特徴点抽出
        std::vector<cv::Point2f> prevCorners;
        std::vector<cv::Point2f> currCorners;
        
        cv::goodFeaturesToTrack(prevFrameGray, prevCorners, 20, 0.05, 5.0);
        cv::goodFeaturesToTrack(currFrameGray, currCorners, 20, 0.05, 5.0);
        
        cv::cornerSubPix(prevFrameGray, prevCorners, cv::Size(21, 21), cv::Size(-1, -1), cv::TermCriteria(cv::TermCriteria::COUNT | cv::TermCriteria::EPS, 30, 0.01));
        cv::cornerSubPix(currFrameGray, currCorners, cv::Size(21, 21), cv::Size(-1, -1), cv::TermCriteria(cv::TermCriteria::COUNT | cv::TermCriteria::EPS, 30, 0.01));
        
        std::vector<uchar> featuresFound;
        std::vector<float> featuresErrors;
        
        
        cv::calcOpticalFlowPyrLK(
                                 prevFrameGray,
                                 currFrameGray,
                                 prevCorners,
                                 currCorners,
                                 featuresFound,
                                 featuresErrors);
       
        std::vector<float> direction_x;
        std::vector<float> direction_y;
        for (int i = 0; i < featuresFound.size(); i++) {
            cv::Point p1 = cv::Point((int) prevCorners[i].x, (int) prevCorners[i].y);
            cv::Point p2 = cv::Point((int) currCorners[i].x, (int) currCorners[i].y);
            cv::line(cut_img, p1, p2, cv::Scalar(0, 0, 255), 2);
            direction_y.push_back(prevCorners[i].y-currCorners[i].y);
        }
       
        float total=0;
        int i;
        for (i=0; i < direction_y.size();i++){
            total=total+direction_y[i];
        }
        total=total/i;
        
        if(abs(total)>15){
            if (total>0){
                if(was_head_down){
                   // cout << "UP" <<endl;
                    was_head_down=0;
                }
            }else{
                if(!was_head_down){
                   // cout << "DOWN" <<endl;
                    head_down_count++;
                    was_head_down=1;
                }
            }
        }
        
        cout << total<<endl;
        // 特徴点を描画
        IplImage tmp=cut_img;
        for (int i = 0; i < currCorners.size(); i++){
        //     printf("x=%4.0f y=%4.0f\n", currCorners[i].x,currCorners[i].y);
            cvCircle(&tmp,cvPoint(currCorners[i].x,currCorners[i].y), 2, CV_RGB (0, 0, 255), 2);
        }

        IplImage tmp2=prevFrameGray;
        for (int i = 0; i < prevCorners.size(); i++){
            //     printf("x=%4.0f y=%4.0f\n", currCorners[i].x,currCorners[i].y);
            cvCircle(&tmp2,cvPoint(prevCorners[i].x,prevCorners[i].y), 2, CV_RGB (0, 0, 255), 2);
        }
        
        char value_c[256]; //次の行で使う一時的な変数
        sprintf(value_c, "Count: %d", head_down_count);
         putText(frame, value_c, cv::Point(450,450), cv::FONT_HERSHEY_DUPLEX,1, cv::Scalar(0,0,255), 1, CV_AA);
        if(was_head_down)putText(frame, "DOWN", cv::Point(450,400), cv::FONT_HERSHEY_DUPLEX,1, cv::Scalar(0,0,255), 1, CV_AA);
        else putText(frame, "UP", cv::Point(450,400), cv::FONT_HERSHEY_DUPLEX,1, cv::Scalar(0,0,255), 1, CV_AA);

        
      
        
        cv::imshow("preview", frame);
        cv::imshow("featue", cv::cvarrToMat(&tmp));
        cv::imshow("featue2", cv::cvarrToMat(&tmp2));
        prevFrame = frame;
        //PinP_tr(frame, cv::cvarrToMat(&cut_img), 10,10);
        
        int key = cv::waitKey(cycle);
        if(key == 113)
        {
            break;
        }
        else if(key == 'w'){
            crop_y=crop_y-10;
            cout<<crop_x<<","<<crop_y<<endl;
        }
        else if(key == 'x'){
            crop_y=crop_y+10;
             cout<<crop_x<<","<<crop_y<<endl;
        }
        else if(key == 'a'){
            crop_x=crop_y-10;
             cout<<crop_x<<","<<crop_y<<endl;
        }
        else if(key =='d'){
            crop_x=crop_x+10;
             cout<<crop_x<<","<<crop_y<<endl;
        }
        else if(key =='s'){
            crop_w=crop_w-10;
            crop_h=crop_h-10;
            cout<<crop_w<<","<<crop_h<<endl;
        }
        else if(key =='b'){
            crop_w=crop_w+10;
            crop_h=crop_h+10;
            cout<<crop_w<<","<<crop_h<<endl;
        }
        else if(key =='r'){
            head_down_count=0;
        }
        

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


