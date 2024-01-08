#pragma once
#include <array>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

#include "Terminal.h"
#include "Video.h"
#include "opencv.h"

namespace term_vid_player {

class Config {
  private:
    std::unordered_map<std::string, void (Config::*)()> keys{};
    std::string file;

  public:
    Config() = default;
    Config(int argc, char* argv[]) {
        if (argc == 1)
            return;

        for (int i = 1; i < argc; ++i) {
            char* arg = argv[i];

            if (arg[0] != '-') {
                if (not file.empty()) {
                    std::cerr
                        << "encounter several src files; should be only one"
                        << std::endl;
                    std::exit(64);
                }
                file = arg;
                continue;
            }

            if (not keys.contains(arg)) {
                std::cerr << "unknown argument: " << arg << std::endl;
                std::exit(64);
            }
            (this->*keys[arg])();
        }
    }

    auto source() const -> std::string const& { return file; }
};

enum class Event { pause, play, stop };

// const player cant be paused
class Player {
  private:
    // darker --(next char)--> lighter
    constexpr static std::array output{' ', '.', ':', '-', '=',
                                       '+', '*', '#', '%', '@'};

    char get_output_char(uchar grey) const {
        return output[round(grey / 255.0 * (output.size() - 1))];
    }

    std::atomic<bool> running_;
    std::queue<Event> events;
    std::mutex mutex;
    std::thread player;

    Terminal term;
    Video video;
    int count{};
    Config config;

    // TODO: add exception handling
    auto run() -> void;
    auto next_frame() -> GrayFrame;
    auto process_frame(GrayFrame&) -> GrayFrame&;
    auto get_frame_string(GrayFrame const&, std::string&) -> void;
    auto output_frame_string(std::string&) -> void;

    auto push_event(Event) -> Player&;

  public:
    Player(Player const&)            = delete;
    Player& operator=(Player const&) = delete;
    ~Player() {
        if (player.joinable())
            player.join();
    }

    Player(Config config) : video(config.source()), config(std::move(config)) {}

    // interface
    auto play() -> Player&;
    auto pause() -> Player&;
    auto stop() -> Player&;

    auto running() -> bool { return running_.load(); }
};
} // namespace term_vid_player
