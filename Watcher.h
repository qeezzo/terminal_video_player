#pragma once

#include "Player.h"
#include <unordered_map>

namespace term_vid_player {

class Watcher {
  private:
    std::unordered_map<char, void (Watcher::*)()> keys{
        {'q', &Watcher::stop},
        {' ', &Watcher::play_pause},
        {'l', &Watcher::forward},
        {'h', &Watcher::backward},
    };

    Player& player;
    bool paused = false;

    auto stop() -> void;
    auto play_pause() -> void;
    auto forward() -> void;
    auto backward() -> void;

  public:
    Watcher(Player& player) : player(player) {}

    auto watch() -> void;
};

} // namespace term_vid_player
