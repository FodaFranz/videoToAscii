#ifndef FRAME_H
#define FRAME_H

#include <opencv2/opencv.hpp>
#include <string.h>
#include <vector>
#include <cmath>

using namespace std;
using namespace cv;

class Frame
{
public:
    Frame(Mat source, int dotSize = 1);
    ~Frame();

    Mat getAsciiImage();

private:
    Mat source;
    Mat grayscale;
    const string charMap = " .,:;ox%#@";
    int dotSize;

    vector<string> getAscii();
};

#endif // FRAME_H
