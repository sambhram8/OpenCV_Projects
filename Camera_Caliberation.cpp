#include <opencv2/opencv.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/aruco_detector.hpp>
#include <stdio.h>
#include <iostream>

using namespace std;
using namespace cv;

int checkboard[2]{6, 9};

int main()
{
  //camera caliberation

  vector<vector<Point3f>> objpoints;
  vector<vector<Point2f>> imgpoints;
  vector<Point3f> objp;

  for (auto i = 0; i < checkboard[1]; i++)
  {
    for (auto j = 0; j < checkboard[0]; j++)
    {
      objp.push_back(Point3f(j, i, 0));
    }
  }

  vector<String> images;
  string path = "./images/*.png";
  glob(path, images);
  vector<Point2f> corner_pts;
  Mat frame, gray;
  for (int i = 0; i < images.size(); i++)
  {
    frame = imread(images[i]);
    cvtColor(frame, gray, COLOR_BGR2GRAY);

    bool success = findChessboardCorners(gray, Size(checkboard[0], checkboard[1]), corner_pts, CALIB_CB_ADAPTIVE_THRESH);

    cout << success << endl;
    if (success)
    {
      cornerSubPix(gray, corner_pts, Size(11, 11), Size(-1, -1), TermCriteria(TermCriteria::EPS + TermCriteria::COUNT, 30, 0.0001));
      drawChessboardCorners(frame, Size(checkboard[0], checkboard[1]), corner_pts, success);
      objpoints.push_back(objp);
      imgpoints.push_back(corner_pts);
    }

    // imshow("image", frame);
    // waitKey(0);
  }
  Mat cameraMatrix, distCoeffs, R, T;

  /*
   * Performing camera calibration by
   * passing the value of known 3D points (objpoints)
   * and corresponding pixel coordinates of the
   * detected corners (imgpoints)
   */
  calibrateCamera(objpoints, imgpoints, Size(gray.rows, gray.cols), cameraMatrix, distCoeffs, R, T);

  cout << "cameraMatrix : " << cameraMatrix << endl;
  cout << "distCoeffs : " << distCoeffs << endl;
  cout << "Rotation vector : " << R << endl;
  cout << "Translation vector : " << T << endl;

//to get the undistorted image uding distCoeffs from camera caliberation

  // for(auto i=0;i<images.size();i++){
  //     Mat dst,map1,map2,newCamMat,frame1;
  //     frame1=imread(images[i]);
  //    fisheye::estimateNewCameraMatrixForUndistortRectify(cameraMatrix, distCoeffs, Size(frame1.cols,frame1.rows),
  //  Matx33d::eye(), newCamMat, 1);
  //   fisheye::undistortImage(frame1, dst, cameraMatrix, distCoeffs, newCamMat);
  // //   remap(frame1,dst,map1,map2,INTER_LINEAR);
  //   imshow("undistorted",dst);
  // waitKey(0);
  // }



  // to generate aruco marker using predefined dict

  //  Mat markerimage;
  //  aruco::Dictionary dictionary=aruco::getPredefinedDictionary(aruco::DICT_6X6_250);
  //  aruco::generateImageMarker(dictionary,23,200,markerimage,1);
  //  imwrite("marker23.png",markerimage);
  //  imshow("aruco",markerimage);
  //  waitKey(0);

  // aruco marker detection

  Mat aruco_img;
  aruco_img = imread("charuco_board.png");
  // int top= (int)(0.10*aruco_img.rows);
  // int bottom=top;
  // int left=(int)(0.10*aruco_img.cols);
  // int right=left;
  // copyMakeBorder(aruco_img,aruco_img,top,bottom,left,right,BORDER_CONSTANT,Scalar(255,255,255));
  // vector<int> markerIds;
  // vector<vector<Point2f>> marker_corners, rejected_candidates;
  // aruco::DetectorParameters detector_params = aruco::DetectorParameters();
  // aruco::Dictionary dictionary = aruco::getPredefinedDictionary(aruco::DICT_6X6_250);
  // aruco::ArucoDetector detector(dictionary, detector_params);
  // detector.detectMarkers(aruco_img, marker_corners, markerIds, rejected_candidates);
  // aruco::drawDetectedMarkers(aruco_img, marker_corners, markerIds);
  // //to show the rejected candidates
  // aruco::drawDetectedMarkers(aruco_img,rejected_candidates,noArray(),Scalar(255,0,255));
  // cout << marker_corners.size() << endl;
  // cout << markerIds.size() << endl;
  // //these corner coordinates can be used for homography
  // cout<<marker_corners[0][0]<<" "<<marker_corners[0][1]<<" "<<marker_corners[0][2]<<" "<<marker_corners[0][3]<<endl;

  // aruco marker pose estimation

  // float markerLength = 0.0785;
  // Mat object_points(4, 1, CV_32FC3);
  // object_points.ptr<Vec3f>(0)[0] = Vec3f(-markerLength / 2.f, markerLength / 2.f, 0);
  // object_points.ptr<Vec3f>(0)[1] = Vec3f(markerLength / 2.f, markerLength / 2.f, 0);
  // object_points.ptr<Vec3f>(0)[2] = Vec3f(markerLength / 2.f, -markerLength / 2.f, 0);
  // object_points.ptr<Vec3f>(0)[3] = Vec3f(-markerLength / 2.f, -markerLength / 2.f, 0);
  // vector<Vec3d> rvecs(marker_corners.size()), tvecs(marker_corners.size());

  // for (auto i = 0; i < marker_corners.size(); i++)
  // {
  //   solvePnP(object_points, marker_corners.at(i), cameraMatrix, distCoeffs, rvecs.at(i), tvecs.at(i));
  // }
  // for (auto i = 0; i < markerIds.size(); i++)
  // {
  //   drawFrameAxes(aruco_img, cameraMatrix, distCoeffs, rvecs[i], tvecs[i], markerLength * 1.5f, 2);
  // }


  //for aruco board
//   int markersX=5; //number of markers along X
//   int markersY=7; //number of markers along Y
//   float markerLength=0.0585; //length of a marker
//   float markerSeperation=0.0100; //seperation distance between two markers
//   aruco::Dictionary dictionary=aruco::getPredefinedDictionary(aruco::DICT_6X6_250);
//   aruco::DetectorParameters detectorParams=aruco::DetectorParameters();
//   aruco::ArucoDetector detector(dictionary,detectorParams);
//   vector<vector<Point2f>> corners,rejected;
//   vector<int> ids;
//   Vec3d rvec,tvec;
//   detector.detectMarkers(aruco_img,corners,ids,rejected);
//    cout<<ids.size()<<endl;
//   aruco::GridBoard board(Size(markersX,markersY),markerLength,markerSeperation,dictionary);
  
//     detector.refineDetectedMarkers(aruco_img,board,corners,ids,rejected,cameraMatrix,distCoeffs);
 
//   int markerOfBoardDetected=0;
//   if(!ids.empty()){
//     Mat objectpoints,imagepoints;
//     board.matchImagePoints(corners,ids,objectpoints,imagepoints);

//     solvePnP(objectpoints,imagepoints,cameraMatrix,distCoeffs,rvec,tvec);
//     markerOfBoardDetected=(int)objectpoints.total()/4;
    
//     aruco::drawDetectedMarkers(aruco_img,corners,ids);

//     aruco::drawDetectedMarkers(aruco_img,rejected,noArray(),Scalar(200,0,200));
//      float axisLength = 0.5f * ((float)min(markersX, markersY) * (markerLength + markerSeperation) +
//  markerSeperation);
//     drawFrameAxes(aruco_img,cameraMatrix,distCoeffs,rvec,tvec,axisLength);
//   }

  
 int squaresX = 5;
 int squaresY = 7;
 float squareLength = 0.300;
 float markerLength = 0.150;
 bool refine = true;
 
 aruco::DetectorParameters detectorparams = aruco::DetectorParameters();
 aruco::Dictionary dictionary = aruco::getPredefinedDictionary(aruco::DICT_6X6_250);
 
 float axisLength = 0.5f * ((float)min(squaresX, squaresY) * (squareLength));
aruco::CharucoBoard charucoBoard(Size(squaresX, squaresY), squareLength, markerLength, dictionary);

aruco::CharucoParameters charucoParams;
charucoParams.tryRefineMarkers=true;
charucoParams.cameraMatrix=cameraMatrix;
charucoParams.distCoeffs=distCoeffs;
aruco::CharucoDetector charucoDetector(charucoBoard ,charucoParams,detectorparams);

 vector<int> markerIds, charucoIds;
 vector<vector<Point2f> > markerCorners;
 vector<Point2f> charucoCorners;
 Vec3d rvec, tvec;

 charucoDetector.detectBoard(aruco_img,charucoCorners,charucoIds,markerCorners,markerIds);
 aruco::interpolateCornersCharuco(markerCorners,markerIds,aruco_img,board,charucoCorners,charucoIds,cameraMatrix,distCoeffs);
 cout<<charucoCorners.size()<<" "<<markerIds.size()<<endl;
aruco::drawDetectedMarkers(aruco_img, markerCorners);
aruco::drawDetectedCornersCharuco(aruco_img, charucoCorners, charucoIds, cv::Scalar(255, 0, 0));


 Mat objPoints, imgPoints;
 charucoBoard.matchImagePoints(charucoCorners, charucoIds, objPoints, imgPoints);
 validpose = solvePnP(objPoints, imgPoints, cameraMatrix, distCoeffs, rvec, tvec);
 drawFrameAxes(aruco_img, cameraMatrix, distCoeffs, rvec, tvec, axisLength);
  imshow("aruco_makers", aruco_img);
  waitKey(0);
}