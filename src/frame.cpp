#include "frame.h"
#include <iostream>
#include <fstream>

Frame::Frame (Mat source, int dotSize) {
    this->source = source;
    this->dotSize = dotSize;

    cvtColor(this->source, this->grayscale, COLOR_BGR2GRAY);
}


Mat Frame::getAsciiImage() {
    vector<string> ascii = this->getAscii();
    Mat asciiImage = Mat::zeros(ascii.size()*10, ascii[0].length()*10, this->source.type());

    int row = 0;
    for(auto const& line : ascii) {
        for(int i = 0;i < line.length(); i++) {
            putText(asciiImage, string(1, line.at(i)), Point(i*10, row), FONT_HERSHEY_PLAIN, 1.0, (255,255,255), 1, LINE_8);
        }
        row += 10;
    }

    return asciiImage;
}

vector<string> Frame::getAscii() {
    vector<string> ascii;

    int height = this->grayscale.size().height;
    int width = this->grayscale.size().width;

    int dotValue = 0;
    string line = "";
    for(int y = 0;y < height;y+=this->dotSize) {
        line = "";
        for(int x = 0; x < width;x+=this->dotSize) {
            if(y + (this->dotSize - 1) < height && x + (this->dotSize - 1) < width) {
                for(int i = 0;i < this->dotSize;i++) {
                    for(int j = 0;j < this->dotSize;j++) {
                        dotValue += this->grayscale.at<uchar>(i+y,j+x);
                    }
                }
            }

            dotValue /= pow(this->dotSize, 2);
            line += charMap[(255-dotValue)*10/256];
        }
        ascii.push_back(line);
    }

    return ascii;
}

Frame::~Frame() {

}

