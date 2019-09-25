#include "PictureParameter.h"


PictureParameter::PictureParameter()
{
    //ctor
}
void PictureParameter::init(cv::Mat pic)
{

        intensityPic=pic.clone();
        width=pic.cols;
        height=pic.rows;
        depth=pic.depth();
        channels=pic.channels();
}
PictureParameter::~PictureParameter()
{
    //dtor
}

int PictureParameter::getWidth()
{
        return width;
}

int PictureParameter::getHeight()
{
        return height;
}
int PictureParameter::getDepth()
{
        return depth;
}
int PictureParameter::getChannels()
{
        return channels;
}
void  PictureParameter::caculatePic()
{
        Mat src=intensityPic.clone();
        uchar* data=src.data;
        int width=src.cols;
        int height=src.rows;
        int num=width*height;
        float sum=0;
        validvalue.clear();
        pointValidPos.clear();
        pointInvalidPos.clear();
        for(int i=0;i<height;i++)
            for(int j=0;j<width;j++)
                    if(data[i*width+j]>0)
                    {
                        sum+=data[i*width+j];
                        validvalue.push_back(data[i*width+j]);
                        pointValidPos.push_back(Point2f(i,j));
                    }
                    else
                        pointInvalidPos.push_back(Point2f(i,j));
        //get the mean value
        int validnum=validvalue.size();
        mean= sum/validnum;
        sum=0;
        for(int i=0;i<validnum;i++)
            sum+=pow(validvalue[i]-mean ,2);
        //get the variance value
        variance=sum/validnum;
        standarddeviation=sqrt(variance);

        cv::minMaxLoc(validvalue,&minvalue,&maxvalue,0,0);

}

float PictureParameter::getMean()
{
        return mean;
}

float PictureParameter::getVariance()
{
        return variance;
}
float PictureParameter::getStandardDeviation()
{
        return standarddeviation;
}

int PictureParameter::getMaxValue()
{
    return maxvalue;
}
int PictureParameter::getMinValue()
{
    return minvalue;
}
void PictureParameter::pointToPic(Mat pic)
{
    init(pic);
    caculatePic();
}
void PictureParameter::showParas(string str)
{
        cout<<"------------------this is "<<str<<"  paras------------------"<<endl;
        cout<<"  width:  "<< this->getWidth()<<endl;
        cout<<"  hight  "<< this->getHeight()<<endl;
        cout<<"  depth:  "<<this->getDepth()<<endl;
        cout<<"  channels:  "<<this->getChannels()<<endl;
        cout<<"  mean value:  "<<this->getMean()<<endl;
        cout<<"  variance:  "<<this->getVariance()<<endl;
        cout<<"  standarddeviation:  "<<this->getStandardDeviation()<<endl;
        cout<<"------------------this is "<<str<<"  paras------------------"<<endl;
}
