#pragma once

#include <string>

namespace util {
    void enable_verbosity();
    void die(const char *fmt, ...);
    void err(const char *fmt, ...);
    void verbose(const char *fmt, ...);
    void daemonize();
}
