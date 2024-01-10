#pragma once
#include <iostream>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

namespace term_vid_player {

class Terminal {
  private:
    winsize size;
    bool raw_mode = false;

    auto update() -> void { ioctl(STDOUT_FILENO, TIOCGWINSZ, &size); }

  public:
    Terminal() = default;
    ~Terminal() { turn_off_raw_mode(); }

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

    auto turn_on_raw_mode() -> void {
        if (raw_mode)
            return;
        struct termios raw;
        tcgetattr(STDIN_FILENO, &raw);

        raw.c_lflag &= ~(ECHO | ICANON);
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
        raw_mode = true;
    }

    auto turn_off_raw_mode() -> void {
        if (not raw_mode)
            return;
        struct termios origin;
        tcgetattr(STDIN_FILENO, &origin);
        raw_mode = false;
    }
};
} // namespace term_vid_player
