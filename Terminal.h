#pragma once
#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>

namespace term_vid_player {

class Terminal {
  private:
    winsize size;

    auto update() -> void { ioctl(STDOUT_FILENO, TIOCGWINSZ, &size); }

  public:
    Terminal() = default;

    auto width() const -> unsigned short { return size.ws_col; }
    auto width() -> unsigned short {
        update();
        return size.ws_col;
    }

    auto height() const -> unsigned short { return size.ws_row; }
    auto height() -> unsigned short {
        update();
        return size.ws_row;
    }

    auto clear() -> void { std::cout << "\e[2J\e[H"; }
};
} // namespace term_vid_player
