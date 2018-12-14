#include <opencv2/opencv.hpp>

#include <zbar.h>

#include <iostream>

using namespace cv;
using namespace std;
using namespace zbar;

typedef struct {
  string type;
  string data;
  vector < Point > location;
}
decodedObject;

// Find and decode barcodes and QR codes
void decode(Mat & im, vector < decodedObject > & decodedObjects) {

  // Create zbar scanner
  ImageScanner scanner;

  // Configure scanner
  scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);

  // Convert image to grayscale
  Mat imGray;
  cvtColor(im, imGray, COLOR_BGR2GRAY);

  // Wrap image data in a zbar image
  Image image(im.cols, im.rows, "Y800", (uchar * ) imGray.data, im.cols * im.rows);

  // Scan the image for barcodes and QRCodes
  int n = scanner.scan(image);

  // Print results
  for (Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol) {
    decodedObject obj;

    obj.type = symbol -> get_type_name();
    obj.data = symbol -> get_data();

    // Print type and data
    cout << "Type : " << obj.type << endl;
    cout << "Data : " << obj.data << endl << endl;

    // Obtain location
    for (int i = 0; i < symbol -> get_location_size(); i++) {
      obj.location.push_back(Point(symbol -> get_location_x(i), symbol -> get_location_y(i)));
    }

    decodedObjects.push_back(obj);
  }
}

// Display barcode and QR code location  
void display(Mat & im, vector < decodedObject > & decodedObjects) {
  // Loop over all decoded objects
  for (int i = 0; i < decodedObjects.size(); i++) {
    vector < Point > points = decodedObjects[i].location;
    vector < Point > hull;

    // If the points do not form a quad, find convex hull
    if (points.size() > 4)
      convexHull(points, hull);
    else
      hull = points;

    // Number of points in the convex hull
    int n = hull.size();

    for (int j = 0; j < n; j++) {
      line(im, hull[j], hull[(j + 1) % n], Scalar(255, 0, 0), 3);
    }

  }

  // Display results 
  imshow("Results", im);

}

Point2f getCenter(vector<Point> points){
  Point2f baricenter(0,0);
  for(int i = 0; i < points.size(); i++){
    baricenter.x += points[i].x;
    baricenter.y += points[i].y;

  }
  baricenter.x /= points.size();
  baricenter.y /= points.size();
  return baricenter;
}

int main(int argc, char * argv[]) {

  // Read image
  Mat im = imread("page2.bmp");

  // Variable for decoded objects 
  vector < decodedObject > decodedObjects;

  // Find and decode barcodes and QR codes
  decode(im, decodedObjects);

  /*
  for (int i = 0; i < decodedObjects.size(); i++) {
    cout << decodedObjects[i].location << endl << endl;
    cout << getCenter( decodedObjects[i].location) << endl;
  }
  */
  // Display location 
  display(im, decodedObjects);

  decodedObject *topLeft, *topRight, *bottomLeft, *bottomRight;
  

  for (decodedObject i: decodedObjects) {
    if(i.data == "m0-v1-ihc19062017")
      topLeft = &i;
    else if(i.data == "m2")
      topRight = &i;
    else if(i.data == "m5")
      bottomLeft = &i;
    else
      bottomRight = &i;
  }

  Point2f inputQuad[4]; 
  Point2f outputQuad[4];

  inputQuad[0] = getCenter(topLeft->location);
  inputQuad[1] = getCenter(topRight->location);
  inputQuad[2] = getCenter(bottomLeft->location);
  inputQuad[3] = getCenter(bottomRight->location);

  outputQuad[0] = Point2f(300, 300);
  outputQuad[1] = Point2f(600, 300);
  outputQuad[2] = Point2f(300, 600);
  outputQuad[3] = Point2f(600, 600);

  Mat trans = getPerspectiveTransform(inputQuad, outputQuad);
  Mat transformedImage;
  warpPerspective(im, transformedImage, trans, Size(1000, 1000));

  imshow("Grid", transformedImage);
  waitKey(0);

  return EXIT_SUCCESS;
}