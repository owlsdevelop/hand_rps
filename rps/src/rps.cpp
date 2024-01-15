#include <iostream>
#include <chrono>
#include <thread>
#include <time.h>
#include <stdlib.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/background_segm.hpp> // Background Subtraction

using namespace std;

int cnt = 0;
char a[40];
const int width = 720;
const int height = 540;
int h_low = 50;
int s_low, v_low;
int h_high = 179;
int s_high = 255;
int v_high = 255;
int bar_switch_off = 0;
int bar_switch_on = 1;
int bar_switch_value;
int CheckArea = 5;
int flag = 0;
int RPSFlag = 0;
int RPSHand = 0;
int RPSStart = 0;
int RPSBetween = 0;
int win_no = 0;
int lose_no = 0;
int draw_no = 0;
int comp = 0;
string RPSJudge;
string COMJudge;

vector<cv::Rect> RectSum;

cv::Mat canvas = cv::imread("../img/button1.png", -1);
cv::Mat canvas2 = cv::imread("../img/button2.png", -1);
cv::Mat BackgroundMain = cv::imread("../img/white.png", -1);
cv::Mat judge1 = cv::imread("../img/first1.png", -1);
cv::Mat judge2 = cv::imread("../img/first2.png", -1);
cv::Mat judge3 = cv::imread("../img/first3.png", -1);
cv::Mat rps1 = cv::imread("../img/RPS1.png", -1);
cv::Mat rps2 = cv::imread("../img/RPS2.png", -1);
cv::Mat rps3 = cv::imread("../img/RPS3.png", -1);
cv::Mat result = cv::imread("../img/result.png", -1);

cv::Rect RectButton = cv::Rect(740, 50, 200, 50);
cv::Rect RectButton2 = cv::Rect(740, 450, 200, 50);

auto now = chrono::system_clock::now();
time_t Start_time = chrono::system_clock::to_time_t(now);

// Overlay images
void PutPng(cv::Mat &frame, cv::Mat &png, int x, int y) {
    vector<cv::Mat> layers;
    cv::split(png, layers);
    cv::Mat bgr;
    cv::merge(layers.data(), 3, bgr);
    cv::Mat mask = layers[3];
    cv::Mat roi = frame(cv::Rect(x, y, bgr.cols, bgr.rows));
    bgr.copyTo(roi, mask);
}

void MouseCallback(int event, int x, int y, int flags, void* data) {
    switch (event) {    
        case cv::EVENT_LBUTTONDOWN: { 
            for (int i = 0; i <= 2; i++) {
                if (RectSum[1].contains(cv::Point(x, y))) {  
                    flag = 1;
                }
                if (RectSum[0].contains(cv::Point(x, y))) { 
                    cv::destroyAllWindows();
                    flag = 2;
                    RPSStart = 0;
                    RPSBetween = 0;
                }
            }
        } break;
        case cv::EVENT_RBUTTONDOWN: {
                cout << "(x, y) = " << x << ',' << y;
            } break;
        }
    cv::waitKey(1);
}

void initialize(void) {
    srand(time(NULL));
    comp = rand() % 3;

    if (comp == 0) {COMJudge = "Rock";}
    if (comp == 1) {COMJudge = "Scissors";}
    if (comp == 2) {COMJudge = "Paper";}
}

void count_no(int result) {
    switch (result) {
        case 0:
            cout << "Draw"<< endl;
            draw_no++;
            break;
        case 1:
            cout << "Lose"<< endl;
            lose_no++;
            break;
        case 2:
            cout << "Win"<< endl;
            win_no++;
            break;
    }
}


