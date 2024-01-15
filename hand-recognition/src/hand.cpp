#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/background_segm.hpp> // Background Subtraction

using namespace std;

int cnt = 0;
char a[40];
const int width = 640;
const int height = 480;
int h_low = 50;
int s_low, v_low;
int h_high = 179;
int s_high = 255;
int v_high = 255;

int main() {
    // Checked for webcam
    cv::VideoCapture cap(0, cv::CAP_DSHOW);
    if (!cap.isOpened()) {
        cout << "Video camera capture status: Not Opened!!" << endl;
        return -1;
    }
    else {
        cout << "Video camera capture status: OK" << endl;
    }

    cap.set(cv::CAP_PROP_FRAME_WIDTH, width);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, height);

    cv::Size size (
        // Main Frame
        (int)cap.get(cv::CAP_PROP_FRAME_WIDTH),
        (int)cap.get(cv::CAP_PROP_FRAME_HEIGHT)
    );

    double fps = cap.get(cv::CAP_PROP_FPS);

    cout << "Captured frame: " << size.width << "(W)," << size.height << "(H)" << endl;

    cv::namedWindow( "Main", 1);
    
    // Background Subtraction
    cv::Ptr<cv::BackgroundSubtractorMOG2> pMOG2;
    vector<vector <cv::Point>> contours;
    pMOG2 = cv::createBackgroundSubtractorMOG2(30, false);

    cv::Mat src, gray, SrcROI, SrcHsv, fgMask, fgMask2;
    cv::Rect roi(width/3, height/3, 250, height*1/2);    

    for (;;) { 
        cv::namedWindow("Trackbar", 1);
        cap >> src;

        //ROI
        cv::rectangle(src, roi, cv::Scalar(255, 0, 0));
        cv::Mat ROI(src, roi);
        SrcROI = ROI;
        pMOG2->apply(ROI, ROI);
        cv::imshow("ROI", SrcROI);
        cv::imshow("pMOG_ROI", ROI);

        //HSV　H:H/2, S:S*1.0, V:V*0.95
        cv::createTrackbar("H[Low]", "Trackbar", &h_low, 179);
        cv::createTrackbar("H[High]", "Trackbar", &h_high, 179);
        cv::createTrackbar("S[Low]", "Trackbar", &s_low, 255);
        cv::createTrackbar("S[High]", "Trackbar", &s_high, 255);
        cv::createTrackbar("V[Low]", "Trackbar", &v_low, 255);
        cv::createTrackbar("V[High]", "Trackbar", &v_high, 255);
        cv::cvtColor(SrcROI, fgMask, cv::COLOR_BGR2HSV);
        cv::inRange(fgMask, cv::Scalar(h_low, s_low, v_low), cv::Scalar(h_high, s_high, v_high), fgMask);
        cv::imshow("HSV", fgMask);
        
/*
        // Gray(HSV)
        cv::Mat hsv_channels[3];
        cv::split( fgMask, hsv_channels );
        cv::imshow("HSV to gray", hsv_channels[2]);
        fgMask = hsv_channels[2];
*/
        
        // Smoothing
        cv::GaussianBlur(fgMask, fgMask, cv::Size(5, 5), 3, 3);

        // OTSU
        cv::threshold(fgMask, fgMask, 0, 255, (cv::THRESH_BINARY_INV + cv::THRESH_OTSU));
        // cv::imshow("Threshold2", fgMask);

        // Erosion & Dilation
        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5,5));
		cv::morphologyEx(fgMask, fgMask, cv::MORPH_OPEN, kernel, cv::Point(-1,-1), 2);
        cv::imshow("HSV-Opening", fgMask);
        // cv::Canny(fgMask, fgMask, ThreshholdDown, ThreshholdDown*2.5, 3);
        
/*
        cv::cvtColor(src, gray,     cv::COLOR_BGR2GRAY);
        cv::GaussianBlur(gray,gray,cv::Size(5, 5),3,3);
        cv::threshold(gray,gray,0,255,(cv::THRESH_BINARY_INV + cv::THRESH_OTSU));
        cv::Canny(gray, gray,ThreshholdDown, ThreshholdUp,3);
        cv::imshow( "Threshold",gray);
*/

        // Contours cv::Point contours[n][m]
        cv::findContours(fgMask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
        cv::Mat drawing = cv::Mat::zeros(fgMask.size(), CV_8UC3);
        vector<vector<cv::Point>> hullPoint(contours.size());
        vector<vector<cv::Vec4i>> defects(contours.size());
        vector<vector<int>> defectI(contours.size());
        vector<vector<cv::Point>> defectPoint(contours.size());

        for (size_t i=0; i<contours.size(); i++) {
            double area = cv::contourArea(contours.at(i));
            if (area > 5) {
                cv::convexHull(contours[i], hullPoint[i], true, false);
                cv::convexHull(contours[i], defectI[i], true, false);
                cv::drawContours(drawing, contours, i, cv::Scalar(0, 255, 0));
                cv::drawContours(drawing, hullPoint, i, cv::Scalar(0, 0, 255));
                // cout << cv::contourArea(contours[i]) << endl;

                cv::convexityDefects(contours[i], defectI[i], defects[i]);
                size_t j = 0;
                for (cnt = 0; j < defects[i].size(); j++){
                    if (defects[i][j][3] > 13 * 256){
                        int p_start = defects[i][j][0];
                        int p_end = defects[i][j][1];
                        int p_far = defects[i][j][2];
                        defectPoint[i].push_back(contours[i][p_far]);
                        cv::circle(drawing, contours[i][p_end], 10, cv::Scalar(255, 255, 0), 2);
                        cnt++;
                    }
                }
                // Finger
                if (cnt == 1) {strcpy_s(a, "1");}
                else if (cnt == 2) {strcpy_s(a, "2");}
                else if (cnt == 3) {strcpy_s(a, "3");}
                else if (cnt == 4) {strcpy_s(a, "4");}
                else if (cnt == 5 || cnt == 6) {strcpy_s(a, "5");}
                putText(src, a, cv::Point(75, 450), cv::FONT_HERSHEY_SIMPLEX, 3, cv::Scalar(255, 255, 255), 3, 8, false);
            }
        }
        cv::imshow("Main", src);
        cv::imshow("Conclusion", drawing);

        char c = (char) cv::waitKey(1);
        if (c == 'q') {
            break;
        }

    }
    cv::destroyAllWindows();
    return 0;
}
