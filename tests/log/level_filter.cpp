#define HANDY_LOG_LEVEL WARN
#define HANDY_LOG_STDIO STDERR
#include <handy/log.h>

#include <gtest/gtest.h>

#include "temp_file.hpp"

TEST(Log, LevelWarnDropsDebugAndInfoKeepsWarnAndError) {
    TempFile tmp;
    ASSERT_NE(freopen(tmp.path().c_str(), "w", stderr), nullptr);

    handy_log_d("debug should not appear");
    handy_log_i("info should not appear");
    handy_log_w("warn should appear");
    handy_log_e("error should appear");
    fflush(stderr);

    EXPECT_EQ(tmp.contents(), "[?] warn should appear\n[!] error should appear\n");
}