int main() { 
    RectSum.push_back(RectButton);
    RectSum.push_back(RectButton2);

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

    cv::Size size(
        // Main Frame
        (int)cap.get(cv::CAP_PROP_FRAME_WIDTH),
        (int)cap.get(cv::CAP_PROP_FRAME_HEIGHT)
    );

    double fps = cap.get(cv::CAP_PROP_FPS);

    cout << "Captured frame: " << size.width << "(W)," << size.height << "(H)" << endl;

    // Background Subtraction
    cv::Ptr<cv::BackgroundSubtractorMOG2> pMOG2;
    vector<vector <cv::Point>> contours;
    pMOG2 = cv::createBackgroundSubtractorMOG2(30, false);

    cv::Mat src, gray, SrcROI, SrcHsv, fgMask, fgMask2;
    cv::Rect MainRoi(0,0,width,height);
    cv::Rect roi(width/3, height/3, height*1/2, height*1/2);    
    cout << "Current Time and Date: " << ctime(&Start_time) << endl;

    for (;;) {
        cv::namedWindow("Trackbar", cv::WINDOW_FREERATIO);
        cap >> src;

        // ROI
        cv::rectangle(src, roi, cv::Scalar(255, 0, 0));
        cv::Mat ROI(src, roi);
        SrcROI = ROI;
        pMOG2->apply(ROI, ROI);
        
        // HSV　H:H/2, S:S*1.0, V:V*0.95
        cv::createTrackbar("H[Low]", "Trackbar", &h_low, 179);
        cv::createTrackbar("H[High]", "Trackbar", &h_high, 179);
        cv::createTrackbar("S[Low]", "Trackbar", &s_low, 255);
        cv::createTrackbar("S[High]", "Trackbar", &s_high, 255);
        cv::createTrackbar("V[Low]", "Trackbar", &v_low, 255);
        cv::createTrackbar("V[High]", "Trackbar", &v_high, 255);
        cv::createTrackbar("Area", "Trackbar", &CheckArea, 5000);
        cv::cvtColor(SrcROI, fgMask, cv::COLOR_BGR2HSV);
        cv::inRange(fgMask, cv::Scalar(h_low, s_low, v_low), cv::Scalar(h_high, s_high, v_high), fgMask);
                
        if (flag == 1) {
            cv::imshow("ROI", SrcROI);
            cv::imshow("pMOG_ROI", ROI);
            cv::imshow("HSV", fgMask);
            cv::imshow("Debug", src);
        }

        // Smoothing
        cv::GaussianBlur(fgMask, fgMask, cv::Size(5, 5), 3, 3);

        // OTSU
        cv::threshold(fgMask, fgMask, 0, 255, (cv::THRESH_BINARY_INV + cv::THRESH_OTSU));
        
        if (flag == 1) {
            cv::imshow("Threshold2", fgMask);
        }

        // Erosion & Dilation
        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5,5));
		cv::morphologyEx(fgMask, fgMask, cv::MORPH_OPEN, kernel, cv::Point(-1,-1), 2);
        
        if (flag == 1) {
            cv::imshow("HSV-Opening", fgMask);
        }

        // Contours cv::Point contours[n][m]
        cv::findContours(fgMask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
        cv::Mat drawing = cv::Mat::zeros(fgMask.size(), CV_8UC3);
        vector<vector<cv::Point>>   hullPoint(contours.size());
        vector<vector<cv::Vec4i>>  defects(contours.size());
        vector<vector<int> >        defectI(contours.size());
        vector<vector<cv::Point>>  defectPoint(contours.size());

        for (size_t i = 0; i < contours.size(); i++) {
            double area = cv::contourArea(contours.at(i));
            if (area > CheckArea) {
                cv::convexHull(contours[i], hullPoint[i], true, false);
                cv::convexHull(contours[i], defectI[i], true, false);
                cv::drawContours(drawing, contours, i, cv::Scalar(0, 255, 0));
                cv::drawContours(SrcROI, contours, i, cv::Scalar(0, 255, 0));
                cv::drawContours(drawing, hullPoint, i, cv::Scalar(0, 0, 255));
                cv::drawContours(SrcROI, hullPoint, i, cv::Scalar(0, 0, 255));
                //cout << cv::contourArea(contours[i]) << endl;

                cv::convexityDefects(contours[i], defectI[i], defects[i]);
                size_t j = 0;
                for (cnt = 0; j < defects[i].size(); j++){
                    if (defects[i][j][3] > 13 * 256){
                        int p_start = defects[i][j][0];
                        int p_end = defects[i][j][1];
                        int p_far = defects[i][j][2];
                        defectPoint[i].push_back(contours[i][p_far]);
                        cv::circle(drawing, contours[i][p_end], 10, cv::Scalar(255, 255, 0), 2);
                        cv::circle(SrcROI, contours[i][p_end], 10, cv::Scalar(255, 255, 0), 2);
                        cnt++;
                    }
                }

                // Finger
                if (cnt == 0 && area > 4000) {strcpy_s(a, "0"); RPSHand = 0;}
                else if (cnt == 1) {strcpy_s(a, "1"); RPSHand = 0;}
                else if (cnt == 2) {strcpy_s(a, "2"); RPSHand = 1;}
                else if (cnt == 3) {strcpy_s(a, "3"); RPSHand = 1;}
                else if (cnt == 4) {strcpy_s(a, "4"); RPSHand = 2;}
                else if (cnt == 5 || cnt == 6) {strcpy_s(a, "5"); RPSHand = 2;}
                else {//cout << cnt << endl;
                };
                cv::putText(src, a, cv::Point(75, 450), cv::FONT_HERSHEY_SIMPLEX, 3, cv::Scalar(255, 255, 255), 3, 8, false);
                if (RPSHand == 0) {
                    RPSJudge = "Rock";
                    RPSStart++;
                    if (RPSFlag) {
                        RPSBetween++;
                    }
                }
                if (RPSHand == 1) {
                    RPSJudge = "Scissors";
                    if (RPSFlag) {
                        RPSBetween++;
                    }
                }
                if (RPSHand == 2) {
                    RPSJudge = "Paper";
                    if (RPSFlag) {
                        RPSBetween++;
                    }
                }
                cv::putText(src, RPSJudge, cv::Point(height*1/2, 450), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 3, 8, false);
            }
        }
        cv::Mat img_roi = src(roi);
        SrcROI.copyTo(img_roi);
        
        int x = 0;
        int y = 0;
        if (flag == 2) {
            PutPng(src, judge1, 100, y);
            if (RPSStart >= 20 && RPSStart <= 30) {
                RPSFlag = 1;
                RPSBetween = 0;
            }
            if (RPSFlag == 1) {PutPng(src, judge2, 100, y);}
            if (RPSBetween >= 40) {
                PutPng(src, judge3, 100, y);
                int human = RPSHand;
                cv::imwrite("your-hand.png", SrcROI);
                // cout << " image has been saved " << endl;
                cv::Mat JudgePicture = cv::imread("your-hand.png", -1);
                cv::resize(JudgePicture, JudgePicture, cv::Size(), 500/JudgePicture.cols, 500/JudgePicture.rows);
                initialize();
                cout << "COM:" << COMJudge << ", You:" << RPSJudge << endl;

                int judge = (human - comp + 3) % 3;

                count_no(judge);
                cout << win_no << "Win, " << lose_no << "lose, " << draw_no << "draw" << endl;
                cv::imshow("Result", JudgePicture);
                flag = 0;
                RPSFlag = 0;
            }

        }

        cv::resize(src, src, cv::Size(MainRoi.width, MainRoi.height));
        src.copyTo(BackgroundMain(MainRoi)); 
        
        cv::resize(canvas, canvas, cv::Size(RectButton.width, RectButton.height));
        canvas.copyTo(BackgroundMain(RectButton));
        cv::resize(canvas2, canvas2, cv::Size(RectButton2.width, RectButton2.height));
        canvas2.copyTo(BackgroundMain(RectButton2));

        cv::setMouseCallback("RPS-main", MouseCallback);
        cv::imshow("RPS-main", BackgroundMain);
        if (flag == 1) {
            cv::imshow("Conclusion", drawing);
        }

        char c = (char) cv::waitKey(1);
        if (c == 'q') {
            break;
        }
    }
    cv::destroyAllWindows();
    return 0;
}
