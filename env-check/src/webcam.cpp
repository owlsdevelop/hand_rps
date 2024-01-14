#include <iostream>
#include <opencv2/opencv.hpp>

int main()
{
    cv::VideoCapture cap(0);
    cv::Mat frame;

    if (!cap.isOpened())
    {
        return -1;
    }

      std::cout << cv::getVersionString() << std::endl;
    
    while (true) {
        cap.read(frame);
        cv::imshow("Cap", frame);

        char c = (char) cv::waitKey(1);
        if(c == 'q'){
            break;
        }
    }
}