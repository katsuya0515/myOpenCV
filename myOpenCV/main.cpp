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
    cv::VideoCapture cap(0);
    cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
    
    const int cycle = 30;
    
    cv::Mat prevFrame;
    cv::Size frameSize = prevFrame.size();
    cap >> prevFrame;
    
    cv::waitKey(cycle);
    
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
        
        for (int i = 0; i < featuresFound.size(); i++) {
            cv::Point p1 = cv::Point((int) prevCorners[i].x, (int) prevCorners[i].y);
            cv::Point p2 = cv::Point((int) currCorners[i].x, (int) currCorners[i].y);
            cv::line(frame, p1, p2, cv::Scalar(0, 0, 255), 2);
        }
        
        cv::imshow("preview", frame);
        prevFrame = frame;
        if (cv::waitKey(cycle) == 27) { break; }
    }
    return 0;
}
