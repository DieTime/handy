#define HANDY_LOG_LEVEL DEBUG
#define HANDY_LOG_STDIO STDOUT
#include <handy/log.h>

#include <gtest/gtest.h>
#include <pty.h>
#include <sys/wait.h>
#include <termios.h>
#include <unistd.h>

namespace {

std::string capture_pty_output(void (*log_call)()) {
    int master_fd = -1;
    pid_t pid = forkpty(&master_fd, nullptr, nullptr, nullptr);

    if (pid == 0) {
        termios tio;
        tcgetattr(STDOUT_FILENO, &tio);
        tio.c_oflag &= ~static_cast<tcflag_t>(OPOST); /* disable \n -> \r\n translation */
        tcsetattr(STDOUT_FILENO, TCSANOW, &tio);

        log_call();
        fflush(stdout);
        _exit(0); /* skip libc's normal exit-time flush */
    }

    std::string output;
    char buf[256];
    ssize_t n;
    while ((n = read(master_fd, buf, sizeof(buf))) > 0) {
        output.append(buf, static_cast<size_t>(n));
    }
    close(master_fd);
    waitpid(pid, nullptr, 0);
    return output;
}

} // namespace

TEST(Log, ColoredOutputWrapsDebugTagAndTextInFaint) {
    std::string output = capture_pty_output([] { handy_log_d("PTY_MARKER"); });
    EXPECT_EQ(output, HANDY_COLOR_FAINT "[~]" HANDY_COLOR_RESET " " HANDY_COLOR_FAINT "PTY_MARKER" HANDY_COLOR_RESET "\n");
}

TEST(Log, ColoredOutputWrapsInfoTagAndTextInReset) {
    std::string output = capture_pty_output([] { handy_log_i("PTY_MARKER"); });
    EXPECT_EQ(output, HANDY_COLOR_RESET "[*]" HANDY_COLOR_RESET " " HANDY_COLOR_RESET "PTY_MARKER" HANDY_COLOR_RESET "\n");
}

TEST(Log, ColoredOutputWrapsWarnTagInYellow) {
    std::string output = capture_pty_output([] { handy_log_w("PTY_MARKER"); });
    EXPECT_EQ(output, HANDY_COLOR_YELLOW "[?]" HANDY_COLOR_RESET " " HANDY_COLOR_RESET "PTY_MARKER" HANDY_COLOR_RESET "\n");
}

TEST(Log, ColoredOutputWrapsErrorTagInRed) {
    std::string output = capture_pty_output([] { handy_log_e("PTY_MARKER"); });
    EXPECT_EQ(output, HANDY_COLOR_RED "[!]" HANDY_COLOR_RESET " " HANDY_COLOR_RESET "PTY_MARKER" HANDY_COLOR_RESET "\n");
}
