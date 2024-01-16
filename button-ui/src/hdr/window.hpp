#ifndef WINDOW_H
#define WINDOW_H

#include <opencv2/opencv.hpp>

class Window{
    //Window (Once)
    std::string WindowName;
    cv::Mat3b   WindowBackgroundImage;
    int ButtonCounting;
    
public:
    Window(const std::string& name, cv::Mat3b& img, int number);
    
    std::string Window::get_WindowName() const { return WindowName; }

    cv::Mat3b Window::get_Background() const {return WindowBackgroundImage;}
};


class ExpandWindow : public Window{
    //Window (Once)
    std::string WindowName;
    cv::Mat3b   WindowBackgroundImage;
    
public:
    ExpandWindow(const std::string& name, cv::Mat3b& img);
    
    std::string ExpandWindow::get_WindowName() const {return WindowName;}

    cv::Mat3b ExpandWindow::get_Background() const {return WindowBackgroundImage;}
};

#endif