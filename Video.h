#pragma once
#include <filesystem>

#include "GrayFrame.h"
#include "opencv.h"

namespace term_vid_player {

namespace fs = std::filesystem;

class Video {
  private:
    fs::path const path;
    cv::VideoCapture cap;

    int length_, fps_, width_, height_;

  public:
    Video(fs::path path);
    Video(Video const&)            = delete;
    Video& operator=(Video const&) = delete;

    friend Video& operator>>(Video&, GrayFrame&);

    int length() const { return length_; }
    int fps() const { return fps_; }
    int width() const { return width_; }
    int height() const { return height_; }
};

Video& operator>>(Video&, GrayFrame&);

} // namespace term_vid_player
