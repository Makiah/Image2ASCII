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
#include <map>

// The sizes of the squares which this image is divided into.
const unsigned int subdividedSquareSideLength = 20;
const unsigned int subdividedSquareSideHalfLength = subdividedSquareSideLength / 2.0;
const unsigned int useSpaceCeiling = 50;

struct SubdivisionScore
{
    unsigned int n;
    float mean;
    float variance;
};

// Variance equation from https://math.stackexchange.com/questions/198336/how-to-calculate-standard-deviation-with-streaming-inputs
struct SubdivisionScores
{
    SubdivisionScore left;
    SubdivisionScore right;
};

void handleSubdivisionScoresUpdate(float newData, SubdivisionScore& s)
{
    s.n++;
    float delta = (newData - s.mean);
    s.mean += delta / s.n;
    s.variance += delta * (newData - s.mean); // NOT the same as delta^2
}

unsigned int assignFinalScore(SubdivisionScores& s)
{
    return (unsigned int) (abs(((s.left.mean - s.right.mean))));
}

std::string explainSubdivisionScores(SubdivisionScores& s)
{
    return "Left mean is " + std::to_string(s.left.mean) + " while right mean is " + std::to_string(s.right.mean) + " and left variance is " + std::to_string(s.left.variance) + " and right variance is " + std::to_string(s.right.variance) + " resulting in score of " + std::to_string(assignFinalScore(s));
}

/**
 Definitely the hardest part of this, have to figure out how this image is segmented into conformant colors.
 */
