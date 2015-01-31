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
    IplImage gray = gray_img;
    cvSmooth(&gray, &gray, CV_GAUSSIAN, 5, 5, 0, 0);//平滑化
    
    gray_img = &gray;
    // (2)apply a fixed-level threshold to each pixel
    Mat trunc_img;
    threshold(gray_img, trunc_img, 0, 255, THRESH_TRUNC|THRESH_OTSU);
    threshold(trunc_img, trunc_img, 0, 255, THRESH_BINARY|THRESH_OTSU);

    
    //(4)show source and destination images
 
    namedWindow("Trunc", CV_WINDOW_AUTOSIZE);
   
    imshow("Trunc", trunc_img);
   
    waitKey(0);
    
    
    /*コーナー検出
    vector<Point2f> corners;
    goodFeaturesToTrack(trunc_img, corners, 10, 0.5, 3, Mat(), 3, true);
    vector<Point2f>::iterator it_corner = corners.begin();
    for(; it_corner!=corners.end(); ++it_corner) {
        circle(src_img, Point(it_corner->x, it_corner->y), 1, Scalar(0,255,0), -1);
        circle(src_img, Point(it_corner->x, it_corner->y), 8, Scalar(0,255,0 ));
    }
    
    namedWindow("Trunc", CV_WINDOW_AUTOSIZE);
    imshow("Trunc", src_img);
    
    waitKey(0);
*/
    

    CvMemStorage *storage = cvCreateMemStorage (0);
    CvSeq *contours = 0;
    
    IplImage iplImage = src_img;
    IplImage test =trunc_img;
    //int levels = 0;
    

    //輪郭検出
     cvFindContours(&test, storage, &contours, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));
    
    CvSeq* result;
    int i = 0;
    while(contours){
        result = cvApproxPoly(contours, sizeof(CvContour), storage, CV_POLY_APPROX_DP, cvContourPerimeter(contours)*0.05, 0);
        
        
        //if there are 4 vertices in the contour(It should be a quadrilateral)
       if(result->total==4  && abs(cvContourArea(result, CV_WHOLE_SEQ))>100)
        {
            //iterating through each point
            CvPoint *pt[4];
            for(int i=0;i<4;i++){
                pt[i] = (CvPoint*)cvGetSeqElem(result, i);
            }
    
            if(abs(pt[1]->y - pt[0]->y) < 500){
            
            //drawing lines around the quadrilateral
            cvLine(&iplImage, *pt[0], *pt[1], cvScalar(0,255-i,i),4);
            cvLine(&iplImage, *pt[1], *pt[2], cvScalar(0,255-i,i),4);
            cvLine(&iplImage, *pt[2], *pt[3], cvScalar(0,255-i,i),4);
            cvLine(&iplImage, *pt[3], *pt[0], cvScalar(0,255-i,i),4);
            
            i += 60;
            }
        }
       
        //obtain the next contour
        contours = contours->h_next; 
 

        
    }
     //cvFindContours (&test, storage, &contours, sizeof (CvContour),CV_RETR_LIST,CV_CHAIN_APPROX_SIMPLE);
    // (3)輪郭の描画
    //cvDrawContours (&iplImage, contours, CV_RGB (0, 0, 0), CV_RGB (0, 255, 0), 2, 2, CV_AA, cvPoint (0, 0));
    
    
    /* ポリライン近似
    CvMemStorage *polyStorage = cvCreateMemStorage(0);
    CvTreeNodeIterator polyIterator;
    CvSeq *polys, *poly;
    int i;
    CvPoint pre_point,point,next_point;
    
    polys = cvApproxPoly( contours, sizeof( CvContour), polyStorage, CV_POLY_APPROX_DP, cvContourPerimeter(contours)*0.03, 0);
    
    cvInitTreeNodeIterator( &polyIterator, ( void*)polys, 5);
    
    while( (poly = (CvSeq *)cvNextTreeNode( &polyIterator)) != NULL)
    {
  //      if( ( abs(cvContourArea(poly, CV_WHOLE_SEQ)>10 ) ) )
        for( i=0; i<poly->total; i++)
        {
            
            pre_point = *( CvPoint*)cvGetSeqElem( poly, i-1);
            point = *( CvPoint*)cvGetSeqElem( poly, i);
            next_point = *( CvPoint*)cvGetSeqElem( poly, i+1);
    
            
          //if(80 < abs(point.y - next_point.y) && abs(point.y - next_point.y) < 250){
                cvLine(&iplImage,point,next_point,CV_RGB( 0, 255, 0) );
                cvCircle( &iplImage, next_point, 5, CV_RGB( 255, 0,0), -1);
                cvCircle( &iplImage, point, 5, CV_RGB( 0, 0, 255), -1);
//          }
            
  //          if(50 < abs(point.x - next_point.x) && abs(point.x - next_point.x) < 250 ){
      //          cvLine(&iplImage,point,next_point,CV_RGB( 0, 255, 0) );
       //         cvCircle( &iplImage, next_point, 5, CV_RGB( 255, 0,0), -1);
        //        cvCircle( &iplImage, point, 5, CV_RGB( 0, 0, 255), -1);
    //        }
 
            
        }
    }
    cvShowImage( "Poly", &iplImage);
    cvWaitKey(0);
     */
    
    // (4)画像の表示
    cvNamedWindow ("Level:1", CV_WINDOW_AUTOSIZE);
    cvShowImage ("Level:1", &iplImage);
    cvWaitKey (0);
    

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
    lines = cvHoughLines2 (&test, storage, CV_HOUGH_PROBABILISTIC, 1, CV_PI/90, 10, 50, 10);
    for (i = 0; i < lines->total; i++) {
        point = (CvPoint *) cvGetSeqElem (lines, i);
        cvLine (&src, point[0], point[1], CV_RGB (255, 0, 0), 3, 8, 0);
    }
    
    //エッジ検出可能
    cvNamedWindow ("Level:-1", CV_WINDOW_AUTOSIZE);
    cvShowImage ("Level:-1", &test);
    
    
    namedWindow("Trunc", CV_WINDOW_AUTOSIZE);
    imshow("Trunc", src_img);
    
    waitKey(0);
    */
    
    
    
    return 0;}