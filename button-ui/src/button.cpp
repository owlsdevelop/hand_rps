#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "hdr/button.hpp"

using namespace std;

int num;
cv::Rect RectButton;
cv::Mat3b canvas;
cv::Mat3b Image;
string Name;
vector<cv::Rect> Rect;

Window::Window(const string& name, cv::Mat3b& img, int number)
                :WindowName(name), WindowBackgroundImage(img), ButtonCounting(number) {
    this->WindowName = WindowName;
    this->WindowBackgroundImage = WindowBackgroundImage;
    this->ButtonCounting = ButtonCounting;
    Name = WindowName;
    Image = WindowBackgroundImage;
    num = number;
}


Button::Button(const string& name, cv::Mat3b& img, int number, const char& ButtonSize, 
                const string& ButtonText,const int& ButtonPosition_x,const int& ButtonPosition_y, cv::Vec3b& RectColor)
                :Window(name, img, number) {
    this->ButtonText = ButtonText;
    this->RectColor = RectColor;

    switch (ButtonSize) {
        case 'S':
            RectButton = cv::Rect( ButtonPosition_x, ButtonPosition_y, 300, 50);
            break;
        case 'M':
            RectButton = cv::Rect( ButtonPosition_x, ButtonPosition_y, 300, 100);
            break;
        case 'L':
            RectButton = cv::Rect( ButtonPosition_x, ButtonPosition_y, 300, 150);
            break;
        default:
            cout << "S or M or L \n" ; 
            break;
    }
    Rect.push_back(RectButton);
    SetMouseClick();
}

void MouseCallback(int event, int x, int y, int flags, void* param) {
    switch (event) {
        case cv::EVENT_LBUTTONDOWN: {
            for (int i = 1; i <= num; i++) {
                if (Rect[1].contains(cv::Point(x, y))) {
                    cout << "Clicked!" << endl;
                    //rectangle(mouse.canvas(mouse.RectButton), mouse.RectButton, cv::Scalar(0,0,255), 2);
                    for(;;) {
                    cv::VideoCapture cap(0,cv::CAP_DSHOW);
                    cv::Mat src22;
                    cap >> src22;
                    cv::imshow("Camera", src22); 
                        if((char) cv::waitKey(1) == 'k') {
                            cv::destroyAllWindows();
                            break;
                        }
                    }

                }

                if (Rect[2].contains(cv::Point(x, y))) {
                    cout << "Peace!!!!!" << endl;
                    //rectangle(mouse.canvas(mouse.RectButton), mouse.RectButton, cv::Scalar(0,0,255), 2);
                    cv::Mat canvas2 = cv::imread("../img/RPS2.png", -1);
                    cv::imshow("V sign", canvas2); 
                } 
            }break;
        case cv::EVENT_RBUTTONDOWN: {
                cout << "(x, y) = " << x << ',' << y;
            } break;
        }
    } 
    cv::waitKey(1);
}


void Button::SetMouseClick(){
    canvas = cv::Mat3b(RectButton.width+RectButton.height, RectButton.width+RectButton.height, RectColor);
    // Draw the button
    putText(canvas(RectButton), ButtonText, cv::Point(RectButton.width*0.35, RectButton.height*0.6), cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0,0,0));
    cv::Mat3b InsetImage(get_Background(), RectButton);
    canvas(RectButton).copyTo(InsetImage);
    // Setup callback function
    cv::namedWindow(get_WindowName());
    cv::setMouseCallback(get_WindowName(), MouseCallback);
    cv::imshow(get_WindowName(), get_Background());
}
