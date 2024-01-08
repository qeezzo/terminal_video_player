#include "Watcher.h"
#include <termios.h>
#include <unistd.h>

using namespace term_vid_player;

auto Watcher::watch() -> void {

    struct termios raw;
    tcgetattr(STDIN_FILENO, &raw);

    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

    char key;
    while (player.running()) {
        std::cin.read(&key, 1);
        if (keys.contains(key)) {
            (this->*keys[key])();
        }
        if (key == 'q') break;
    }
}

auto Watcher::stop() -> void {
    player.stop();
}

auto Watcher::play_pause() -> void {
    if (paused) player.play();
    else player.pause();
    paused = not paused;
}
