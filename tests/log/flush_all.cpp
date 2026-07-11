#define HANDY_LOG_LEVEL DEBUG
#define HANDY_LOG_STDIO STDOUT
#define HANDY_LOG_FLUSH ALL
#include <handy/log.h>

#include <gtest/gtest.h>
#include <sys/wait.h>

#include "temp_file.hpp"

TEST(Log, FlushAllFlushesEveryLineImmediately) {
    TempFile tmp;

    pid_t pid = fork();
    ASSERT_GE(pid, 0);
    if (pid == 0) {
        freopen(tmp.path().c_str(), "w", stdout);
        handy_log_d("DEBUG_MARKER");
        _exit(0); /* skip libc's normal exit-time flush */
    }
    waitpid(pid, nullptr, 0);

    EXPECT_EQ(tmp.contents(), "[~] DEBUG_MARKER\n");
}
