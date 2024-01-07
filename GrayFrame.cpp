#include "GrayFrame.h"
#include <opencv2/imgproc.hpp>

using namespace term_vid_player;

GrayFrame::GrayFrame(cv::Mat frame) : frame(frame) {
    if (not empty())
        to_grayscale();
}

auto GrayFrame::to_grayscale() const -> void {
    cv::Mat gray;
    // TODO: expand conversions: bgr, ...
    cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
    frame = gray;
}
auto GrayFrame::resize(int width, int height) -> void {
    cv::Mat scaled;
    cv::resize(frame, scaled, cv::Size(width, height));
    frame = scaled;
}
auto GrayFrame::scale(double scale) -> GrayFrame& {
    cv::Mat scaled;
    cv::resize(frame, scaled, cv::Size(width() * scale, height() * scale));
    frame = scaled;
    return *this;
}
