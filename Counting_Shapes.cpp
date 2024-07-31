
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;

Mat src, src_gray;
Mat dst, detected_edges;

int lowThreshold = 50;
const int max_lowThreshold = 100;
const int ratio = 3;
const int kernel_size = 3;
const char *window_name = "Edge Map";
const char *window_name1 = "opening";

void CannyThreshold(int, void *);

int main(int argc, char **argv)
{

    src = imread("circles.png"); // Load an image

    if (src.empty())
    {
        std::cout << "Could not open or find the image!\n"
                  << std::endl;
        return -1;
    }

    // dst.create(src.size(), src.type());

    cvtColor(src, src_gray, COLOR_BGR2GRAY);
    blur(src_gray, detected_edges, Size(3, 3));
    //  threshold(detected_edges,detected_edges,100,255,0);
    namedWindow(window_name, WINDOW_AUTOSIZE);
    // createTrackbar("Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold);

    CannyThreshold(0, 0);

    waitKey();

    return 0;
}
void CannyThreshold(int, void *)
{

    Mat canny_output;
    Canny(detected_edges, canny_output, 100, 200);
    const char *canny = "canny";
    namedWindow(canny);

    Mat element = getStructuringElement(0, Size(3, 3), Point(1, 1));

    morphologyEx(canny_output, canny_output, 3, element);
    imshow(canny, canny_output);
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);
    vector<vector<Point>> contours_poly(contours.size());
    vector<Rect> boundrect(contours.size());
    vector<Point2f> centers(contours.size());
    vector<float> radius(contours.size());
    for (auto i = 0; i < contours.size(); i++)
    {
        approxPolyDP(contours[i], contours_poly[i], 3, true);
        boundrect[i] = boundingRect(contours_poly[i]);
        minEnclosingCircle(contours_poly[i], centers[i], radius[i]);
    }
    Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
    for (size_t i = 0; i < contours.size(); i+=2)
    {
        // if((int)radius[i]>45){
        Scalar color = Scalar(255, 0, 255);
        drawContours(drawing, contours, (int)i, color, 2, LINE_8, hierarchy, 0);
        rectangle(drawing, boundrect[i].tl(), boundrect[i].br(), Scalar(0, 255, 255), 2);
        circle(drawing, centers[i], (int)radius[i], Scalar(0, 255, 255), 2);
        cout << "radius = "<<radius[i] << endl;
        circle(drawing, centers[i], 1, Scalar(0, 255, 255), -1);
        line(drawing,centers[i],Point(centers[i].x,centers[i].y+radius[i]),Scalar(0,255,255),2,LINE_8);
        cout<<"area = "<<contourArea(contours[i])<<endl;
        cout<<"perimeter of arc = "<<arcLength(contours[i],true)<<endl;
        // }
    }
    cout << contours.size() / 2 << endl;
    imshow(window_name, drawing);
}