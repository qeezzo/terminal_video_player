#include "Video.h"
#include <filesystem>

using namespace term_vid_player;

Video::Video(fs::path path) : path(path), cap(path) {
    if (not cap.isOpened())
        throw std::runtime_error("Couldn't open file: " + path.string());

    length_ = cap.get(cv::CAP_PROP_FRAME_COUNT);
    fps_    = cap.get(cv::CAP_PROP_FPS);
    width_  = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    height_ = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
}

Video& term_vid_player::operator>>(Video& video, GrayFrame& gray_frame) {
    cv::Mat frame;
    video.cap >> frame;
    gray_frame = GrayFrame(frame);
    return video;
}
