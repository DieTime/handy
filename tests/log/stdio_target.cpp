#define HANDY_LOG_LEVEL DEBUG
#define HANDY_LOG_STDIO STDOUT
#include <handy/log.h>

#include <gtest/gtest.h>

#include "temp_file.hpp"

TEST(Log, StdioTargetRoutesLinesToConfiguredStreamOnly) {
    TempFile out, err;
    ASSERT_NE(freopen(out.path().c_str(), "w", stdout), nullptr);
    ASSERT_NE(freopen(err.path().c_str(), "w", stderr), nullptr);

    handy_log_i("routed to stdout");
    fflush(stdout);
    fflush(stderr);

    EXPECT_EQ(out.contents(), "[*] routed to stdout\n");
    EXPECT_TRUE(err.contents().empty());
}
