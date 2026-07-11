#define HANDY_LOG_LEVEL DEBUG
#define HANDY_LOG_STDIO STDOUT
#define HANDY_LOG_FLUSH ERRORS
#include <handy/log.h>

#include <gtest/gtest.h>
#include <sys/wait.h>

#include "temp_file.hpp"

/* A message logged on its own (nothing else touches the shared stdio
 * buffer) only reaches disk if handy_log_* explicitly flushed it, since
 * _exit() skips libc's normal exit-time flush. */
static void run_child_logging_debug(const std::string &path) {
    pid_t pid = fork();
    ASSERT_GE(pid, 0);
    if (pid == 0) {
        freopen(path.c_str(), "w", stdout);
        handy_log_d("DEBUG_MARKER");
        _exit(0);
    }
    waitpid(pid, nullptr, 0);
}

static void run_child_logging_error(const std::string &path) {
    pid_t pid = fork();
    ASSERT_GE(pid, 0);
    if (pid == 0) {
        freopen(path.c_str(), "w", stdout);
        handy_log_e("ERROR_MARKER");
        _exit(0);
    }
    waitpid(pid, nullptr, 0);
}

TEST(Log, FlushErrorsDoesNotFlushDebugLevelLines) {
    TempFile tmp;
    run_child_logging_debug(tmp.path());

    EXPECT_TRUE(tmp.contents().empty());
}

TEST(Log, FlushErrorsFlushesErrorLevelLinesImmediately) {
    TempFile tmp;
    run_child_logging_error(tmp.path());

    EXPECT_EQ(tmp.contents(), "[!] ERROR_MARKER\n");
}
