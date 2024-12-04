#include "Watcher.h"

using namespace term_vid_player;

auto Watcher::watch() -> void {

    Terminal term;
    term.turn_on_raw_mode(); // turn off automatically

    char key;
    while (player.running()) {
        std::cin.read(&key, 1);
        if (keys.contains(key)) {
            (this->*keys[key])();
        }
        if (key == 'q')
            break;
    }
}

auto Watcher::stop() -> void { player.stop(); }

auto Watcher::play_pause() -> void {
    if (paused)
        player.play();
    else
        player.pause();
    paused = not paused;
}

auto Watcher::forward() -> void { player.forward(); }

auto Watcher::backward() -> void { player.backward(); }
