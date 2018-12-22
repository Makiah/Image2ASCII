//
//  main.cpp
//  Image2ASCII
//
//  Created by Makiah Bennett on 12/22/18.
//  Copyright © 2018 Makiah Bennett. All rights reserved.
//

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <string>
int main( int argc, char** argv )
{
    cv::String imageName( "../data/HappyFish.jpg" ); // by default
    if( argc > 1)
    {
        imageName = argv[1];
    }
    cv::Mat image;
    image = imread( imageName, cv::IMREAD_COLOR ); // Read the file
    if( image.empty() )                      // Check for invalid input
    {
        std::cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
    namedWindow( "Display window", cv::WINDOW_AUTOSIZE ); // Create a window for display.
    imshow( "Display window", image );                // Show our image inside it.
    cv::waitKey(0); // Wait for a keystroke in the window
    return 0;
}
