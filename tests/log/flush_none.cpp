#define HANDY_LOG_LEVEL DEBUG
#define HANDY_LOG_STDIO STDOUT
#define HANDY_LOG_FLUSH NONE
#include <handy/log.h>

#include <gtest/gtest.h>
#include <sys/wait.h>

#include "temp_file.hpp"

TEST(Log, FlushNoneNeverFlushesExplicitly) {
    TempFile tmp;

    pid_t pid = fork();
    ASSERT_GE(pid, 0);
    if (pid == 0) {
        freopen(tmp.path().c_str(), "w", stdout);
        handy_log_e("ERROR_MARKER");
        _exit(0); /* skip libc's normal exit-time flush */
    }
    waitpid(pid, nullptr, 0);

    EXPECT_TRUE(tmp.contents().empty());
}
