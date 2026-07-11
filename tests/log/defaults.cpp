#include <handy/log.h>

#include <gtest/gtest.h>

#include "temp_file.hpp"

#ifdef log_d
#error "HANDY_PREFIX defaults to KEEP; log_d must not be defined without HANDY_PREFIX STRIP"
#endif

TEST(Log, DefaultsFilterAtInfoAndRouteToStderrWithoutStripping) {
    TempFile tmp;
    ASSERT_NE(freopen(tmp.path().c_str(), "w", stderr), nullptr);

    handy_log_d("debug should be suppressed by default");
    handy_log_i("info %d", 1);
    handy_log_w("warn %d", 2);
    handy_log_e("error %d", 3);
    fflush(stderr);

    EXPECT_EQ(tmp.contents(), "[*] info 1\n[?] warn 2\n[!] error 3\n");
}
