#define HANDY_LOG_LEVEL DEBUG
#define HANDY_LOG_STDIO STDERR
#include <handy/log.h>

#include <gtest/gtest.h>

#include "temp_file.hpp"

TEST(Log, FormatTagsDebugLinesWithTilde) {
    TempFile tmp;
    ASSERT_NE(freopen(tmp.path().c_str(), "w", stderr), nullptr);

    handy_log_d("debug %d", 1);
    fflush(stderr);

    EXPECT_EQ(tmp.contents(), "[~] debug 1\n");
}

TEST(Log, FormatTagsInfoLinesWithAsterisk) {
    TempFile tmp;
    ASSERT_NE(freopen(tmp.path().c_str(), "w", stderr), nullptr);

    handy_log_i("info %d", 2);
    fflush(stderr);

    EXPECT_EQ(tmp.contents(), "[*] info 2\n");
}

TEST(Log, FormatTagsWarnLinesWithQuestionMark) {
    TempFile tmp;
    ASSERT_NE(freopen(tmp.path().c_str(), "w", stderr), nullptr);

    handy_log_w("warn %d", 3);
    fflush(stderr);

    EXPECT_EQ(tmp.contents(), "[?] warn 3\n");
}

TEST(Log, FormatTagsErrorLinesWithExclamationMark) {
    TempFile tmp;
    ASSERT_NE(freopen(tmp.path().c_str(), "w", stderr), nullptr);

    handy_log_e("error %d", 4);
    fflush(stderr);

    EXPECT_EQ(tmp.contents(), "[!] error 4\n");
}
