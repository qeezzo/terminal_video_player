#include <iostream>
#include <opencv2/videoio.hpp>
#include <ostream>
#include <sys/ioctl.h>
#include <thread>
#include <timer.h>

#include "opencv.h"

using namespace cv;

std::array output{' ', '.', ':', '-', '=', '+', '*', '#', '%', '@'};

char get_output_char(uchar grey) {
    return output[ round(grey / 255.0 * (output.size() - 1)) ];
}

int main() {
    std::cout.sync_with_stdio(false);
    VideoCapture cap("/home/renat/Videos/bad_apple.mp4");

    if (not cap.isOpened()) {
        std::cout << "couldnt open the file" << std::endl;
        return -1;
    }

    const int length = cap.get(CAP_PROP_FRAME_COUNT);
    const int fps = cap.get(CAP_PROP_FPS);
    const int width = cap.get(CAP_PROP_FRAME_WIDTH);
    const int height = cap.get(CAP_PROP_FRAME_HEIGHT);

    int count{};
    for (Mat frame; count < length; cap >> frame) {
        Timer timer;
        if (frame.empty())
            continue;

        winsize size;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

        const int term_width = size.ws_col;
        const int term_height = size.ws_row * 2;
        const double w_scale = (double)term_width / width;
        const double h_scale = (double)term_height / height;
        const double scale = std::min(w_scale, h_scale);

        Mat gray_frame;
        cvtColor(frame, gray_frame, COLOR_BGR2GRAY);

        Mat scaled_gray_frame;
        const int scaled_width = width * scale;
        const int scaled_height = height * scale;
        resize(gray_frame, scaled_gray_frame,
               Size(scaled_width, scaled_height));

        int px_count{1};
        system("clear");
        std::ostringstream buffer;
        for (auto px = scaled_gray_frame.begin<uchar>(),
                  end = scaled_gray_frame.end<uchar>();
             px != end; ++px, ++px_count) {

            buffer << get_output_char(*px);
            if (px_count == scaled_width) {
                std::advance(px, scaled_width);
                buffer << "\n";
                px_count = 0;
            }
        }
        std::cout << buffer.str() << std::endl;

        auto sleep = std::chrono::milliseconds(1000 / fps - (int)(timer.elapsed()*1000));
        std::this_thread::sleep_for(sleep);
        count++;
    }

    return 0;
}
