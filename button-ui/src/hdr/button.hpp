#ifndef BUTTON_H
#define BUTTON_H

#include <opencv2/opencv.hpp>
#include "window.hpp"

class Button : public Window {
    char ButtonSize;
    std::string ButtonText;
    cv::Vec3b RectColor;
    int ButtonPosition_x;
    int ButtonPosition_y;

public:
    Button(const std::string& name, cv::Mat3b& img, int number, const char& ButtonSize, 
    const std::string& ButtonText, const int& ButtonPosition_x, const int& ButtonPosition_y, cv::Vec3b& RectColor);
    void SetMouseClick();
};


#endif