char matchSubdivisionToASCII(cv::Mat& subdivided)
{
    const bool debugging = false;
    
    // Calculate standard deviation and mean for grayscale for both halves in all cases.
    // Standard deviation too high indicates that it's too noisy to come to a conclusion, also reduces the chance that we miss an outlier
    // Large dfference in mean between both and low standard deviation indicates that they're distinct entities
    
    // Ensure this is a correctly formed mat
    assert(subdivided.cols == subdividedSquareSideLength && subdivided.rows == subdividedSquareSideLength);
    
    // Measures standard deviation and mean for both sides
    SubdivisionScores horizontal = {(SubdivisionScore){0, 0., 0.}, (SubdivisionScore){0, 0., 0.}};
    SubdivisionScores vertical = {(SubdivisionScore){0, 0., 0.}, (SubdivisionScore){0, 0., 0.}};
    SubdivisionScores diagonalFromTopLeft = {(SubdivisionScore){0, 0., 0.}, (SubdivisionScore){0, 0., 0.}};
    SubdivisionScores diagonalFromTopRight = {(SubdivisionScore){0, 0., 0.}, (SubdivisionScore){0, 0., 0.}};
    
    // Big pixel state machine
    for(int row = 0; row < subdividedSquareSideLength; row++) // rows
    {
        for (int col = 0; col < subdividedSquareSideLength; col++) // cols
        {
            float currentValue = (float)(subdivided.at<uchar>(row, col));
            
            // On left of diagonal from top left
            if (col <= row)
            {
                handleSubdivisionScoresUpdate(currentValue, diagonalFromTopLeft.left);
            }
            // On right of diagonal
            else
            {
                handleSubdivisionScoresUpdate(currentValue, diagonalFromTopLeft.right);
            }
            
            // On left of diagonal from top right
            if (col <= subdividedSquareSideLength - row)
            {
                handleSubdivisionScoresUpdate(currentValue, diagonalFromTopRight.left);
            }
            // On right of diagonal
            else
            {
                handleSubdivisionScoresUpdate(currentValue, diagonalFromTopRight.right);
            }
            
            // On top half
            if (row <= subdividedSquareSideHalfLength)
            {
                handleSubdivisionScoresUpdate(currentValue, vertical.left);
            }
            // On bottom half
            else
            {
                handleSubdivisionScoresUpdate(currentValue, vertical.right);
            }
            
            // On left half
            if (col <= subdividedSquareSideHalfLength)
            {
                handleSubdivisionScoresUpdate(currentValue, horizontal.left);
            }
            // On bottom half
            else
            {
                handleSubdivisionScoresUpdate(currentValue, horizontal.right);
            }
        }
    }
    
    // Better way to rank hopefully
    std::map<unsigned int, char> characterScores;
    characterScores[assignFinalScore(horizontal)] = '|';
    characterScores[assignFinalScore(vertical)] = '-';
    characterScores[assignFinalScore(diagonalFromTopLeft)] = '\\';
    characterScores[assignFinalScore(diagonalFromTopRight)] = '/';
    
    std::map<unsigned int, char>::iterator lastEntry = (--characterScores.end());
    unsigned int bestScore = lastEntry->first;
    
    char chosenASCII = bestScore <= useSpaceCeiling ? ' ' : lastEntry ->second;
    
    if (debugging)
    {
        imshow("Current", subdivided);
        std::cout << "Horizontal: " + explainSubdivisionScores(horizontal) << std::endl;
        std::cout << "Vertical: " + explainSubdivisionScores(vertical) << std::endl;
        std::cout << "Diagonal TL: " + explainSubdivisionScores(diagonalFromTopLeft) << std::endl;
        std::cout << "Diagonal TR: " + explainSubdivisionScores(diagonalFromTopRight) << std::endl;
        std::cout << "In favor of " + std::string(1, chosenASCII) << std::endl;
        std::cout << std::endl;
        cv::waitKey(0);
    }
    
    return chosenASCII;
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
//    imshow( "Image grayscale", image );                // Show our image inside it.
//    cv::waitKey(0); // Wait for a keystroke in the window
    
    // Subdivision from http://answers.opencv.org/question/53694/divide-an-image-into-lower-regions/
    // Subdivide image into squares of predetermined dimension
    cv::Mat maskImg = image.clone();
    
    // check if divisors fit to image dimensions
    std::vector<char> matchedASCII;
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
        matchedASCII.reserve((image.cols * image.rows) / (subdividedSquareSideLength * subdividedSquareSideLength));
        
        // Subdivide by Rect
        for(int y = 0; y < image.rows; y += subdividedSquareSideLength)
        {
            for(int x = 0; x < image.cols; x += subdividedSquareSideLength)
            {
                cv::Mat newImage = image(cv::Rect(x, y, subdividedSquareSideLength, subdividedSquareSideLength));
                
                rectangle(maskImg, cv::Point(x, y), cv::Point(x + subdividedSquareSideLength - 1, y + subdividedSquareSideLength - 1), cv::Scalar(0), 1); // visualization
                
//                std::cout << "x = " + std::to_string(x) + " y = " + std::to_string(y) << std::endl;
                char selectedASCII = matchSubdivisionToASCII(newImage);
                
                if (x == 17 * 20 && y == 2 * 20)
                {
                    std::cout << "For obvious one chose " + std::string(1, selectedASCII) << std::endl;
                }
                
                cv::putText(maskImg,
                            std::string(1, selectedASCII),
                            cv::Point(x + 10, y + 10), // Coordinates
                            cv::FONT_HERSHEY_COMPLEX_SMALL, // Font
                            0.7, // Scale. 2.0 = 2x bigger
                            cv::Scalar(127), // BGR Color
                            1, // Line Thickness (Optional)
                            CV_AA); // Anti-alias (Optional)
                
                matchedASCII.push_back(selectedASCII);
            }
        }
    }
    
    // Create string "image"
    std::string asciiImage;
    for (unsigned int i = 0; i < matchedASCII.size(); i++)
    {
        if (i % (image.cols / subdividedSquareSideLength) == 0)
        {
            asciiImage.append("\n");
        }
        asciiImage += matchedASCII[i];
    }
    std::cout << asciiImage << std::endl;
    
    imshow("Close?", maskImg); // visualization
    cv::waitKey(0); // visualization
    
    return 0;
}
