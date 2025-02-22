#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include "omp.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using uchar = unsigned char;
using uint = unsigned int;

class MedianFilter
{
public:
    virtual ~MedianFilter();
    MedianFilter(cv::Mat frame, uchar size);
    MedianFilter(int width, int height, uchar size);
    void initMedian(cv::Mat frame);
    void showIterator();
    void printCollection(std::vector<uchar> &collection);
    void showMedianNode(uint temp);
    void removeNode();
    void addImage(cv::Mat frame);
    void getMedian(cv::Mat frame);
    cv::Mat getMedian();
    void unitTestInit(cv::Mat M, uint interator);
    void unitTestCheck(std::vector<uchar> V1, std::vector<uchar> N1, std::vector<uchar> P1);
    

private:
    uchar mVectorIterator;
    uchar mSize;
    uchar mHalfSize;
    //uchar q1;
    //uchar q2;
    //uchar q3; 
    
    uint mFrameWidth;
    uint mFrameHeight;

    std::vector<std::vector<uchar>> vectorOfValues;
    std::vector<std::vector<uchar>> vectorOfNext;
    std::vector<std::vector<uchar>> vectorOfPrev;
    //cv::Mat localCopy;
    std::vector<uchar>::iterator it;
};
