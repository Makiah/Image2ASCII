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
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <string>

// The sizes of the squares which this image is divided into.
const int subdividedSquareSideLength = 20;

/**
 Definitely the hardest part of this, have to figure out how this image is segmented into conformant colors.
 */
char matchSubdivisionToASCII(cv::Mat& subdivided)
{
    return '-';
}

int main( int argc, char** argv )
{
    // Use default image in the evnet that no image was provided
    cv::String imageName( "../data/HappyFish.jpg" ); // by default
    if( argc > 1)
    {
        imageName = argv[1];
    }
    
    // Load image
    cv::Mat image;
    image = imread( imageName, cv::IMREAD_COLOR ); // Read the file
    if( image.empty() )                      // Check for invalid input
    {
        std::cout <<  "Could not open or find the image" << std::endl ;
        return -1;
    }
    
    // Display some image properties
    std::cout << "Image has " + std::to_string(image.cols) + " columns, and " + std::to_string(image.rows) + " rows" << std::endl;
    
    // Display image
    cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
    namedWindow( "Display window", cv::WINDOW_AUTOSIZE ); // Create a window for display.
    imshow( "Image grayscale", image );                // Show our image inside it.
    cv::waitKey(0); // Wait for a keystroke in the window
    
    // Subdivision from http://answers.opencv.org/question/53694/divide-an-image-into-lower-regions/
    // Subdivide image into squares of predetermined dimension
    std::vector<cv::Mat> blocks;
    cv::Mat maskImg = image.clone();
    
    // check if divisors fit to image dimensions
    if(image.cols % subdividedSquareSideLength != 0)
    {
        std::cerr << "Error: Please use another divisor for the column split." << std::endl;
        exit(1);
    }
    else if(image.rows % subdividedSquareSideLength != 0)
    {
        std::cerr << "Error: Please use another divisor for the row split." << std::endl;
        exit(1);
    }
    else
    {
        // No need to resize since we know image dimensions.
        blocks.reserve((image.cols * image.rows) / (subdividedSquareSideLength * subdividedSquareSideLength));
        
        // Subdivide by Rect
        for(int y = 0; y < image.cols; y += subdividedSquareSideLength)
        {
            for(int x = 0; x < image.rows; x += subdividedSquareSideLength)
            {
                blocks.push_back(image(cv::Rect(y, x, subdividedSquareSideLength, subdividedSquareSideLength)).clone());
                rectangle(maskImg, cv::Point(y, x), cv::Point(y + subdividedSquareSideLength - 1, x + subdividedSquareSideLength - 1), cv::Scalar(255), 1); // visualization
            }
        }
    }
    
    imshow("Image block", maskImg); // visualization
    std::cout << "Created " + std::to_string(blocks.size()) + " blocks" << std::endl;
    cv::waitKey(0); // visualization
    
    // Figure out which ASCII character to match each subdivided region to
    std::vector<char> matchedASCII;
    matchedASCII.reserve(blocks.size());
    for (cv::Mat& block : blocks)
        matchedASCII.push_back(matchSubdivisionToASCII(block));
    
    std::cout << "Created " + std::to_string(matchedASCII.size()) + " ascii" << std::endl;
    
    // Create string "image"
    std::string asciiImage;
    for (unsigned int i = 0; i < matchedASCII.size(); i++)
    {
        if (i % (image.cols / subdividedSquareSideLength) == 0)
        {
            asciiImage.append("\n");
            std::cout << "Newline at " + std::to_string(i) << std::endl;
        }
        asciiImage += matchedASCII[i];
    }
    std::cout << asciiImage << std::endl;
    
    return 0;
}
