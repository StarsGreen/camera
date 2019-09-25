#ifndef PICTUREPROCESS_H
#define PICTUREPROCESS_H
#include<opencv2/opencv.hpp>
#include<iostream>
#include<stdio.h>
#include<math.h>
#include "PictureParameter.h"
using namespace cv;
using namespace std;
class PictureProcess
{
    public:
        PictureProcess();
        virtual ~PictureProcess();
        void GaussFilter(Mat& dst,Mat src);
        void SobelFilter(Mat& dstx,Mat& dsty,Mat src);
        void Laplacian(Mat& dst,Mat src);
        void picErode(cv::Mat& dst,cv::Mat src,int geometry,Size elem_size);
        void picDilate(cv::Mat& dst,cv::Mat src,int geometry,Size elem_size);
        void picPannelProcess(cv::Mat& dst,cv::Mat src,cv::Mat gausseSrc,cv::Mat model, std::vector<cv::Point2f> pos);
        void picConvolute(cv::Mat& dst,cv::Mat src,cv::Mat kernel,std::vector<cv::Point2f> pos);
        void picConvolute(cv::Mat& dst,cv::Mat src,Mat kernel);
        void picCannyCheck(cv::Mat& dst,cv::Mat src,double threshold1,double threshold2);
        void picFuseProcess(cv::Mat& dst,cv::Mat src,cv::Mat kernel,std::vector<cv::Point2f> pos);
        void picEclosionProcess(cv::Mat& dst,cv::Mat src,cv::Mat kernel,std::vector<cv::Point2f> pos);
        void picCreateOutline(cv::Mat& dst,cv::Mat src,int dilateSize,int erodeSize);



        cv::Mat gaussKernel(int kernel_size,float sigma,int flags);
        cv::Mat meanKernel(int kernel_size);

        float getxFirstDerivationGrads(Mat src);
        float getyFirstDerivationGrads(Mat src);
        float getSecondDerivationGrads(Mat src);
        float getxMeanFirstDerivationGrads(Mat src);
        float getyMeanFirstDerivationGrads(Mat src);
        float getMeanDerivationGrads(Mat src);
        void pictureCut(std::vector<cv::Mat>&imageROI,cv::Mat& src,int rowscut,int colscut);
        void pictureSharpen(cv::Mat& dst,cv::Mat src,int flags);

    protected:

    private:
        float xMeanFirstDerivationGrads;
        float yMeanFirstDerivationGrads;
        float secondDerivationGrads;
};

#endif // PICTUREPROCESS_H
