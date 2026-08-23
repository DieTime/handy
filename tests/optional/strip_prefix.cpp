#define HANDY_OPTIONAL_PREFIX STRIP
#include <handy/optional.h>

#include <gtest/gtest.h>

typedef optional_t(int) opt_int_t;

TEST(Optional, StripPrefixAliasesMatchHandyPrefixedBehavior) {
    opt_int_t opt = optional_some(opt_int_t, 5);

    EXPECT_EQ(optional_is_some(opt), handy_optional_is_some(opt));
    EXPECT_EQ(optional_is_none(opt), handy_optional_is_none(opt));
    EXPECT_EQ(optional_unwrap(opt),  handy_optional_unwrap(opt));

    opt = optional_none(opt_int_t);

    EXPECT_EQ(optional_is_some(opt),       handy_optional_is_some(opt));
    EXPECT_EQ(optional_is_none(opt),       handy_optional_is_none(opt));
    EXPECT_EQ(optional_unwrap_or(opt, -1), handy_optional_unwrap_or(opt, -1));
}
