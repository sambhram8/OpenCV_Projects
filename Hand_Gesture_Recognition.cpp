#include <opencv2/opencv.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/aruco_detector.hpp>
#include <stdio.h>
#include <iostream>
#include <cmath>

using namespace std;
using namespace cv;

const char *wind = "hand";
const char *wind1 = "hand1";
Mat img, img_gray, img_threshold;

int main()
{
    VideoCapture cam(0);
    while (1)
    {
        bool b = cam.read(img);
        // img=img(Rect(340,100,270,270));
        // rectangle()
        // img = imread("hand_img2.jpg");
        // if (img.empty())
        // {
        //     cout << "image not read";
        //     exit(0);
        // }
        // imshow(wind1, img);

        cvtColor(img, img_gray, COLOR_BGR2GRAY);
        GaussianBlur(img_gray, img_gray, Size(19, 19), 0.0, 0);
        threshold(img_gray, img_threshold, 0, 255, THRESH_BINARY_INV + THRESH_OTSU);
        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;
        findContours(img_threshold, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point());
        if (contours.size())
        {
            size_t indexofBiggestContour = 0;
            size_t sizeofBiggestContour = 0;
            for (size_t i = 0; i < contours.size(); i++)
            {
                cout << contours[i].size() << endl;
                cout << sizeofBiggestContour << endl;
                if (contours[i].size() > sizeofBiggestContour)
                {
                    indexofBiggestContour = i;
                    cout << "ind " << indexofBiggestContour << endl;
                    sizeofBiggestContour = contours[i].size();
                }
            }
            cout << "index=" << indexofBiggestContour << endl;
            cout << "contours size" << contours.size() << endl;
            vector<vector<int>> hull(contours.size());
            vector<vector<Point>> hullPoints(contours.size());
            vector<vector<Vec4i>> defects(contours.size());
            vector<vector<Point>> defectPoint(contours.size());
            vector<vector<Point>> contours_poly(contours.size());
            Point2f rect_point[4];
            vector<RotatedRect> minRect(contours.size());
            vector<Rect> boundRect(contours.size());
            int count = 0;
            for (size_t i = 0; i < contours.size(); i++)
            {
                if (contourArea(contours[i]) > 5000)
                {
                    convexHull(contours[i], hull[i], true);
                    convexityDefects(contours[i], hull[i], defects[i]);
                    if (indexofBiggestContour == i)
                    {
                        minRect[i] = minAreaRect(contours[i]);
                        for (size_t k = 0; k < hull[i].size(); k++)
                        {
                            int ind = hull[i][k];
                            hullPoints[i].push_back(contours[i][ind]);
                        }

                        for (size_t k = 0; k < defects[i].size() - 1; k++)
                        {
                            int p_start = defects[i][k][0]; // index of the starting point of the defect
                            int p_end = defects[i][k][1];   // index of the ending point of the defect
                            int p_far = defects[i][k][2];   // index of the farthest point of the defect
                            defectPoint[i].push_back(contours[i][p_far]);
                            // int a = sqrt(pow((defects[i][k][1] - defects[i][k][0]), 2) + pow((defects[i][k + 1][1] - defects[i][k + 1][0]), 2));
                            // int b = sqrt(pow((defects[i][k][2] - defects[i][k][0]), 2) + pow((defects[i][k + 1][2] - defects[i][k + 1][0]), 2));
                            // int c = sqrt(pow((defects[i][k][1] - defects[i][k][2]), 2) + pow((defects[i][k + 1][1] - defects[i][k + 1][2]), 2));
                            // int angle = cos(((b * b) + (c * c) - (a * a)) / (2 * b * c)) * 57;
                            // if (angle <= 90)
                            // {
                            //     count = count + 1;
                            //     circle(img, contours[i][p_start], 3, Scalar(255, 0, 0), 2);
                            //     circle(img, contours[i][p_end], 3, Scalar(0, 255, 0), 2);
                            //     circle(img, contours[i][p_far], 3, Scalar(0, 0, 255), 2);
                            // }
                            if (defects[i][k][3] > 13 * 256)
                            {
                               count=count+1;
                                circle(img, contours[i][p_start], 3, Scalar(255, 0, 0), 2);
                                circle(img, contours[i][p_end], 3, Scalar(0, 255, 0), 2);
                                circle(img, contours[i][p_far], 3, Scalar(0, 0, 255), 2);
                               
                            }
                        }
                        if (count == 1)
                        {
                            cout << "1" << endl;
                        }
                        else if (count == 2)
                        {
                            cout << "2" << endl;
                        }
                        else if (count == 3)
                        {
                            cout << "3" << endl;
                        }
                        else if (count == 4)
                        {
                            cout << "4" << endl;
                        }
                        else if (count == 5)
                        {
                            cout << "5" << endl;
                        }
                        else
                        {
                            cout << "not detected" << endl;
                        }
                        cout << "defects size" << defects.size() << endl;
                        cout << "count=" << count << endl;
                    }
                }
            }

            // drawContours(img,contours,indexofBiggestContour,Scalar(255,0,255),10,LINE_8,hierarchy,0);
            // cout<<indexofBiggestContour<<endl;
            // cout<<contours.size()<<endl;
        }
        else
        {
            cout << "shitt" << endl;
        }

        namedWindow(wind, WINDOW_AUTOSIZE);
        imshow(wind, img);
        // waitKey(0);
        if (waitKey(30) == 'q')
        {
            return -1;
        }
    }
    // createTrackbar("Min Threshold:", wind, &lowThreshold, max_lowThreshold, CannyThreshold);
}
