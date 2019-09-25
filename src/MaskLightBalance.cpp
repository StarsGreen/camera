#include "MaskLightBalance.h"

MaskLightBalance::MaskLightBalance()
{
    //ctor
}
MaskLightBalance::MaskLightBalance(cv::Mat src)
{
    //ctor
    cv::Mat img=src.clone();
    PictureParameter* picParas=new PictureParameter();
    picParas->pointToPic(img);
    oringinPicMean=picParas->getMean();
    oringinPicMaxValue=picParas->getMaxValue();
    oringinPicMinValue=picParas->getMinValue();
}
MaskLightBalance::~MaskLightBalance()
{
    //dtor
}
void  MaskLightBalance::lightBalance(cv::Mat& dst,cv::Mat src)
{
    cv::Mat img=src.clone();
    std::vector<cv::Mat> roi;
    PictureParameter* picParas=new PictureParameter();
    int colsblock=2,rowsblock=2;
    cv::Mat outImg,stretchImg,shrinkImg;
    /*
    this->pictureCut(roi,img,rowsblock,colsblock);
    for(int i=0;i<colsblock*rowsblock;i++)
    {
        }
        */
    cv::Mat bkImg;
    GaussFilter(bkImg,img);
    picParas->pointToPic(bkImg);
    bkPicMaxValue=picParas->getMaxValue();
    bkPicMinValue=picParas->getMinValue();

    filterPic(outImg,img,bkImg);
    picParas->pointToPic(bkImg);

    imshow("out img",outImg);
    shrinkPic(shrinkImg,img,outImg);
  //  imshow("shrink img",shrinkImg);
    stretchPic(img,shrinkImg,outImg);
   // imshow("stretch img",img);

    dst=img.clone();
}
void  MaskLightBalance::filterPic(cv::Mat& dst,cv::Mat src,cv::Mat bkImg)
{
    cv::Mat img=src.clone();
    cv::Mat tempImg(img.rows,img.cols,CV_16SC1);
    //tempImg=img-bkImg;
    PictureParameter* picParas=new PictureParameter();
    picParas->pointToPic(img);

    int pixValue;
    int pointSize=picParas->pointValidPos.size();
        for(int i=0;i<pointSize;i++)
            {
            pixValue=img.at<uchar>(picParas->pointValidPos[i].x,picParas->pointValidPos[i].y);
            pixValue+=oringinPicMean;
            //tempValue=GammaTransfer(pixValue)-GammaTransfer(1);
            tempImg.at<int>(picParas->pointValidPos[i].x,picParas->pointValidPos[i].y)=pixValue-bkImg.at<uchar>(picParas->pointValidPos[i].x,picParas->pointValidPos[i].y);
            //img.at<uchar>(picParas->pointValidPos[i].x,picParas->pointValidPos[i].y)=255;
            }
     dst=tempImg.clone();
}

float  MaskLightBalance::getPicShrinkFactor(Point2f pointPos,Mat src)
{
    float factorLow;
    float factorHigh;
    if(src.at<int>(pointPos.x,pointPos.y)>=0)
        factorLow=1;
    else
        factorLow=oringinPicMinValue/(bkPicMinValue-oringinPicMean);
    if(src.at<int>(pointPos.x,pointPos.y)<=255)
        factorHigh=1;
    else
        factorLow=(oringinPicMaxValue-255)/(bkPicMaxValue-oringinPicMean);

    return MIN(factorLow,factorHigh);
}

void  MaskLightBalance::stretchPic(cv::Mat& dst,cv::Mat outImg,cv::Mat shrinkImg)
{
    cv::Mat img=shrinkImg.clone();
    PictureParameter* picParas=new PictureParameter();
    picParas->pointToPic(img);
    float stretchFactor;
    int pixImgValue,pixOutImgValue;

    picParas->pointToPic(img);
    int shrinkPicMaxValue=picParas->getMaxValue();
    int shrinkPicMinValue=picParas->getMinValue();
    int pixValue;
    int pointSize=picParas->pointValidPos.size();
    picParas->pointToPic(img);
        for(int i=0;i<pointSize;i++)
            {
            pixImgValue=img.at<uchar>(picParas->pointValidPos[i].x,picParas->pointValidPos[i].y);
            pixOutImgValue=outImg.at<uchar>(picParas->pointValidPos[i].x,picParas->pointValidPos[i].y);
            stretchFactor=1+sin((shrinkPicMaxValue-pixImgValue)/(shrinkPicMaxValue-shrinkPicMinValue)*PI/2);
            pixValue=(pixOutImgValue-oringinPicMean)*stretchFactor+oringinPicMean;
            //tempValue=GammaTransfer(pixValue)-GammaTransfer(1);
            img.at<uchar>(picParas->pointValidPos[i].x,picParas->pointValidPos[i].y)=pixValue;
            //img.at<uchar>(picParas->pointValidPos[i].x,picParas->pointValidPos[i].y)=255;
            }
    dst=img.clone();
}

void  MaskLightBalance::shrinkPic(cv::Mat& dst,cv::Mat src,cv::Mat outImg)
{
    cv::Mat img=src.clone();
    PictureParameter* picParas=new PictureParameter();
    picParas->pointToPic(img);
    int pixValue;
    int tempValue;
    int pointSize=picParas->pointValidPos.size();
        for(int i=0;i<pointSize;i++)
            {
            pixValue=img.at<uchar>(picParas->pointValidPos[i].x,picParas->pointValidPos[i].y);
            pixValue=(pixValue-oringinPicMean)*getPicShrinkFactor(picParas->pointValidPos[i],outImg)+oringinPicMean;
            //tempValue=GammaTransfer(pixValue)-GammaTransfer(1);
            img.at<uchar>(picParas->pointValidPos[i].x,picParas->pointValidPos[i].y)=pixValue;
            //img.at<uchar>(picParas->pointValidPos[i].x,picParas->pointValidPos[i].y)=255;
            }
    dst=img.clone();

}
