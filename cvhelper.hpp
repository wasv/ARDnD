// The "Square Detector" program.
// It loads several images sequentially and tries to find squares in
// each image

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/video.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <math.h>
#include <string.h>

using namespace cv;
using namespace std;

int thresh = 50, N = 11;
const char* wndname = "Square Detection Demo";
const char* fltname = "Square Detection Demo - Gray";

static void applyFilter(const Mat& src, Mat& filter, const int filterNum) {
    assert(src.type() == CV_8UC3);
    
    Mat hsv;
    cvtColor(src,hsv,CV_BGR2HSV);

    switch(filterNum) {
      case 0:
        inRange(hsv, Scalar( 90,  32,  0), Scalar(120, 192, 255), filter); // Detects Cyan
        break;
      case 1:
        inRange(hsv, Scalar( 30,  64,  0), Scalar( 60, 192, 255), filter); // Detects Yellow
        break;
      case 2:
        inRange(hsv, Scalar(150, 127,  0), Scalar(180, 255, 255), filter); // Detects Pink
        break;
    }
}

// helper function:
// finds a cosine of angle between vectors
// from pt0->pt1 and from pt0->pt2
static double angle( Point pt1, Point pt2, Point pt0 )
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

// returns sequence of squares detected on the image.
// the sequence is stored in the specified memory storage
static void findSquares( const Mat& image, vector<vector<Point> >& squares )
{
    squares.clear();

    Mat pyr, timg, gray0(image.size(), CV_8U), gray;

    // down-scale and upscale the image to filter out the noise
    pyrDown(image, pyr, Size(image.cols/2, image.rows/2));
    pyrUp(pyr, timg, image.size());
    vector<vector<Point>> contours;


    // find squares in every color plane of the image
    for( int c = 0; c < 3; c++ )
    {
        Mat gray;
        dilate(image, gray, Mat(), Point(-1,-1));

        // find contours and store them all as a list
        findContours(gray, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

        vector<Point> approx;

        // test each contour
        for( size_t i = 0; i < contours.size(); i++ )
        {
            // approximate contour with accuracy proportional
            // to the contour perimeter
            approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);

            // square contours should have 4 vertices after approximation
            // relatively large area (to filter out noisy contours)
            // and be convex.
            // Note: absolute value of an area is used because
            // area may be positive or negative - in accordance with the
            // contour orientation
            if( approx.size() == 4 &&
                fabs(contourArea(Mat(approx))) > 1000 &&
                isContourConvex(Mat(approx)) )
            {
                double maxCosine = 0;

                for( int j = 2; j < 5; j++ )
                {
                    // find the maximum cosine of the angle between joint edges
                    double cosine = fabs(angle(approx[j%4], approx[j-2], approx[j-1]));
                    maxCosine = MAX(maxCosine, cosine);
                }

                // if cosines of all angles are small
                // (all angles are ~90 degree) then write quandrange
                // vertices to resultant sequence
                if( maxCosine < 0.3 )
                    squares.push_back(approx);
            }
        }
    }
}


// the function draws all the squares in the image
static void drawSquares( Mat& image, const vector<vector<Point>>& squares, Scalar color)
{
    for( size_t i = 0; i < squares.size(); i++ )
    {
        const Point* p = &squares[i][0];
        int n = (int)squares[i].size();
        polylines(image, &p, &n, 1, true, color, 3, LINE_AA);
    }

}

static void findAnchors( const vector<vector<Point>>& squares, vector<vector<Point>>& anchors) {
  anchors = vector<vector<Point>>(squares.size());

  for( size_t i = 0; i < squares.size(); i++) {
    Point c = (squares[i][0] + squares[i][2])/ 2; // Center point
    Point f = (squares[i][1] + squares[i][2])/ 2; // Midpoint of front edge
    Point l = (squares[i][2] + squares[i][3])/ 2; // Midpoint of left edge
    anchors[i].push_back(c);
    anchors[i].push_back(f);
    anchors[i].push_back(l);
  }
}

static void drawCenters( Mat& image, const vector<vector<Point>>& anchors, Scalar color) {
    for( size_t i = 0; i < anchors.size(); i++ )
    {
        circle(image, anchors[i][0], 5, color, 3);
        circle(image, anchors[i][1], 5, color, 3);
        circle(image, anchors[i][2], 5, color, 3);
    }
}

static void findObjects(Mat& image, vector<vector<float>>& poses) {
  vector<Scalar> colors;
  vector<vector<Point>> anchors;
  colors.push_back(Scalar(255,0,0));
  colors.push_back(Scalar(0,255,0));
  colors.push_back(Scalar(0,0,255));


  poses.clear();
  for(int fNum = 0; fNum < 3; fNum++) {
    Mat filter;
    vector<vector<Point>> squares;

    applyFilter(image, filter, fNum);   
    findSquares(filter, squares);
    findAnchors(squares, anchors);
    drawSquares(image, squares, colors[fNum]);
    drawCenters(image, anchors, colors[fNum]);

    for(int i = 0; i < anchors.size(); i++) {
      float dx, dy;
      vector<float> pose;
      pose.resize(5);

      pose[0] = (float)fNum;
      pose[1] = (float)anchors[i][0].x / image.cols;
      pose[2] = (float)anchors[i][0].y / image.rows;

      if(anchors[i][1].y > anchors[i][2].y) {
        dx = (anchors[i][1].x - anchors[i][0].x);
        dy = (anchors[i][1].y - anchors[i][0].y);
      } else {
        dx = (anchors[i][2].x - anchors[i][0].x);
        dy = (anchors[i][2].y - anchors[i][0].y);
      }

      pose[3] = -atan(dx/dy);
      pose[4] = 1.5*(dy/image.rows);
      poses.push_back(pose);
    }
  }
}
