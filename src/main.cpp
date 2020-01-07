#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>

#include "frame.h"

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    string input;
    string output;
    int downscaleFactor;
    int dotSize;
    int outputFps;

    if(argc == 1) {
        cout << "Parameters: \n" << "input output [downscale_factor] [dotsize] [outputFps]" << endl;
        return 0;
    }
    else {
        input = argv[1];
        output = argv[2];

        if(output.find(".avi") == string::npos) {
            cout << "Ouput file must be of type .avi" << endl;
            return -1;
        }

        try {
            downscaleFactor = atoi(argv[3]);
            dotSize = atoi(argv[4]);
            outputFps = atoi(argv[5]);

        }
        catch(const exception&) {
            downscaleFactor = 1;
            dotSize = 10;
            outputFps = 1;
        }
    }


    VideoCapture inputVideo(input);
    if(!inputVideo.isOpened()) {
        cout << "Could not open video file" << endl;
        return -1;
    }

    int width = inputVideo.get(CAP_PROP_FRAME_WIDTH);
    int height = inputVideo.get(CAP_PROP_FRAME_HEIGHT);
    int fps = inputVideo.get(CAP_PROP_FPS);
    int length = inputVideo.get(CAP_PROP_FRAME_COUNT)/outputFps;
    VideoWriter outputVideo(output, VideoWriter::fourcc('M','J','P','G'), fps/outputFps, Size(width/downscaleFactor/dotSize * 10, height/downscaleFactor/dotSize * 10));

    for(int i = 0;i < length;i+=outputFps) {
        Mat frame;
        inputVideo >> frame;

        if(frame.empty())
            break;

        Frame *frameObj = new Frame(frame, downscaleFactor, dotSize);
        outputVideo.write(frameObj->getAsciiImage());
        imshow("Ascii", frameObj->getAsciiImage());
        imshow("Originial", frame);

        cout << i << "/" << length << endl;

        char c = static_cast<char>(waitKey(1));
        if(c==27) {
            inputVideo.release();
            outputVideo.release();

            cout << "Program has been forcefully quit using the esc-key" << endl;
            return 0;
        }

        delete frameObj;
    }

    inputVideo.release();
    outputVideo.release();

    cout << input << " has been transformed into an ascii-video. Output: " << output << endl;
}
