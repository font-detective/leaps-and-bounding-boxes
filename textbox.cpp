// Luke Mitchell, 2015

#include <iostream>
#include "opencv2/opencv.hpp"

// william from StackOverflow.com
// http://stackoverflow.com/questions/23506105/extracting-text-opencv
// http://creativecommons.org/licenses/by-sa/3.0/
std::vector<cv::Rect> detectLetters(cv::Mat img)
{
    std::vector<cv::Rect> boundRect;
    cv::Mat img_gray, img_sobel, img_threshold, element;
    cvtColor(img, img_gray, CV_BGR2GRAY);
    cv::Sobel(img_gray, img_sobel, CV_8U, 1, 0, 3, 1, 0, cv::BORDER_DEFAULT);
    cv::threshold(img_sobel, img_threshold, 0, 255, CV_THRESH_OTSU+CV_THRESH_BINARY);
    element = getStructuringElement(cv::MORPH_RECT, cv::Size(17, 3) );
    cv::morphologyEx(img_threshold, img_threshold, CV_MOP_CLOSE, element);
    std::vector< std::vector< cv::Point> > contours;
    cv::findContours(img_threshold, contours, 0, 1); 
    std::vector<std::vector<cv::Point> > contours_poly( contours.size() );
    for( int i = 0; i < contours.size(); i++ )
        if (contours[i].size()>100)
        { 
            cv::approxPolyDP( cv::Mat(contours[i]), contours_poly[i], 3, true );
            cv::Rect appRect( boundingRect( cv::Mat(contours_poly[i]) ));
            if (appRect.width>appRect.height) 
                boundRect.push_back(appRect);
        }
    return boundRect;
}

int main(int argc,char** argv)
{
    cv::Mat img, imggray, subimg;
    int ii, jj;

    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " imagename.jpg" << std::endl;
        return EXIT_FAILURE;
    }

    // Read image
    img = cv::imread(argv[1]);

    if (!img.data)
    {
        std::cout << "Could not find or open image " << argv[1] << std::endl;
        return EXIT_FAILURE;
    }
    
    // Convert this image to grayscale, store it
    cv::cvtColor(img, imggray, CV_BGR2GRAY);
    
    // Detect letters and return bounding boxes
    std::vector<cv::Rect> letterBBoxes = detectLetters(img);
    
    // Pick largest bounding box
    cv::Rect max(0,0,0,0);
    for(int i = 0; i < letterBBoxes.size(); i++) {
        cv::Rect r = letterBBoxes[i];
        if (r.area() > max.area())
          max = r;
    }
    
    // Now, store that as another matrix
    img(max).copyTo(subimg);

    // Overlay bounding boxes
    for(int i=0; i< letterBBoxes.size(); i++)
        cv::rectangle(img, letterBBoxes[i], cv::Scalar(0,255,0), 3, 8, 0);
    
    // ...and overlay largest box in red
    cv::rectangle(img, max, cv::Scalar(0, 0, 255), 3, 8, 0);
    
    // Display it!
    cv::namedWindow("Bounding Boxes", CV_WINDOW_AUTOSIZE);
    cv::imshow("Bounding Boxes", img);
    
    // Display text box!
    cv::namedWindow("Text Box", CV_WINDOW_AUTOSIZE);
    cv::imshow("Text Box", subimg);
    
    // Wait for keypress
    cv::waitKey(0);
    return EXIT_SUCCESS;
}

