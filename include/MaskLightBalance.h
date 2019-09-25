#ifndef MASKLIGHTBALANCE_H
#define MASKLIGHTBALANCE_H
#include<opencv2/opencv.hpp>
#include<iostream>
#include<stdio.h>
#include<math.h>
#include "PictureParameter.h"
#include "PictureProcess.h"

#define LimitPix(x) do{ if(x>255)x=255;}while(0)

#define GAMMA_TRANSFER_C 0.8
#define GAMMA_TRANSFER_R 0.2
#define GAMMA_RETRANSFER_C 1.8
#define GAMMA_RETRANSFER_R 1.2
#define GammaTransfer(x) \
({ \
    float temp=0;\
    do{\
        temp=x/255.0;\
        temp=GAMMA_TRANSFER_C*pow(temp,GAMMA_TRANSFER_R)*255;\
        LimitPix(temp);\
        }while(0);\
        (uchar)temp;\
        })

#define GammaReverseTransfer(x)\
({\
    float temp=0;\
    do{\
        temp=x/255.0;\
        temp=GAMMA_RETRANSFER_C*pow(temp,GAMMA_RETRANSFER_R)*255;\
    }while(0);\
    (uchar)temp;\
})
#define PI 3.1415926
using namespace cv;
using namespace std;

class MaskLightBalance:public PictureProcess
{
    public:
        MaskLightBalance();
        MaskLightBalance(cv::Mat src);
        void filterPic(cv::Mat& dst,cv::Mat src,cv::Mat bkImg);
        float getPicShrinkFactor(Point2f pointPos,Mat src);
        void stretchPic(cv::Mat& dst,cv::Mat outImg,cv::Mat shrinkImg);
        void shrinkPic(cv::Mat& dst,cv::Mat src,cv::Mat outImg);
        void lightBalance(cv::Mat& dst,cv::Mat src);
        virtual ~MaskLightBalance();

    protected:


    private:
        float oringinPicMean;
        int oringinPicMinValue;
        int oringinPicMaxValue;
        int bkPicMinValue;
        int bkPicMaxValue;
        int constrictFactorLow;
        int constrictFactorHigh;
        int stretchFactor;
        std::vector<int> pixValid;

};

#endif // MASKLIGHTBALANCE_H
