#ifndef PICTUREPARAMETER_H
#define PICTUREPARAMETER_H
#include<opencv2/opencv.hpp>
#include<iostream>
#include<stdio.h>
#include<math.h>

#define FULL_IMGE 0
#define DEFORMITY_IMGE 1

using namespace cv;
using namespace std;
class PictureParameter
{
    public:
        PictureParameter();
        void init(cv::Mat pic);
        virtual ~PictureParameter();
        int getWidth();
        int getHeight();
        int getDepth();
        int getChannels();
        float  getMean();
        float getVariance();
        float getStandardDeviation();
        int getMaxValue();
        int getMinValue();
        void showParas(string str);
        Mat intensityPic;
        void  caculatePic();
        void pointToPic(Mat pic);
        std::vector<cv::Point2f> pointInvalidPos;
        std::vector<cv::Point2f> pointValidPos;
        std::vector<uchar> validvalue;

    protected:

    private:
        int width;
        int height;
        int depth;
        int channels;
        float mean;
        float variance;
        float standarddeviation;
        double maxvalue;
        double minvalue;

};

#endif // PICTUREPARAMETER_H
