#pragma once
#include "opencv.h"

namespace term_vid_player {

class GrayFrame {
  private:
    mutable cv::Mat frame;

    auto to_grayscale() const -> void;

  public:
    using Iterator      = cv::MatIterator_<uchar>;
    using ConstIterator = cv::MatConstIterator_<uchar>;

    auto begin() -> Iterator { return frame.begin<uchar>(); }
    auto end() -> Iterator { return frame.end<uchar>(); }
    auto begin() const -> ConstIterator { return frame.begin<uchar>(); }
    auto end() const -> ConstIterator { return frame.end<uchar>(); }

    GrayFrame()                                  = default;
    GrayFrame(GrayFrame const& other)            = default;
    GrayFrame& operator=(GrayFrame const& other) = default;
    GrayFrame(cv::Mat frame);

    auto resize(int width, int height) -> void;
    auto scale(double scale) -> void;

    auto width() const { return frame.cols; }
    auto height() const { return frame.rows; }
    auto empty() const -> bool { return frame.empty(); }
};
} // namespace term_vid_player
