/*
 * OpenCV_ex_16.c
 *
 * THE SOBEL OPERATOR
 *
 * J. Madeira - Dec 2012 + Nov 2017
 */


// Visual Studio ONLY - Allowing for pre-compiled header files

// This has to be the first #include

// Remove it, if you are not using Windows and Visual Studio


#include <iostream>


#include "opencv2/core/core.hpp"

#include "opencv2/imgproc/imgproc.hpp"

#include "opencv2/highgui/highgui.hpp"


// If you want to "simplify" code writing, you might want to use:

// using namespace cv;

// using namespace std;


// AUXILIARY  FUNCTION

void printImageFeatures(const cv::Mat &image)
{
	std::cout << std::endl;

	std::cout << "Number of rows : " << image.size().height << std::endl;

	std::cout << "Number of columns : " << image.size().width << std::endl;

	std::cout << "Number of channels : " << image.channels() << std::endl;

	std::cout << "Number of bytes per pixel : " << image.elemSize() << std::endl;

	std::cout << std::endl;
}


// MAIN

int main( int argc, char** argv )
{
    

	cv::VideoCapture cap;
    // open the default camera, use something different from 0 otherwise;
    // Check VideoCapture documentation.

	cv::Mat originalImage;

	originalImage = cv::imread( argv[1], cv::IMREAD_UNCHANGED );

	if( originalImage.empty() )
	{
		// NOT SUCCESSFUL : the data attribute is empty

		std::cout << "Image file could not be open !!" << std::endl;

	    return -1;
	}

	if( originalImage.channels() > 1 )
	{
		// Convert to a single-channel, intensity image

		cv::cvtColor( originalImage, originalImage, cv::COLOR_BGR2GRAY, 1 );
	}

	// Create window

    

	// Display image

	cv::imshow( "Imagem Original", originalImage );

	// Print some image features

	std::cout << "ORIGINAL IMAGE" << std::endl;

    printImageFeatures( originalImage );

    // 3 x 3 Canny Op.

    cv::Mat new_image;

	cv::Mat neg_image;
    
	cv::threshold( originalImage, new_image, 120,255, cv::THRESH_BINARY );

	cv::threshold( new_image, neg_image, 120,255, cv::THRESH_BINARY_INV );

    cv::namedWindow( "Negative", cv::WINDOW_AUTOSIZE );

    cv::imshow( "Negative", neg_image );

    
	int erosion_size = 3;

	cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3,3));

	cv::Mat dil_image;

	cv::dilate(originalImage, dil_image, element);

	cv::erode(dil_image, dil_image, element);

	

	cv::namedWindow( "Erode", cv::WINDOW_AUTOSIZE );

    cv::imshow( "Erode", dil_image );


    cv::waitKey( 0 );

	// Destroy the windows

	cv::destroyAllWindows();

	return 0;
}
