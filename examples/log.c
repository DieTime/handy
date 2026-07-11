#define HANDY_LOG_STDIO STDOUT
#define HANDY_LOG_LEVEL DEBUG
#define HANDY_LOG_FLUSH ALL
#define HANDY_LOG_THREAD_SAFETY DISABLED
#define HANDY_PREFIX STRIP
#include <handy/log.h>

int main(void) {
    log_d("starting up");
    log_i("listening on port %d", 8080);
    log_w("cache miss for key '%s'", "session:42");
    log_e("failed to bind socket: %s", "address already in use");
}
