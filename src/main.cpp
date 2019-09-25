#include<opencv2/opencv.hpp>
#include<iostream>
#include<stdio.h>
#include "PictureParameter.h"
#include "PictureProcess.h"
#include "MaskLightBalance.h"

string DIR="./pic/";
#define PIC 1
#define SHOW_OUTLINE 0
#if PIC
string PIC_NAME ="carmodel_3840x2160.png";
string PIC_PRO_NAME="carmodel_process_3840x2160.png";
#else
string PIC_NAME ="soarer.png";
string PIC_PRO_NAME="soarer_process.png";

#endif // PIC
using namespace cv;
using namespace std;
int main()
{

        Mat picture = cv::imread(DIR+PIC_NAME,IMREAD_UNCHANGED);
        cout<<"------------------this is picture  paras------------------"<<endl;
        cout<<"  width:  "<< picture.cols<<endl;
        cout<<"  hight  "<< picture.rows<<endl;
        cout<<"  channels:  "<<picture.channels()<<endl;
        cout<<"------------------this is picture  paras------------------"<<endl;
        PictureParameter* PicParas=new PictureParameter();
        //Mat picture = cv::imread("test.jpg",IMREAD_COLOR);

        int CHANNELS=picture.channels();
#if PIC
        Rect rect=Rect(1000,400,2000,1200);

#else
        Rect rect=Rect(1400,400,1000,1200);
#endif
        //Mat initialsrc=picture(rect);
        Mat initialsrc=picture.clone();
        imshow("initialsrc pic",initialsrc);


        //imshow("initial pic",initialsrc);
        cv::Mat srcHSV;
        std::vector<cv::Mat> rgbChannel;
        //cvtColor(initialsrc,srcHSV,CV_BGR2HSV);
        split(initialsrc,rgbChannel);
        cout<<"rgbChannel size :"<<rgbChannel.size()<<endl;
        cvtColor(initialsrc,srcHSV,CV_BGR2HSV);

        std::vector<cv::Mat> hsvChannel;
        split(srcHSV,hsvChannel);


       // PicParas->showParas();
       // cv::imshow("test",picture);
 //       cv::imshow("test",PicParas->IntensityData);


        //cout<<"intesity:"<<PicParas->IntensityData<<endl;
        //imshow("H",HSVChannel[0]);
        // imshow("S",HSVChannel[1]);
        Mat SrcData=hsvChannel.at(2).clone();
        //imshow("SrcData pic",SrcData);
        PicParas->pointToPic(SrcData);
        PicParas->showParas("SrcData pic");


        Mat img;

        PictureProcess* PicPro=new PictureProcess();

#if 0
        PicPro->GaussFilter(img,SrcData);
        imshow("gaussefilter pic",img);
        PicParas->pointToPic(img);
        PicParas->showParas("gaussefilter pic");
#endif // 1
#if 1
        PicPro->picCannyCheck(img,SrcData,200,500);
        #if SHOW_OUTLINE
       // imshow("canny pic",img);
        #endif
        PicParas->pointToPic(img);
        PicParas->showParas("canny pic");
#else
        PicPro->Laplacian(img,SrcData);
        #if SHOW_OUTLINE
        imshow("laplacian pic",img);
        #endif
        PicParas->pointToPic(img);
        PicParas->showParas("laplacian pic");
#endif
        Mat thickOutline;
        Mat thinOutline;
        Mat bigOutline;
        Mat outline;
#if 1
        PicPro->picCreateOutline(outline,img,7,7);
        #if SHOW_OUTLINE
        imshow("outline pic",outline);
        #endif

        PicPro->picCreateOutline(thickOutline,img,5,6);
        PicPro->picDilate(thickOutline,thickOutline,MORPH_RECT,Size(3,3));
        #if SHOW_OUTLINE
        imshow("thickOutline pic",thickOutline);
        #endif

        thinOutline=outline-thickOutline;
        PicPro->picDilate(thinOutline,thinOutline,MORPH_RECT,Size(2,2));

        //PicPro->picCreateOutline(thinOutline,img,5,7);
        #if SHOW_OUTLINE
        imshow("thinOutline pic",thinOutline);
        #endif



#if 0
        Mat gausseEdge;
        PicPro->GaussFilter(gausseEdge,img);
        imshow("gaussefilter pic",gausseEdge);
        PicParas->pointToPic(gausseEdge);
        PicParas->showParas("gaussefilter pic");
#endif // 1



#endif // 1
        Mat kernel;
        Mat fuseImg;
        Mat fuseMeanImg;

#if 0
        kernel=PicPro->gaussKernel(5,0.85);

        Mat convoluteImg;
        PicPro->picConvolute(convoluteImg,SrcData,kernel,pointValidPos);
        imshow("picConvolute pic",convoluteImg);
        PicParas->pointToPic(convoluteImg);
        PicParas->showParas("picConvolute pic");
#endif
#if 1

        PicParas->pointToPic(thinOutline);
        std::vector<cv::Point2f> pointThinPos(PicParas->pointValidPos);
        kernel=PicPro->meanKernel(5);
       // kernel=PicPro->gaussKernel(5,0.9,0);
        //cout<<"gauss kernel: "<<kernel<<endl;
        PicPro->picFuseProcess(fuseImg,initialsrc,kernel,pointThinPos);
        //imshow("gauss pic0",fuseImg);
        //cv::imwrite(DIR+"gauss_5_0.9"+PIC_PRO_NAME,fuseImg);
        PicParas->pointToPic(thickOutline);
        std::vector<cv::Point2f> pointThickPos(PicParas->pointValidPos);
        kernel=PicPro->meanKernel(15);
       // cout<<"gauss kernel: "<<kernel<<endl;
        PicPro->picFuseProcess(fuseImg,fuseImg,kernel,pointThickPos);
        imshow("meanKernel pic1",fuseImg);
       // cv::addWeighted(fuseImg,0.8,initialsrc,0.2,3,fuseImg);
       // imshow("addWeighted pic1",fuseImg);

        cv::imwrite(DIR+"fusePic_divide_pro_"+PIC_PRO_NAME,fuseImg);
#endif
#if 1


        PicParas->pointToPic(outline);
        std::vector<cv::Point2f> pointOutlinePos(PicParas->pointValidPos);
        kernel=PicPro->meanKernel(7);
        //cout<<"gauss kernel: "<<kernel<<endl;
        PicPro->picFuseProcess(fuseMeanImg,initialsrc,kernel,pointOutlinePos);
        imshow("fuseMeanImg pic1",fuseMeanImg);
       // cv::addWeighted(fuseImg,0.8,initialsrc,0.2,3,fuseImg);
       // imshow("addWeighted pic1",fuseImg);
        cv::imwrite(DIR+"fusePic_all_pro_"+PIC_PRO_NAME,fuseMeanImg);

#endif
#if 0
        kernel=PicPro->gaussKernel(3,0.96,0);
        //cout<<"gauss kernel: "<<kernel<<endl;
        PicPro->picEclosionProcess(fuseImg,initialsrc,gausseEdge,pointValidPos);
        imshow("eclosion pic",fuseImg);
#endif
/*
        Mat pannelImg;

        PicPro->picPannelProcess(pannelImg,picture,kernel,pointValidPos);
        imshow("pannel pic",pannelImg);

        imshow("pannel pic",pannelImg);
        PicParas->pointToPic(pannelImg);
        PicParas->showParas("pannel pic");

        Mat processedImg=pannelImg;
        imshow("processed pic",processedImg);
        PicParas->pointToPic(processedImg);
        PicParas->showParas("processed pic");
*/
        //PicPro->pictureSharpen(img,SrcData,1);
        //imshow("sharpen pic",img);
        //PicParas->pointToPic(img);
       // PicParas->showParas();
        //MaskLightBalance* MaskBal=new MaskLightBalance(SrcData);
       // MaskBal->lightBalance(img,SrcData);
      //  imshow("gamma pic",img);
      //  PicParas->pointToPic(img);
     //   PicParas->showParas();
/*
        Mat DstPic;
//        std::vector<cv::Mat> HSVChannel(3);
        processedImg.copyTo(hsvChannel.at(2));
        cv::merge(hsvChannel,DstPic);
        cvtColor(DstPic,DstPic,CV_HSV2BGR);
        cv::imwrite(PIC_PRO_NAME,DstPic);
*/
      //  cout<<"mean value:      "<<PicParas->getMean()<<"\n"<<endl;
        cv::waitKey(0);
       // delete PicParas;
        return 0;
}
