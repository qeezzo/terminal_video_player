#include <gtest/gtest.h>

#include <sstream>

#include "../src/Player.h"
#include "../src/Watcher.h"

using namespace term_vid_player;

class PlayerTestFixture : public ::testing::Test {
   protected:
    std::string test_video_path = "duck.mp4";
    std::vector<const char*> args;
    std::stringstream input_stream;
    std::streambuf* cin_buf;
    Config config;
    Player player;

    PlayerTestFixture()
        : args{"./player", "--dummy", test_video_path.c_str()},
          config(static_cast<int>(args.size()), const_cast<char**>(args.data())),
          player(config) {}

    auto SetUp() -> void override {
        cin_buf = std::cin.rdbuf(input_stream.rdbuf());
    }

    auto TearDown() -> void override {
        std::cin.rdbuf(cin_buf);
    }
};
TEST_F(PlayerTestFixture, PlayerInitialization) {
    EXPECT_EQ(config.source(), test_video_path);
    EXPECT_FALSE(player.running());
}

TEST_F(PlayerTestFixture, PlayerPlay) {
    player.play();
    EXPECT_TRUE(player.running());
}

TEST_F(PlayerTestFixture, PlayerStop) {
    player.play();
    EXPECT_TRUE(player.running());

    player.stop();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_FALSE(player.running());
}

TEST_F(PlayerTestFixture, WatcherKeyMappings) {
    input_stream << "q";

    player.play();
    EXPECT_TRUE(player.running());

    Watcher watcher(player);
    watcher.watch();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    EXPECT_FALSE(player.running());
}
