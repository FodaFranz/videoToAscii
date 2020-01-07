#include "frame.h"

Frame::Frame (Mat source, int downScaleFactor, int dotSize) {
    this->source = source;
    this->dotSize = dotSize;

    downscale(downScaleFactor);

    cvtColor(this->source, this->grayscale, COLOR_BGR2GRAY);
}

void Frame::downscale(int factor) {
    Mat scaledImage;
    resize(this->source, scaledImage, Size(this->source.size().width/factor, this->source.size().height/factor), factor, factor, INTER_NEAREST);

    scaledImage.copyTo(source);
}

Mat Frame::getAsciiImage() {
    if(!this->asciiImage.empty()) {
        return this->asciiImage;
    }

    vector<string> ascii = this->getAscii();

    this->asciiImage = Mat::zeros(ascii.size()*10, ascii[0].length()*10, this->source.type());
    int row = 0;
    for(auto const& line : ascii) {
        for(int i = 0;i < line.length(); i++) {
            string character = line[i] + "";
            putText(this->asciiImage, string(1, line.at(i)), Point(i*10, row), FONT_HERSHEY_PLAIN, 1.0, (255,255,255,255), 1, LINE_4);
        }
        row += 10;
    }

    return this->asciiImage;
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
