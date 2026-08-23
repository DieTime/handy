#define HANDY_OPTIONAL_PREFIX STRIP
#include <handy/optional.h>

#include <stdio.h>

typedef optional_t(int) opt_int_t;
typedef optional_t(const char *) opt_cstr_t;

struct config {
    /* named type: lets optional_some/optional_none rebuild this field
     * elsewhere too, since they need a stable type name for their
     * compound literal. */
    opt_cstr_t name;

    /* optional_t(T) also works directly inline, with no typedef at all --
     * but then optional_some/optional_none can't construct it (no name to
     * build their compound literal against), so use optional_init_some/
     * optional_init_none instead, or write the raw `unsafe` fields. */
    optional_t(int) port;
};

int main(void) {
    opt_int_t a = optional_some(opt_int_t, 5);
    opt_int_t b = optional_none(opt_int_t);

    /* optional_init_some/_init_none need no type name at all, but only work
     * as a declaration's initializer, not as a general expression. */
    optional_t(int) c = optional_init_some(9);

    struct config cfg = {
        .name = optional_some(opt_cstr_t, "handy"),
        .port = optional_init_some(3),
    };

    printf("a: is_some=%d unwrap=%d\n",         optional_is_some(a), optional_unwrap(a));
    printf("b: is_some=%d unwrap_or(-1)=%d\n",  optional_is_some(b), optional_unwrap_or(b, -1));
    printf("c: is_some=%d unwrap=%d\n",         optional_is_some(c), optional_unwrap(c));

    printf("cfg.name: is_some=%d unwrap=%s\n", optional_is_some(cfg.name), optional_unwrap(cfg.name));
    printf("cfg.port: is_some=%d unwrap=%d\n", optional_is_some(cfg.port), optional_unwrap(cfg.port));
}
