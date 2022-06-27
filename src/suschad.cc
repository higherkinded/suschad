#include <csignal>

#include <sys/wait.h>

#include "inotify.hh"
#include "options.hh"
#include "util.hh"

inline void install_sig_handlers();

auto main(const int argc, const char **argv) -> int {
    const options opts(argc, argv);

    if (opts.verbose) util::enable_verbosity();
    if (opts.daemonize) util::daemonize();

    install_sig_handlers();

    using namespace dsl;

    const inotify inh({
        #include "events"
    });

    inh.run();
}

void sigchld([[maybe_unused]] int _) {
    while (0 < waitpid(-1, nullptr, WNOHANG));
}

inline void install_handler(int sig, void (&handler)(int)) {
    if (signal(sig, handler) == SIG_ERR) util::die(
        "Cannot install a signal handler for signal %d", sig);
}

void install_sig_handlers() {
    install_handler(SIGCHLD, sigchld);
}
