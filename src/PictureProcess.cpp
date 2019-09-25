#include "PictureProcess.h"

PictureProcess::PictureProcess()
{
    //ctor
}
PictureProcess::~PictureProcess()
{
    //dtor
}
void PictureProcess::GaussFilter(Mat& dst,Mat src)
{
    cv::GaussianBlur(src,dst,cv::Size(3,3),0,0,BORDER_DEFAULT);
}
void PictureProcess::SobelFilter(Mat& dstx,Mat& dsty,Mat src)
{
    cv::Sobel(src,dstx,CV_8U,1,0,3,1,0);
    cv::Sobel(src,dsty,CV_8U,0,1,3,1,0);
}
void PictureProcess::Laplacian(Mat& dst,Mat src)
{
    cv::Laplacian(src,dst,CV_8U,1,1,0);
}
float PictureProcess::getMeanDerivationGrads(Mat src)
{
    cv::Mat dstx,dsty;
    uchar* datax;
    uchar* datay;
    int width,hight;
    int sum=0,num=0;
    if(!src.empty())
    {
        width=src.rows;
        hight=src.cols;
        num=width*hight;
        SobelFilter(dstx,dsty,src);
        datax=dstx.data;
        datay=dsty.data;
        for(int i=0;i<num;i++)
                sum+=sqrt(pow(datax[i],2)+pow(datay[i],2));
        return sum/num;
        }
    else
        cout<<"src is empty"<<endl;
    return -1;
}
float PictureProcess::getxMeanFirstDerivationGrads(Mat src)
{
    cv::Mat dstx,dsty;
    uchar* data;
    int num=0;
    int sum=0;
    if(!src.empty())
    {
        num=src.rows*src.cols;
        SobelFilter(dstx,dsty,src);
        data=dstx.data;
        for(int i=0;i<num;i++)
            sum+=data[i];
        return sum/num;
        }
    else
        cout<<"src is empty"<<endl;
    return -1;
}
float PictureProcess::getyMeanFirstDerivationGrads(Mat src)
{
    cv::Mat dstx,dsty;
    uchar* data;
    int num=0;
    int sum=0;
    if(!src.empty())
    {
        num=src.rows*src.cols;
        SobelFilter(dstx,dsty,src);
        data=dsty.data;
        for(int i=0;i<num;i++)
            sum+=data[i];
        return sum/num;
        }
    else
        cout<<"src is empty"<<endl;
    return -1;
}

float PictureProcess::getSecondDerivationGrads(Mat src)
{
    cv::Mat dst;
    uchar* data;
    int num=0;
    int sum=0;
    if(!src.empty())
    {
        num=src.rows*src.cols;
        Laplacian(dst,src);
        data=dst.data;
        for(int i=0;i<num;i++)
            sum+=data[i];
        return sum/num;
        }
    else
        cout<<"src is empty"<<endl;
    return -1;

}

void PictureProcess::pictureCut(std::vector<cv::Mat>& imageROI,cv::Mat& src,int rowscut,int colscut)
{
    PictureParameter* picParas=new PictureParameter();
    picParas->pointToPic(src);
    int width=picParas->getWidth();
    int height=picParas->getHeight();
    int xCutStep=width/colscut;
    int yCutStep=height/rowscut;
    cv::Mat img;
    for(int i=0;i<colscut;i++)
        for(int j=0;j<colscut;j++)
        {
            img=src(Rect(i*yCutStep,j*xCutStep,xCutStep,yCutStep));
            imageROI.push_back(img);
            }
}
void PictureProcess::pictureSharpen(cv::Mat& dst,cv::Mat src,int flags)
{
    Mat LaplacianPic,GaussePic;
    if(flags==0)
    {
    Laplacian(LaplacianPic,src);
    dst=0.2*LaplacianPic+src;
    }
    else if(flags==1)
    {
    GaussFilter(GaussePic,src);
    dst=src-GaussePic+src;
    }
    else
        cout<<"flags error"<<endl;
}
void PictureProcess::picErode(cv::Mat& dst,cv::Mat src,int geometry,Size elem_size)
{
    Mat element=cv::getStructuringElement(geometry,elem_size,Point(-1,-1));
    cv::erode(src,dst,element);
}
void PictureProcess::picDilate(cv::Mat& dst,cv::Mat src,int geometry,Size elem_size)
{
    Mat element=cv::getStructuringElement(geometry,elem_size,Point(-1,-1));
    cv::dilate(src,dst,element);

}
void PictureProcess::picConvolute(cv::Mat& dst,cv::Mat src,Mat kernel)
{
   cv::filter2D(src,dst,CV_8UC1,kernel,Point(-1,-1),0,BORDER_DEFAULT);
}

