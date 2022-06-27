#include "util.hh"

#include <cerrno>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

static bool verbose_logs = false;

void util::enable_verbosity() {
    verbose_logs = true;
}

void util::die(const char *fmt, ...) {
    va_list args;

    std::fprintf(stderr, "(%s) ", EXENAME);

    va_start(args, fmt);
    std::vfprintf(stderr, fmt, args);
    va_end(args);

    std::fputc('\n', stderr);

    exit(1);
}

void util::err(const char *fmt, ...) {
    va_list args;

    std::fprintf(stderr, "(%s) ", EXENAME);

    va_start(args, fmt);
    std::vfprintf(stderr, fmt, args);
    va_end(args);

    std::fputc('\n', stderr);
}

void util::verbose(const char *fmt, ...) {
    if (!verbose_logs) return;

    va_list args;

    std::fprintf(stderr, "(%s) ", EXENAME);

    va_start(args, fmt);
    std::vfprintf(stderr, fmt, args);
    va_end(args);

    std::fputc('\n', stderr);
}

void util::daemonize() {
    if (daemon(true, true) == -1) die("Can't daemonize: %s", strerror(errno));
}
