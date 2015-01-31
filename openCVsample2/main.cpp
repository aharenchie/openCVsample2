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
    const char *imagename = argc > 1 ? argv[1] : "/Users/e125731/Desktop/openCVsample2/pic/写真.JPG";
    Mat src_img = imread(imagename);
    if(!src_img.data)
        return -1;
    Mat gray_img;
    cvtColor(src_img, gray_img, CV_BGR2GRAY);
    
    // (2)apply a fixed-level threshold to each pixel
    Mat trunc_img;
    threshold(gray_img, trunc_img, 0, 255, THRESH_TRUNC|THRESH_OTSU);
    threshold(trunc_img, trunc_img, 0, 255, THRESH_BINARY|THRESH_OTSU);

    
    // (3)apply an adaptive threshold to a grayscale image
    Mat adaptive_img;
    adaptiveThreshold(gray_img, adaptive_img, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 7, 8);
    
    //(4)show source and destination images
 
    namedWindow("Trunc", CV_WINDOW_AUTOSIZE);
   
    imshow("Trunc", trunc_img);
   
    waitKey(0);
     

    int i;
    IplImage **dst_img;
    CvMemStorage *storage = cvCreateMemStorage (0);
    CvSeq *contours = 0;
    
    IplImage iplImage = src_img;
    IplImage test =trunc_img;
    int levels = 0;
    
    dst_img = (IplImage **) cvAlloc (sizeof (IplImage *) * 3);
    for (i = 0; i < 3; i++) {
        dst_img[i] = cvCloneImage (&iplImage);
    }

    //輪郭検出
     cvFindContours (&test, storage, &contours, sizeof (CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
    // (3)輪郭の描画
    cvDrawContours (dst_img[0], contours, CV_RGB (255, 0, 0), CV_RGB (0, 255, 0), levels - 1, 2, CV_AA, cvPoint (0, 0));
    cvDrawContours (dst_img[1], contours, CV_RGB (255, 0, 0), CV_RGB (0, 255, 0), levels, 2, CV_AA, cvPoint (0, 0));
    cvDrawContours (dst_img[2], contours, CV_RGB (0, 0, 0), CV_RGB (0, 255, 0), levels + 1, 2, CV_AA, cvPoint (0, 0));
    
    // (4)画像の表示
    cvNamedWindow ("Level:-1", CV_WINDOW_AUTOSIZE);
    cvShowImage ("Level:-1", dst_img[0]);
    cvNamedWindow ("Level:0", CV_WINDOW_AUTOSIZE);
    cvShowImage ("Level:0", dst_img[1]);
    cvNamedWindow ("Level:1", CV_WINDOW_AUTOSIZE);
    cvShowImage ("Level:1", dst_img[2]);
    cvWaitKey (0);
    
    cvDestroyWindow ("Level:-1");
    cvDestroyWindow ("Level:0");
    cvDestroyWindow ("Level:1");
    
    /*
    int i;
    CvSeq *lines = 0;
    CvMemStorage *storage;
    IplImage test =trunc_img;
    IplImage src = src_img;
    CvPoint *point;
    
    // (2)ハフ変換のための前処理
    cvCanny (&test, &test, 50, 200, 3);
    storage = cvCreateMemStorage (0);
    
    // (4)確率的ハフ変換による線分の検出と検出した線分の描画
    lines = 0;
    lines = cvHoughLines2 (&test, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI / 180, 50, 50, 10);
    for (i = 0; i < lines->total; i++) {
        point = (CvPoint *) cvGetSeqElem (lines, i);
        cvLine (&src, point[0], point[1], CV_RGB (255, 0, 0), 3, 8, 0);
    }
    
    namedWindow("Trunc", CV_WINDOW_AUTOSIZE);
    imshow("Trunc", src_img);
    
    waitKey(0);
     */
    
    return 0;}