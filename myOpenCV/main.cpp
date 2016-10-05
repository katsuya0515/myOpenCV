//
//  main.cpp
//  myOpenCV
//
//  Created by Katsuya Fujii on 9/30/16.
//  Copyright © 2016 Katsuya Fujii. All rights reserved.
//

#include "opencv2/opencv.hpp"
using namespace std;



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


int main(int argh, char* argv[])
{
    cv::VideoCapture cap(0);//デバイスのオープン
    //cap.open(0);//こっちでも良い．
    
    if(!cap.isOpened())//カメラデバイスが正常にオープンしたか確認．
    {
        //読み込みに失敗したときの処理
        return -1;
    }
    
    
    int crop_x=0;
    int crop_y=0;
    
    while(1)//無限ループ
    {
        cv::Mat frame;
        cap >> frame; // get a new frame from camera
        
        //
        //取得したフレーム画像に対して，クレースケール変換や2値化などの処理を書き込む．
        //
        
        cv::Rect myROI(crop_x, crop_y, 300, 300);
        
        cv::Mat cut_img(frame,myROI);
        // cv::imwrite("/Users/kty0515/Documents/Xcode/myOpenCV/data/img.png", cut_img);
       PinP_tr(frame, cut_img, 10, 10);
        
        int key = cv::waitKey(1);
        if(key == 113)//qボタンが押されたとき
        {
            break;//whileループから抜ける．
        }
        else if(key == 115)//sが押されたとき
        {
            cv::Rect myROI(0, 0, 100, 100);
            
            cv::Mat cut_img(frame,myROI);
            cv::imwrite("/Users/kty0515/Documents/Xcode/myOpenCV/data/img.png", cut_img);
            cout<<"Saved"<<endl;
        }
        else if(key == 'w'){
            if(crop_y!=10)crop_y=crop_y-10;
        }
        else if(key == 'x'){
            crop_y=crop_y+10;
        }
        else if(key == 'a'){
            if(crop_x!=10)crop_x=crop_y-10;
        }
        else if(key =='d'){
            crop_x=crop_x+10;
        }
        cout <<crop_x<<","<<crop_y<<endl;
    }
    cv::destroyAllWindows();
    return 0;
}
