#include "Player.h"
#include <ostream>

using namespace term_vid_player;

auto Player::output_frame_string(std::string& buf) -> void {
    term.clear();
    std::cout << buf << std::flush;
    buf.clear();
}

auto Player::get_frame_string(GrayFrame const& frame, std::string& buf)
    -> void {
    for (int row = 0; row < frame.height(); ++row) {
        if (row % 2)
            continue;
        for (int col = 0; col < frame.width(); ++col) {
            buf += get_output_char(frame.at(row, col));
        }
        buf += '\n';
    }
    buf.pop_back();
}

auto Player::process_frame(GrayFrame& frame) -> GrayFrame& {
    // adaptive scaling
    double const scaleX = (double)term.width() / frame.width();
    double const scaleY = (double)term.height() / frame.height() * 2;
    double const scale  = std::min(scaleX, scaleY);
    frame.scale(scale);
    return frame;
}

auto Player::next_frame() -> GrayFrame {
    GrayFrame frame;

    video >> frame;
    if (count++ == video.length())
        stop();

    return frame;
}

auto Player::run() -> void {

    // avoids many heap allocation requests
    std::string buffer;
    bool paused = false;

    const auto gap = std::chrono::milliseconds(1000 / video.fps());
    auto sync      = std::chrono::steady_clock::now();
    for (;;) {

        { // handle events
            std::lock_guard lock(mutex);
            if (not events.empty()) {
                switch (events.front()) {
                case Event::play: paused = false; break;
                case Event::pause: paused = true; break;
                case Event::stop: running_.store(false); return;
                }
                events.pop();
            }
        }

        if (paused) {
            std::this_thread::yield();
            sync = std::chrono::steady_clock::now();
            continue;
        }

        sync += gap;

        if (std::chrono::steady_clock::now() > sync)
            continue;

        auto frame = next_frame();
        if (frame.empty())
            continue;

        process_frame(frame);
        get_frame_string(frame, buffer);

        std::this_thread::sleep_until(sync);
        output_frame_string(buffer);
    }
}

auto Player::push_event(Event event) -> Player& {
    std::lock_guard lock(mutex);
    events.push(event);
    return *this;
}

auto Player::play() -> Player& {
    if (not running_.load()) {
        running_.store(true);
        player = std::thread(&Player::run, this);
    }
    return push_event(Event::play);
}

auto Player::pause() -> Player& { return push_event(Event::pause); }

auto Player::stop() -> Player& {
    return push_event(Event::stop);
}
