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

    auto length() const -> int { return length_; }
    auto fps() const -> int { return fps_; }
    auto width() const -> int { return width_; }
    auto height() const -> int { return height_; }

    auto get_next_frame_pos() -> int {
        return cap.get(cv::CAP_PROP_POS_FRAMES);
    }
    auto set_next_frame_pos(int pos) -> bool {
        return cap.set(cv::CAP_PROP_POS_FRAMES, pos);
    }
};

Video& operator>>(Video&, GrayFrame&);

} // namespace term_vid_player
