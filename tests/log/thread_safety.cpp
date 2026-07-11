#define HANDY_LOG_LEVEL DEBUG
#define HANDY_LOG_STDIO STDERR
#define HANDY_LOG_THREAD_SAFETY ENABLED
#include <handy/log.h>

#include <gtest/gtest.h>
#include <sstream>
#include <thread>
#include <vector>

#include "temp_file.hpp"

namespace {

const int THREAD_COUNT = 8;
const int LINES_PER_THREAD = 50;
/* Long enough that a single fprintf call is very likely to be split into
 * multiple underlying write() syscalls, so an unlocked interleave between
 * threads would actually have a chance to show up within a line. */
const size_t LINE_LENGTH = 4096;

void log_lines(char marker) {
    std::string payload(LINE_LENGTH, marker);
    for (int i = 0; i < LINES_PER_THREAD; ++i) {
        handy_log_d("%s", payload.c_str());
    }
}

std::string run_concurrent_logging() {
    TempFile tmp;
    freopen(tmp.path().c_str(), "w", stderr);

    std::vector<std::thread> threads;
    for (int t = 0; t < THREAD_COUNT; ++t) {
        threads.emplace_back(log_lines, static_cast<char>('A' + t));
    }
    for (auto &th : threads) {
        th.join();
    }
    fflush(stderr);

    return tmp.contents();
}

} // namespace

TEST(Log, ThreadSafetyProducesExactlyOneLinePerLogCall) {
    std::istringstream lines(run_concurrent_logging());
    int line_count = 0;
    std::string line;
    while (std::getline(lines, line)) {
        ++line_count;
    }

    EXPECT_EQ(line_count, THREAD_COUNT * LINES_PER_THREAD);
}

TEST(Log, ThreadSafetyNeverMixesMarkersWithinALine) {
    std::istringstream lines(run_concurrent_logging());
    std::string line;
    while (std::getline(lines, line)) {
        auto bracket = line.find(']');
        ASSERT_NE(bracket, std::string::npos);
        size_t body_start = bracket + 2; /* skip "] " */
        ASSERT_LT(body_start, line.size());

        char marker = line[body_start];
        for (size_t i = body_start; i < line.size(); ++i) {
            EXPECT_EQ(line[i], marker);
        }
    }
}
