#define HANDY_LOG_PREFIX STRIP
#define HANDY_LOG_LEVEL DEBUG
#define HANDY_LOG_STDIO STDERR
#include <handy/log.h>

#include <gtest/gtest.h>

#include "temp_file.hpp"

TEST(Log, StripPrefixDebugAliasMatchesHandyPrefixedOutput) {
    TempFile tmp;
    ASSERT_NE(freopen(tmp.path().c_str(), "w", stderr), nullptr);

    log_d("short debug");
    handy_log_d("full debug");
    fflush(stderr);

    EXPECT_EQ(tmp.contents(), "[~] short debug\n[~] full debug\n");
}

TEST(Log, StripPrefixInfoAliasMatchesHandyPrefixedOutput) {
    TempFile tmp;
    ASSERT_NE(freopen(tmp.path().c_str(), "w", stderr), nullptr);

    log_i("short info");
    handy_log_i("full info");
    fflush(stderr);

    EXPECT_EQ(tmp.contents(), "[*] short info\n[*] full info\n");
}

TEST(Log, StripPrefixWarnAliasMatchesHandyPrefixedOutput) {
    TempFile tmp;
    ASSERT_NE(freopen(tmp.path().c_str(), "w", stderr), nullptr);

    log_w("short warn");
    handy_log_w("full warn");
    fflush(stderr);

    EXPECT_EQ(tmp.contents(), "[?] short warn\n[?] full warn\n");
}

TEST(Log, StripPrefixErrorAliasMatchesHandyPrefixedOutput) {
    TempFile tmp;
    ASSERT_NE(freopen(tmp.path().c_str(), "w", stderr), nullptr);

    log_e("short error");
    handy_log_e("full error");
    fflush(stderr);

    EXPECT_EQ(tmp.contents(), "[!] short error\n[!] full error\n");
}