void PictureProcess::picCannyCheck(cv::Mat& dst,cv::Mat src,double threshold1,double threshold2)
{
   cv::Canny(src,dst,threshold1,threshold2);
}
void PictureProcess::picCreateOutline(cv::Mat& dst,cv::Mat src,int dilateSize,int erodeSize)
{
    Mat tmp;
    picDilate(tmp,src,MORPH_RECT,Size(dilateSize,dilateSize));
    picErode(dst,tmp,MORPH_RECT,Size(erodeSize,erodeSize));
}
void PictureProcess::picConvolute(cv::Mat& dst,cv::Mat src,cv::Mat kernel,std::vector<cv::Point2f> pos)
{
    dst=src.clone();
    int kernelsize=sqrt(kernel.rows*kernel.cols);
    int i,j;
    for(int num=0;num<pos.size();num++)
    {
        i=pos[num].x;
        j=pos[num].y;
        if (i > (kernelsize / 2) - 1 && j > (kernelsize / 2) - 1 &&
                i <src.rows - (kernelsize / 2) && j <src.cols - (kernelsize / 2))
      {
                double sum = 0.0;
                for (int k = 0; k < kernelsize; k++)
                {
                    for (int l = 0; l < kernelsize; l++)
                        sum += src.at<uchar>(i+k-(kernelsize/2),j+l-(kernelsize/2)) * (kernel.at<float>(k,l));
                }
                dst.at<uchar>(i,j)=(uchar)sum;
      }
    }
}
void PictureProcess::picPannelProcess(cv::Mat& dst,Mat src,cv::Mat gausseSrc,cv::Mat model, std::vector<cv::Point2f> pos)
{

}

void PictureProcess::picEclosionProcess(cv::Mat& dst,cv::Mat src,cv::Mat edge,std::vector<cv::Point2f> pos)
{
	dst = src.clone();
    int posNum = pos.size();
    std::vector<cv::Mat> bgrChannels;
    split(dst,bgrChannels);
    cv::Mat bData=bgrChannels.at(0);
    cv::Mat gData=bgrChannels.at(1);
    cv::Mat rData=bgrChannels.at(2);
    cv::Mat alphaData=bgrChannels.at(3);
    alphaData=edge.clone();
    for (int i = 0; i < posNum; i++)
    {

        //bData.at<uchar>(pos[i].x, pos[i].y)*=(alphaData.at<uchar>(pos[i].x, pos[i].y)/255.0);
        //gData.at<uchar>(pos[i].x, pos[i].y)*=(alphaData.at<uchar>(pos[i].x, pos[i].y)/255.0);
        //rData.at<uchar>(pos[i].x, pos[i].y)*=(alphaData.at<uchar>(pos[i].x, pos[i].y)/255.0);
        alphaData.at<uchar>(pos[i].x, pos[i].y)=255;

    }

    bData.copyTo(bgrChannels.at(0));
    gData.copyTo(bgrChannels.at(1));
    rData.copyTo(bgrChannels.at(2));
    alphaData.copyTo(bgrChannels.at(3));
    cv::merge(bgrChannels,dst);
    #if 0
	int posNum = pos.size();
        for (int i = 0; i < dst.rows; i++)
                for (int j = 0;j <dst.cols; j++)
        {
             Vec4b & rgba=dst.at<Vec4b>(i, j);
             rgba[0]=255;
        }

    bData.copyTo(bgrChannels.at(0));
    gData.copyTo(bgrChannels.at(1));
    rData.copyTo(bgrChannels.at(2));
    alphaData.copyTo(bgrChannels.at(3));
    #endif // 0
   // cv::imshow("dst alpha img",dst);
}
void PictureProcess::picFuseProcess(cv::Mat& dst,cv::Mat src,cv::Mat kernel,std::vector<cv::Point2f> pos)
{
	dst = src.clone();
	std::vector<cv::Mat> bgrChannels;
    split(dst,bgrChannels);
    cv::Mat bData=bgrChannels.at(0);
    cv::Mat gData=bgrChannels.at(1);
    cv::Mat rData=bgrChannels.at(2);
    picConvolute(bData,bData,kernel,pos);
    picConvolute(gData,gData,kernel,pos);
    picConvolute(rData,rData,kernel,pos);
    bData.copyTo(bgrChannels.at(0));
    gData.copyTo(bgrChannels.at(1));
    rData.copyTo(bgrChannels.at(2));
    cv::merge(bgrChannels,dst);
}
Mat PictureProcess::gaussKernel(int kernel_size,float sigma,int flags)
{
    const double PI = 3.14159265358979323846;
    int m = kernel_size/2;
    Mat mask_temp(kernel_size,kernel_size,CV_32FC1);
    float s_2 = 2*sigma*sigma;
    for (int i=0;i<kernel_size;i++)
    {
        for (int j=0;j<kernel_size;j++)
        {
            int x=i-m;
            int y=j-m;
            if(flags==0)
                mask_temp.ptr<float>(i)[j] = exp(-(x*x+y*y)/2*s_2) /(PI*s_2);
            else
                mask_temp.ptr<float>(i)[j] = 1-exp(-(x*x+y*y)/2*s_2) /(PI*s_2);

        }
    }
    return mask_temp;
}
Mat PictureProcess::meanKernel(int kernel_size)
{
    int m = kernel_size/2;
    Mat mask_temp(kernel_size,kernel_size,CV_32FC1);
    for (int i=0;i<kernel_size;i++)
    {
        for (int j=0;j<kernel_size;j++)
        {
                mask_temp.ptr<float>(i)[j] = 1.0/(kernel_size*kernel_size);

        }
    }
    return mask_temp;
}
