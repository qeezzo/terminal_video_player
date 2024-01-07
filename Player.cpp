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
    int px_count{1};
    for (auto px = frame.begin(), end = frame.end(); px != end; ++px) {

        buf += get_output_char(*px);
        if (px_count == frame.width()) {
            // skip one row cuz assume size of row is as size of two columns
            // thats why scaleY is multiplied by 2
            std::advance(px, frame.width());

            buf += "\n";
            px_count = 0;
        }
        ++px_count;
    }
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
    while (frame.empty() and count++ < video.length())
        video >> frame;

    if (count >= video.length())
        stop();

    return frame;
}

auto Player::run() -> void {

    // avoids many heap allocation requests
    std::string buffer;
    bool paused = false;

    const auto gap = std::chrono::milliseconds(1000 / video.fps());
    auto sync = std::chrono::steady_clock::now();
    for (;;) {
        sync += gap;

        if (std::chrono::steady_clock::now() > sync)
            continue;

        auto frame = next_frame();

        { // handle events
            std::lock_guard lock(mutex);
            if (not events.empty()) {
                switch (events.back()) {
                case Event::play: paused = false; break;
                case Event::pause: paused = true; break;
                case Event::stop: running.store(false); return;
                }
                events.pop();
            }
        }

        if (paused) {
            std::this_thread::yield();
            continue;
        }

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
    if (not running.load())
        player = std::thread(&Player::run, this);
    return push_event(Event::play);
}

auto Player::pause() -> Player& { return push_event(Event::pause); }

auto Player::stop() -> Player& { return push_event(Event::stop); }
