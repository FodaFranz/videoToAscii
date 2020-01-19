#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <vector>
#include <fstream>

#include "frame.h"

using namespace std;
using namespace cv;

vector<Mat> scaleVideo(VideoCapture video, int factor, int fpsMultiplier) {
     int length = video.get(CAP_PROP_FRAME_COUNT);
     vector<Mat> outVideo;
     cout << "Downscaling video ..." << endl;
     for(int i = 0;i < length;i+=fpsMultiplier) {
         Mat frame, outFrame;
         video.set(CAP_PROP_POS_FRAMES, i);
         video >> frame;
         resize(frame, outFrame, Size(frame.cols/factor, frame.rows/factor), factor, factor, INTER_LINEAR);
         outVideo.push_back(outFrame);
     }

     return outVideo;
}

int main(int argc, char *argv[])
{
    string input;
    string output;
    int scalingFactor;
    int dotSize;
    int outputFpsMultiplier;

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
            scalingFactor = atoi(argv[3]);
            dotSize = atoi(argv[4]);
            outputFpsMultiplier = atoi(argv[5]);

        }
        catch(const exception&) {
            scalingFactor = 1;
            dotSize = 10;
            outputFpsMultiplier = 1;
        }
    }


    VideoCapture inputVideo(input);
    if(!inputVideo.isOpened()) {
        cout << "Could not open video file" << endl;
        return -1;
    }

    int fps = inputVideo.get(CAP_PROP_FPS);

    vector<Mat> sourceVideo = scaleVideo(inputVideo, scalingFactor, outputFpsMultiplier);
    Mat exampleFrame = sourceVideo[100];
    VideoWriter outputVideo(output,
                            VideoWriter::fourcc('M','J','P','G'),
                            fps/outputFpsMultiplier,
                            Size(exampleFrame.cols/dotSize * 10, exampleFrame.rows/dotSize * 10));

    cout << "Converting to ascii..." << endl;
    for(Mat const& image : sourceVideo) {
        Frame* frame = new Frame(image, dotSize);
        outputVideo.write(frame->getAsciiImage());
    }

    outputVideo.release();
    inputVideo.release();

    cout << input << " has been transformed into an ascii-video. Output: " << output << endl;
}
