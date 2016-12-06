#include <iostream>
#include <string>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


std::string prePath = "/Users/aliaksandrzankevich/Education/University/ComputerVision/Projects/OpenCV_6/";

int getBackGround(cv::Mat& target, std::string path) {
    cv::VideoCapture cap(path);
    int counter = 0;
    
    if (!cap.isOpened()) {
        std::cout << "Cannot open the video file" << std::endl;
        return -1;
    }
    
    while(true) {
        cv::Mat frame;
        
        bool bSuccess = cap.read(frame);
        
        if (!bSuccess) {
            std::cout << "Cannot read the frame from video file" << std::endl;
            break;
        }
        
        frame.convertTo(frame, CV_32FC1);
        
        if (counter == 0) {
            target = frame;
        } else {
            target += frame;
        }
        
        counter++;
    }
    
    target /= counter;
    target.convertTo(target, CV_8UC1);
    
    return 0;
}

int substractBackground(std::string path) {
    cv::Mat background;
    cv::namedWindow("BG", CV_WINDOW_AUTOSIZE);
    cv::namedWindow("Video", CV_WINDOW_AUTOSIZE);
    cv::VideoCapture cap(path);
    
    getBackGround(background, path);
    cv::cvtColor(background, background, CV_RGB2GRAY);

    
    cv::imshow("BG", background);
    
    if (!cap.isOpened()) {
        std::cout << "Cannot open the video file" << std::endl;
        return -1;
    }
    
    while(true) {
        cv::Mat frame;
        
        bool bSuccess = cap.read(frame);
        
        if (!bSuccess) {
            std::cout << "Cannot read the frame from video file" << std::endl;
            break;
        }
        
        cv::cvtColor(frame, frame, CV_RGB2GRAY);
        
        /*frame -= background;*/

        for (int i = 0; i < frame.rows - 1; i++) {
            for (int j = 0; j < frame.cols - 1; j++) {
                if (std::abs(frame.at<uchar>(i, j) - background.at<uchar>(i, j)) < 30) {
                    frame.at<uchar>(i, j) = 0;
                } else {
                    frame.at<uchar>(i, j) = 255;
                }
            }
        }
        
        cv::morphologyEx(frame, frame, cv::MORPH_CLOSE, cv::getStructuringElement(2, cv::Size(7, 7)));

        cv::morphologyEx(frame, frame, cv::MORPH_OPEN, cv::getStructuringElement(2, cv::Size(20, 20)));
        
        cv::imshow("Video", frame);
        
        
        if(cv::waitKey(30) == 27) {
            std::cout << "Esc key is pressed by user" << std::endl;
            break;
        }
    }
    
    return 0;
}

int backgroundSubstration(std::string path) {
    while(true) {
        substractBackground(path);
        
        if(cv::waitKey(30) == 27) {
            std::cout << "Esc key is pressed by user" << std::endl;
            break;
        }
    }
    
    cv::waitKey(0);
    
    return 0;
}

int main() {
    return backgroundSubstration(prePath + "c10.avi");
}
