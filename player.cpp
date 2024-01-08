#include <iostream>

#include "Player.h"
#include "Watcher.h"
#include "opencv.h"

using namespace term_vid_player;

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
    Player player(config);
    Watcher watcher(player);

    player.play();

    watcher.watch();
}

void help() {
    std::cout << "Usage: player [options] <source_file>\n";
    std::cout << "NO Avaible options\n";
    std::cout.flush();
}
