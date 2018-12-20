#include <opencv2/opencv.hpp>

#include "opencv2/xfeatures2d.hpp"

#include "opencv2/features2d.hpp"

#include <cmath>

#include "test.hpp"

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

const int MAX_FEATURES = 500;
const float GOOD_MATCH_PERCENT = 0.15f;

float getWhitePercentage(Mat im){
  return ((float)countNonZero(im))/(im.rows * im.cols) * 100; 
}

void alignImages(Mat & im1, Mat & im2, Mat & im1Reg, Mat & h)

{

	// Convert images to grayscale
	// Variables to store keypoints and descriptors
	std::vector < KeyPoint > keypoints1, keypoints2;
	Mat descriptors1, descriptors2;

	// Detect ORB features and compute descriptors.
	Ptr < Feature2D > orb = ORB::create(MAX_FEATURES);
	orb -> detectAndCompute(im1, Mat(), keypoints1, descriptors1);
	orb -> detectAndCompute(im2, Mat(), keypoints2, descriptors2);

	// Match features.
	std::vector < DMatch > matches;
	Ptr < DescriptorMatcher > matcher = DescriptorMatcher::create("BruteForce-Hamming");
	matcher -> match(descriptors1, descriptors2, matches, Mat());

	// Sort matches by score
	std::sort(matches.begin(), matches.end());

	// Remove not so good matches
	const int numGoodMatches = matches.size() * GOOD_MATCH_PERCENT;
	matches.erase(matches.begin() + numGoodMatches, matches.end());


	// Draw top matches
	Mat imMatches;
	drawMatches(im1, keypoints1, im2, keypoints2, matches, imMatches);
	imwrite("matches.jpg", imMatches);


	// Extract location of good matches
	std::vector < Point2f > points1, points2;

	for (size_t i = 0; i < matches.size(); i++) {
		points1.push_back(keypoints1[matches[i].queryIdx].pt);
		points2.push_back(keypoints2[matches[i].trainIdx].pt);
	}

	// Find homography
	h = findHomography(points1, points2, RANSAC);

	// Use homography to warp image
	warpPerspective(im1, im1Reg, h, im2.size());

}


