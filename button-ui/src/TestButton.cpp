#include <iostream>
#include "hdr/button.hpp"

using namespace std;

int main() {
    for(;;) {
        cv::Mat3b img(640, 480, cv::Vec3b(255, 255, 255));
        Window test("test", img, 2);
        
        // Setup callback function
        Button test1("test", img, 2, 'S', "Button v0.1", 50,50, cv::Vec3b(200,200,200));
        Button test2("test", img, 2, 'M', "Button v0.2", 50,200, cv::Vec3b(0,200,0));
        if((char) cv::waitKey(1) == 'q') {
            break;
        }
    }
    cv::destroyAllWindows();
    return 0;
}

