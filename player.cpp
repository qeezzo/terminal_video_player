#include <iostream>
#include <thread>

#include "GrayFrame.h"
#include "Terminal.h"
#include "Video.h"
#include "opencv.h"

// darker --(next char)--> lighter
std::array output{' ', '.', ':', '-', '=', '+', '*', '#', '%', '@'};

char get_output_char(uchar grey) {
    return output[round(grey / 255.0 * (output.size() - 1))];
}

class Config {
  private:
    std::unordered_map<std::string, void (Config::*)()> keys{};
    std::string file;

  public:
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

void run(Config const& config);
void help();

int main(int argc, char* argv[]) {
    Config config(argc, argv);

    if (config.source().empty()) {
        help();
        return 0;
    }

    run(config);
}

void run(Config const& config) {
    using namespace term_vid_player;

    Video video(config.source());
    Terminal term;

    int count{};
    for (GrayFrame frame; count < video.length(); video >> frame, ++count) {
        if (frame.empty()) {
            std::cout << "here" << std::endl;
            continue;
        }

        // adaptive scaling
        double const scaleX = (double)term.width() / frame.width();
        double const scaleY = (double)term.height() / frame.height() * 2;
        double const scale  = std::min(scaleX, scaleY);
        frame.scale(scale);

        term.clear();
        int px_count{1};
        for (auto px = frame.begin(), end = frame.end(); px != end; ++px) {

            std::cout << get_output_char(*px);
            if (px_count == frame.width()) {
                // skip one row cuz assume row's size is as size of two columns
                // thats why scaleY is multiply by 2
                std::advance(px, frame.width());

                std::cout << "\n";
                px_count = 0;
            }
            ++px_count;
        }
        std::cout.flush();

        auto sleep = std::chrono::milliseconds(1000 / video.fps());
        std::this_thread::sleep_for(sleep);
    }
}

void help() {
    std::cout << "Usage: player [options] <source_file>\n";
    std::cout << "NO Avaible options\n";
    std::cout.flush();
}
