//
//  main.cpp
//  openCVsample2
//
//  Created by Chie AHAREN on 2015/01/31.
//  Copyright (c) 2015年 Chie AHAREN. All rights reserved.
//

#include <cvaux.h>
#include <highgui.h>

using namespace cv;

int main (int argc, char **argv){
    
    int vertices = 4;
    int object_num = 0;
    float x = 0,y = 0;
    CvMemStorage *storage = cvCreateMemStorage (0);
    CvSeq* contours = 0;
    CvSeq* result;
    CvPoint* pt[vertices];

    
    //画像の読み込み
    IplImage* src_img = cvLoadImage("/Users/e125731/Desktop/openCVsample2/pic/写真.JPG");
    
    IplImage *gray_img =cvCreateImage(cvGetSize(src_img),IPL_DEPTH_8U,1);
    
    // 平滑化
    cvCvtColor(src_img, gray_img, CV_BGR2GRAY);//グレー化
    cvSmooth(gray_img, gray_img, CV_GAUSSIAN, 5, 5, 0, 0);//平滑化(ガウシアン処理)

    // 二値化
    cvThreshold(gray_img, gray_img, 0, 255, THRESH_TRUNC|THRESH_OTSU);
    cvThreshold(gray_img, gray_img, 0, 255, THRESH_BINARY|THRESH_OTSU);

    //輪郭検出
    cvFindContours(gray_img, storage, &contours, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
    
    //検出した輪郭から折れ線を検出する
    while(contours){
        
        //折れ線のもとになる線を検出する(Douglas-Peuckerを利用)
        result = cvApproxPoly(contours, sizeof(CvContour), storage, CV_POLY_APPROX_DP, cvContourPerimeter(contours)*0.05, 0);
        
        //折れ線のもとになる線の数が4つのとき
       if(result->total==vertices  && abs(cvContourArea(result, CV_WHOLE_SEQ))>100)
        {
         
            for(int i=0;i<vertices;i++)
                pt[i] = (CvPoint*)cvGetSeqElem(result, i);
            
            if(abs(pt[1]->y - pt[0]->y) < 500){
                
                x = (pt[0]->x + pt[1]->x + pt[2]->x + pt[3]->x )/4; //四角の中心座標xを求める
                y = (pt[0]->y + pt[1]->y + pt[2]->y + pt[3]->y )/4; //四角の中心座標yを求める
                
                cvCircle(src_img,cvPoint(x,y), 5, CV_RGB(255,0,0), -1);
                
                object_num++;
                printf("---%d個目---\n",object_num);
                printf("(%d ,%d)\n",(int)x,(int)y);
            }
        }
       
        contours = contours->h_next;//次の輪郭を調べる

    }
    
    // (4)画像の表示
    cvNamedWindow ("result", CV_WINDOW_AUTOSIZE);
    cvShowImage ("result",src_img);
    cvWaitKey (0);

    cvDestroyWindow ("result");
    
    return 0;
}