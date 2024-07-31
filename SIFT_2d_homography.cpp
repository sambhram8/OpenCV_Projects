#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char **argv)
{
    Mat input1 = imread("virat_face.png"); // Load as grayscale
    Mat input2 = imread("virat.png");
    Mat img1=input1.clone();
    Mat img2=input2.clone();
    
    cvtColor(input2,input2,COLOR_BGR2GRAY);
    Ptr<SIFT> siftPtr = SIFT::create();
    vector<KeyPoint> keypoints1, keypoints2;
    Mat desc1, desc2;
    siftPtr->detectAndCompute(input1, noArray(), keypoints1, desc1);
    siftPtr->detectAndCompute(input2, noArray(), keypoints2, desc2);

    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
    vector<vector<DMatch>> knn_matches;
    matcher->knnMatch(desc1, desc2, knn_matches, 2);

    const float ratio_thresh = 0.7f;
    vector<DMatch> good_matches;
    for (size_t i = 0; i < knn_matches.size(); i++)
    {
        if (knn_matches[i][0].distance < ratio_thresh * knn_matches[i][1].distance)
        {
            good_matches.push_back(knn_matches[i][0]);
        }
    }

    Mat img_matches;
    drawMatches(input1, keypoints1, input2, keypoints2, good_matches, img_matches, Scalar::all(-1), Scalar::all(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

    cout << good_matches.size() << endl;
    vector<Point2f> obj;
    vector<Point2f> scene;
    for (size_t i = 0; i < good_matches.size(); i++)
    {
        obj.push_back(keypoints1[good_matches[i].queryIdx].pt);
        scene.push_back(keypoints2[good_matches[i].trainIdx].pt);
    }

    Mat H;
    H = findHomography(obj, scene);

    vector<Point2f> obj_corners;
    obj_corners.push_back(Point2f(0, 0));
    obj_corners.push_back(Point2f((float)input1.cols, 0));
    obj_corners.push_back(Point2d((float)input1.cols, (float)input1.rows));
    obj_corners.push_back(Point2f(0, (float)input1.rows));
    vector<Point2f> scene_corners;

    perspectiveTransform(obj_corners, scene_corners, H);

    circle(img2, scene_corners[0], 10, Scalar(255, 0, 255), -1);
    // circle(img2, scene_corners[1], 10, Scalar(255, 0, 255), -1);
    // circle(img2, scene_corners[2], 10, Scalar(255, 0, 255), -1);
    // circle(img2, scene_corners[3], 10, Scalar(255, 0, 255), -1);

    line(img2,scene_corners[0],scene_corners[1],Scalar(0,255,0),4);
    line(img2,scene_corners[1],scene_corners[2],Scalar(0,255,0),4);
    line(img2,scene_corners[2],scene_corners[3],Scalar(0,255,0),4);
    line(img2,scene_corners[3],scene_corners[0],Scalar(0,255,0),4);
    resize(img2, img2, Size(500, 500), INTER_LINEAR);
    imshow("new", img2);
    resize(img_matches, img_matches, Size(500, 500), INTER_LINEAR);
    // cvtColor(img_matches,img_matches,COLOR_GRAY2BGR);
    imshow("match", img_matches);

    // Add results to image and save.
    // Mat output;
    // drawKeypoints(input, keypoints, output);
    // imshow("new",output);
    waitKey(0);
    // cout<<desc.size()<<endl;

    return 0;
}