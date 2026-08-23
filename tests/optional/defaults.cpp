#include <handy/optional.h>

#include <gtest/gtest.h>

#ifdef optional_t
#error "HANDY_OPTIONAL_PREFIX defaults to KEEP; optional_t must not be defined without HANDY_OPTIONAL_PREFIX STRIP"
#endif

typedef handy_optional_t(int) opt_int_t;

TEST(Optional, SomeSetsHasValueAndValue) {
    opt_int_t opt = handy_optional_some(opt_int_t, 5);

    EXPECT_TRUE(handy_optional_is_some(opt));
    EXPECT_EQ(handy_optional_unwrap(opt), 5);
}

TEST(Optional, NoneClearsHasValue) {
    opt_int_t opt = handy_optional_none(opt_int_t);

    EXPECT_FALSE(handy_optional_is_some(opt));
}

TEST(Optional, SomeInitAndNoneInitWorkWithoutATypeNameAtTheCallSite) {
    handy_optional_t(int) some = handy_optional_init_some(5);
    handy_optional_t(int) none = handy_optional_init_none();

    EXPECT_TRUE(handy_optional_is_some(some));
    EXPECT_EQ(handy_optional_unwrap(some), 5);
    EXPECT_FALSE(handy_optional_is_some(none));
}

TEST(Optional, IsNoneIsTrueOnlyWhenValueLess) {
    opt_int_t some = handy_optional_some(opt_int_t, 5);
    opt_int_t none = handy_optional_none(opt_int_t);

    EXPECT_FALSE(handy_optional_is_none(some));
    EXPECT_TRUE(handy_optional_is_none(none));
}

TEST(Optional, ValueOrReturnsStoredValueWhenPresent) {
    opt_int_t opt = handy_optional_some(opt_int_t, 7);

    EXPECT_EQ(handy_optional_unwrap_or(opt, 99), 7);
}

TEST(Optional, ValueOrReturnsDefaultWhenNone) {
    opt_int_t opt = handy_optional_none(opt_int_t);

    EXPECT_EQ(handy_optional_unwrap_or(opt, 42), 42);
}

TEST(OptionalDeathTest, ValueAbortsWhenNone) {
    opt_int_t opt = handy_optional_none(opt_int_t);

    EXPECT_DEATH(handy_optional_unwrap(opt), "");
}

static opt_int_t divide(int a, int b) {
    if (b == 0) {
        return handy_optional_none(opt_int_t);
    }
    return handy_optional_some(opt_int_t, a / b);
}

TEST(Optional, DeclaredTypeRoundTripsThroughFunctionReturn) {
    opt_int_t ok = divide(10, 2);
    opt_int_t fail = divide(10, 0);

    EXPECT_TRUE(handy_optional_is_some(ok));
    EXPECT_EQ(handy_optional_unwrap(ok), 5);

    EXPECT_FALSE(handy_optional_is_some(fail));
}

/* handy_optional_t(T) is usable as a bare inline field type without a
 * declared name; without a name, handy_optional_some/_none have no stable
 * type to build a compound literal against, so construction goes through
 * the raw `unsafe` fields directly instead. */
struct config {
    handy_optional_t(const char *) name;
};

TEST(Optional, WorksAsAnInlineStructField) {
    struct config cfg;
    cfg.name.unsafe.is_some = true;
    cfg.name.unsafe.value = "handy";

    EXPECT_TRUE(handy_optional_is_some(cfg.name));
    EXPECT_STREQ(handy_optional_unwrap(cfg.name), "handy");
}

typedef handy_optional_t(double) opt_double_t;
typedef handy_optional_t(const char *) opt_cstr_t;

TEST(Optional, IndependentInstantiationsDoNotInterfere) {
    opt_int_t i = handy_optional_some(opt_int_t, 1);
    opt_double_t d = handy_optional_none(opt_double_t);
    opt_cstr_t s = handy_optional_some(opt_cstr_t, "three");

    EXPECT_EQ(handy_optional_unwrap(i), 1);
    EXPECT_FALSE(handy_optional_is_some(d));
    EXPECT_STREQ(handy_optional_unwrap(s), "three");
}

static void handle_value(int x) {
    (void)x;
}

/* T's declarator wraps the identifier for function-pointer and array types
 * (`void (*value)(int)`, `int value[3]`, not `T value;`) - handy_optional_t
 * resolves T through __typeof__ so these work directly, with no need to
 * typedef the inner type first. */
typedef handy_optional_t(void (*)(int)) opt_fp_t;
typedef handy_optional_t(int[3]) opt_arr_t;

TEST(Optional, SupportsFunctionPointerAndArrayTypesDirectly) {
    opt_fp_t f = handy_optional_some(opt_fp_t, handle_value);
    EXPECT_TRUE(handy_optional_is_some(f));
    handy_optional_unwrap(f)(42);

    opt_arr_t a = handy_optional_none(opt_arr_t);
    EXPECT_FALSE(handy_optional_is_some(a));
    a.unsafe.value[0] = 7;
    EXPECT_EQ(a.unsafe.value[0], 7);
}
