#define HANDY_LOG_LEVEL NONE
#define HANDY_LOG_STDIO STDERR
#include <handy/log.h>

#include <gtest/gtest.h>

#include "temp_file.hpp"

TEST(Log, LevelNoneDropsEverythingIncludingErrors) {
    TempFile tmp;
    ASSERT_NE(freopen(tmp.path().c_str(), "w", stderr), nullptr);

    handy_log_d("debug should not appear");
    handy_log_i("info should not appear");
    handy_log_w("warn should not appear");
    handy_log_e("error should not appear");
    fflush(stderr);

    EXPECT_TRUE(tmp.contents().empty());
}
