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

int
main (int argc, char **argv)
{
    // (1)load a specified file and convert it into grayscale image
    IplImage* src_img = cvLoadImage("/Users/e125731/Desktop/openCVsample2/pic/写真.JPG");
    IplImage *gray_img =cvCreateImage( cvGetSize(src_img),IPL_DEPTH_8U,1);
    
    cvCvtColor(src_img, gray_img, CV_BGR2GRAY);
    cvSmooth(gray_img, gray_img, CV_GAUSSIAN, 5, 5, 0, 0);//平滑化

    // (2)apply a fixed-level threshold to each pixel
 
    cvThreshold(gray_img, gray_img, 0, 255, THRESH_TRUNC|THRESH_OTSU);
    cvThreshold(gray_img, gray_img, 0, 255, THRESH_BINARY|THRESH_OTSU);

    
    CvMemStorage *storage = cvCreateMemStorage (0);
    CvSeq *contours = 0;


    //輪郭検出
     cvFindContours(gray_img, storage, &contours, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));
    
    CvSeq* result;
    float x = 0,y = 0;
    int vertices = 4;

    while(contours){
        result = cvApproxPoly(contours, sizeof(CvContour), storage, CV_POLY_APPROX_DP, cvContourPerimeter(contours)*0.05, 0);
        
        
        //if there are 4 vertices in the contour(It should be a quadrilateral)
       if(result->total==vertices  && abs(cvContourArea(result, CV_WHOLE_SEQ))>100)
        {
            //iterating through each point
            CvPoint *pt[vertices];
            for(int i=0;i<vertices;i++){
                pt[i] = (CvPoint*)cvGetSeqElem(result, i);
            }
    
            if(abs(pt[1]->y - pt[0]->y) < 500){
                
                x = (pt[0]->x + pt[1]->x + pt[2]->x + pt[3]->x )/4;
                y = (pt[0]->y + pt[1]->y + pt[2]->y + pt[3]->y )/4;
                cvCircle(src_img,cvPoint(x,y), 5, CV_RGB(255, 0,0), -1);
                
            }
        }
       
        contours = contours->h_next;

        
    }
      // (4)画像の表示
    cvNamedWindow ("Level:1", CV_WINDOW_AUTOSIZE);
    cvShowImage ("Level:1",src_img);
    cvWaitKey (0);
    

    cvDestroyWindow ("Level:1");
 
    
    return 0;
}