int main(int argc, char ** argv) {
	// Read reference image
	string refFilename("page2.bmp");
	cout << "Reading reference image : " << refFilename << endl;
	Mat imReference = imread(refFilename);


	// Read image to be aligned
	string imFilename("IMG_20181217_160724.jpg");
	cout << "Reading image to align : " << imFilename << endl;
	Mat im = imread(imFilename);

	cvtColor(im, im, COLOR_BGR2GRAY);
	cvtColor(imReference, imReference, COLOR_BGR2GRAY);

	// Registered image will be resotred in imReg. 
	// The estimated homography will be stored in h. 
	Mat imReg, h;

	// Align images
	cout << "Aligning images ..." << endl;
	alignImages(im, imReference, imReg, h);

	// Write aligned image to disk. 
	string outFilename("aligned.jpg");
	cout << "Saving aligned image : " << outFilename << endl;
	imwrite(outFilename, imReg);

	// Print estimated homography
	cout << "Estimated homography : \n" << h << endl;


	Mat grid = imReg(Rect(138, 515, 510, 272));

	vector < Mat > lines;

	float lineHeigth = grid.rows / 15.0;
	float colWidth = grid.cols / 18.0;

	cout << colWidth << endl;

	threshold(grid, grid, 120, 255, THRESH_BINARY);
	threshold(grid, grid, 120, 255, THRESH_BINARY_INV);

	vector < Mat > cols;
	vector < Mat > a, b, c, d, v, f;
	Mat col;
	for (int i = 0; i < 18; i++) {

		if (i == 0 || (i % 15 == 0))
			continue;

		//cols.push_back(grid(Rect(round(i * colWidth), round(lineHeigth), round(colWidth), round(272 - lineHeigth))));

		col = grid(Rect(round(i * colWidth), round(lineHeigth), round(colWidth), round(272 - lineHeigth)));

		for (int k = 0; k < 14; k++) {
			switch (i) {
				case 1:
				case 6:
				case 11:
					a.push_back(col(Rect(0, k * lineHeigth, colWidth, lineHeigth)));
					break;

				case 2:
				case 7:
				case 12:
					b.push_back(col(Rect(0, k * lineHeigth, colWidth, lineHeigth)));
					break;

				case 3:
				case 8:
				case 13:
					c.push_back(col(Rect(0, k * lineHeigth, colWidth, lineHeigth)));
					break;

				case 4:
				case 9:
				case 14:
					d.push_back(col(Rect(0, k * lineHeigth, colWidth, lineHeigth)));
					break;

				case 16:
					if (k == 12) break;
					v.push_back(col(Rect(0, k * lineHeigth, colWidth, lineHeigth)));
					break;

				case 17:
					if (k == 12) break;
					f.push_back(col(Rect(0, k * lineHeigth, colWidth, lineHeigth)));
					break;
			}
		}

	}

  cout << "Size a: " << a.size() << endl;
	
  Rect border(Point(0, 0), f[4].size());
	Scalar color(0, 0, 0);
	int thickness = 2;
	rectangle(f[4], border, color, thickness);
  for(int i = 0; i < 42; i++){
    rectangle(a[i], border, color, thickness);
    rectangle(b[i], border, color, thickness);
    rectangle(c[i], border, color, thickness);
    rectangle(d[i], border, color, thickness);
    
    if(i < 12){
      rectangle(f[i], border, color, thickness);
      rectangle(v[i], border, color, thickness);
    }
  }

  vector<OptionsMult> answersMult;
  vector<OptionsVF> answersVF;

  
  for(int i = 0; i < 42; i++){
    vector<OptionsMult> tmpAns;
    if(getWhitePercentage(a[i]) > 5.0 && getWhitePercentage(a[i]) < 60){
      tmpAns.push_back(OptionsMult::A);
    }
    if(getWhitePercentage(b[i]) > 5.0 && getWhitePercentage(b[i]) < 60){
      tmpAns.push_back(OptionsMult::B);
    }
    if(getWhitePercentage(c[i]) > 5.0 && getWhitePercentage(c[i]) < 60){
      tmpAns.push_back(OptionsMult::C);
    }
    if(getWhitePercentage(d[i]) > 5.0 && getWhitePercentage(d[i]) < 60){
      tmpAns.push_back(OptionsMult::D);
    }
    if(tmpAns.size() == 1){
      answersMult.push_back(tmpAns[0]);
    }
    else{
      answersMult.push_back(OptionsMult::NMUL);
    }
  }

  
  for(int i = 0; i < 12; i++){
    vector<OptionsVF> tmpAns;
    if(getWhitePercentage(v[i]) > 7.5 && getWhitePercentage(v[i]) < 80){
      tmpAns.push_back(OptionsVF::V);
    }
    if(getWhitePercentage(f[i]) > 7.5 && getWhitePercentage(f[i]) < 80){
      tmpAns.push_back(OptionsVF::F);
    }
    if(tmpAns.size() == 1){
      answersVF.push_back(tmpAns[0]);
    }
    else{
      answersVF.push_back(OptionsVF::NVF);
    }
  }
  
  int cnt = 0;

  cout << "Multiple Choice:" <<endl << endl;
  
  for(OptionsMult i : answersMult){
    cnt++;
    switch(i){
      case OptionsMult::A:
        cout << cnt << " :A" << endl;
        break;
      case OptionsMult::B:
        cout << cnt << " :B" << endl;
        break;
      case OptionsMult::C:
        cout << cnt << " :C" << endl;
        break; 
      case OptionsMult::D:
        cout << cnt << " :D" << endl;
        break; 
      case OptionsMult::NMUL:
        cout << cnt << " :N" << endl;
        break; 
      }
    }

  cnt = 0;
  cout << endl << endl << "True or False: " <<endl << endl;
  for(OptionsVF i : answersVF){
    cnt++;
    switch(i){
      case OptionsVF::V:
        cout << cnt << " :V" << endl;
        break;
      case OptionsVF::F:
        cout << cnt << " :F" << endl;
        break;
      case OptionsVF::NVF:
        cout << cnt << " :N" << endl;
        break; 
    }
  }

  Test test = Test();
  cout << "Score: " << test.getScore(answersMult, answersVF) << endl;


	//RGB = BGR
	Scalar orange(0,165,255);
	Scalar red(0,0,200);
	Scalar green(0,200,0);

	threshold(grid, grid, 120, 255, THRESH_BINARY);
	threshold(grid, grid, 120, 255, THRESH_BINARY_INV);
	cvtColor(grid, grid, COLOR_GRAY2BGR);
	
	for(int i = 0; i < 42; i++){
		Point tmpPoint;
		if(answersMult[i] == test.correctMult[i]){
			switch(answersMult[i]){
				case OptionsMult::A:
					tmpPoint.x = colWidth * ((i/14)*5 + 1);
					tmpPoint.y = lineHeigth*(i%14 + 1);
					rectangle(grid, tmpPoint, Point(tmpPoint.x + colWidth, tmpPoint.y + lineHeigth), green, 2);
					break;
				case OptionsMult::B:
					tmpPoint.x = colWidth * ((i/14)*5 + 2);
					tmpPoint.y = lineHeigth*(i%14 + 1);
					rectangle(grid, tmpPoint, Point(tmpPoint.x + colWidth, tmpPoint.y + lineHeigth), green, 2);
					break;
				case OptionsMult::C:
					tmpPoint.x = colWidth * ((i/14)*5 + 3);
					tmpPoint.y = lineHeigth*(i%14 + 1);
					rectangle(grid, tmpPoint, Point(tmpPoint.x + colWidth, tmpPoint.y + lineHeigth), green, 2);
					break;
				case OptionsMult::D:
					tmpPoint.x = colWidth * ((i/14)*5 + 4);
					tmpPoint.y = lineHeigth*(i%14 + 1);
					rectangle(grid, tmpPoint, Point(tmpPoint.x + colWidth, tmpPoint.y + lineHeigth), green, 2);
					break;
			}
		}
		else if(answersMult[i] == OptionsMult::NMUL){
			switch(test.correctMult[i]){
				case OptionsMult::A:
					tmpPoint.x = colWidth * ((i/14)*5 + 1);
					tmpPoint.y = lineHeigth*(i%14 + 1);
					rectangle(grid, tmpPoint, Point(tmpPoint.x + colWidth, tmpPoint.y + lineHeigth), orange, 2);
					break;
				case OptionsMult::B:
					tmpPoint.x = colWidth * ((i/14)*5 + 2);
					tmpPoint.y = lineHeigth*(i%14 + 1);
					rectangle(grid, tmpPoint, Point(tmpPoint.x + colWidth, tmpPoint.y + lineHeigth), orange, 2);
					break;
				case OptionsMult::C:
					tmpPoint.x = colWidth * ((i/14)*5 + 3);
					tmpPoint.y = lineHeigth*(i%14 + 1);
					rectangle(grid, tmpPoint, Point(tmpPoint.x + colWidth, tmpPoint.y + lineHeigth), orange, 2);
					break;
				case OptionsMult::D:
					tmpPoint.x = colWidth * ((i/14)*5 + 4);
					tmpPoint.y = lineHeigth*(i%14 + 1);
					rectangle(grid, tmpPoint, Point(tmpPoint.x + colWidth, tmpPoint.y + lineHeigth), orange, 2);
					break;
			}
		}
		else{
			switch(answersMult[i]){
				case OptionsMult::A:
					tmpPoint.x = colWidth * ((i/14)*5 + 1);
					tmpPoint.y = lineHeigth*(i%14 + 1);
					rectangle(grid, tmpPoint, Point(tmpPoint.x + colWidth, tmpPoint.y + lineHeigth), red, 2);
					break;
				case OptionsMult::B:
					tmpPoint.x = colWidth * ((i/14)*5 + 2);
					tmpPoint.y = lineHeigth*(i%14 + 1);
					rectangle(grid, tmpPoint, Point(tmpPoint.x + colWidth, tmpPoint.y + lineHeigth), red, 2);
					break;
				case OptionsMult::C:
					tmpPoint.x = colWidth * ((i/14)*5 + 3);
					tmpPoint.y = lineHeigth*(i%14 + 1);
					rectangle(grid, tmpPoint, Point(tmpPoint.x + colWidth, tmpPoint.y + lineHeigth), red, 2);
					break;
				case OptionsMult::D:
					tmpPoint.x = colWidth * ((i/14)*5 + 4);
					tmpPoint.y = lineHeigth*(i%14 + 1);
					rectangle(grid, tmpPoint, Point(tmpPoint.x + colWidth, tmpPoint.y + lineHeigth), red, 2);
					break;
			}
			switch(test.correctMult[i]){
				case OptionsMult::A:
					tmpPoint.x = colWidth * ((i/14)*5 + 1);
					tmpPoint.y = lineHeigth*(i%14 + 1);
					rectangle(grid, tmpPoint, Point(tmpPoint.x + colWidth, tmpPoint.y + lineHeigth), green, 2);
					break;
				case OptionsMult::B:
					tmpPoint.x = colWidth * ((i/14)*5 + 2);
					tmpPoint.y = lineHeigth*(i%14 + 1);
					rectangle(grid, tmpPoint, Point(tmpPoint.x + colWidth, tmpPoint.y + lineHeigth), green, 2);
					break;
				case OptionsMult::C:
					tmpPoint.x = colWidth * ((i/14)*5 + 3);
					tmpPoint.y = lineHeigth*(i%14 + 1);
					rectangle(grid, tmpPoint, Point(tmpPoint.x + colWidth, tmpPoint.y + lineHeigth), green, 2);
					break;
				case OptionsMult::D:
					tmpPoint.x = colWidth * ((i/14)*5 + 4);
					tmpPoint.y = lineHeigth*(i%14 + 1);
					rectangle(grid, tmpPoint, Point(tmpPoint.x + colWidth, tmpPoint.y + lineHeigth), green, 2);
					break;
			}
		}
	}


	for(int i = 0; i < 12; i++){
		Point tmpPoint;
		if(answersVF[i] == test.correctVF[i]){
			switch(answersVF[i]){
				case OptionsVF::V:
					tmpPoint.x = colWidth * 16;
					tmpPoint.y = lineHeigth*(i+1);
					rectangle(grid, tmpPoint, Point(tmpPoint.x + colWidth, tmpPoint.y + lineHeigth), green, 2);
					break;
				case OptionsVF::F:
					tmpPoint.x = colWidth * 17;
					tmpPoint.y = lineHeigth*(i%14 + 1);
					rectangle(grid, tmpPoint, Point(tmpPoint.x + colWidth, tmpPoint.y + lineHeigth), green, 2);
					break;
			}
		}
		else if(answersVF[i] == OptionsVF::NVF){
			switch(test.correctVF[i]){
				case OptionsVF::V:
					tmpPoint.x = colWidth * 16;
					tmpPoint.y = lineHeigth*(i+1);
					rectangle(grid, tmpPoint, Point(tmpPoint.x + colWidth, tmpPoint.y + lineHeigth), orange, 2);
					break;
				case OptionsVF::F:
					tmpPoint.x = colWidth * 17;
					tmpPoint.y = lineHeigth*(i%14 + 1);
					rectangle(grid, tmpPoint, Point(tmpPoint.x + colWidth, tmpPoint.y + lineHeigth), orange, 2);
					break;
			}
		}
		else{
			switch(test.correctVF[i]){
				case OptionsVF::V:
					tmpPoint.x = colWidth * 16;
					tmpPoint.y = lineHeigth*(i+1);
					rectangle(grid, tmpPoint, Point(tmpPoint.x + colWidth, tmpPoint.y + lineHeigth), green, 2);
					break;
				case OptionsVF::F:
					tmpPoint.x = colWidth * 17;
					tmpPoint.y = lineHeigth*(i%14 + 1);
					rectangle(grid, tmpPoint, Point(tmpPoint.x + colWidth, tmpPoint.y + lineHeigth), green, 2);
					break;
			}
			switch(answersVF[i]){
				case OptionsVF::V:
					tmpPoint.x = colWidth * 16;
					tmpPoint.y = lineHeigth*(i+1);
					rectangle(grid, tmpPoint, Point(tmpPoint.x + colWidth, tmpPoint.y + lineHeigth), red, 2);
					break;
				case OptionsVF::F:
					tmpPoint.x = colWidth * 17;
					tmpPoint.y = lineHeigth*(i%14 + 1);
					rectangle(grid, tmpPoint, Point(tmpPoint.x + colWidth, tmpPoint.y + lineHeigth), red, 2);
					break;
			}
		}
	}

	cvtColor(imReg, imReg, COLOR_GRAY2BGR);	
	grid.copyTo(imReg(Rect(138, 515, grid.cols, grid.rows)));
	namedWindow("Display frame", WINDOW_NORMAL);
	imshow("Display frame", imReg);
	//imshow("Grid", imReg);
	waitKey(0);

}