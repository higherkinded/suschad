#include "inotify.hh"

#include <array>
#include <cerrno>
#include <climits>
#include <cstddef>
#include <cstdio>
#include <cstring>
#include <exception>
#include <utility>

#include <sys/inotify.h>
#include <unistd.h>

#include "dsl.hh"
#include "util.hh"

using std::vector;

inotify::inotify(std::vector<dsl::watcher> &&watchers) noexcept {
    util::verbose("Creating an inotify instance");

    handlers = new std::map<int, wfd_data>();

    inotify_fd = inotify_init();

    if (inotify_fd == -1) {
        util::die("Cannot start watchers: %s", strerror(errno));
    }

    for (const auto &watcher : watchers) {
        util::verbose(
            "Creating <0x%08x> handler for '%s'",
            watcher.mode,
            watcher.location.c_str()
        );

        int wfd = inotify_add_watch(
            inotify_fd,
            watcher.location.c_str(),
            watcher.mode
        );

        if (wfd == -1) util::die(
            "Failed to create <0x%08x> handler for '%s': %s",
            watcher.mode,
            watcher.location.c_str(),
            strerror(errno)
        );

        handlers->insert({ wfd, wfd_data {
            std::move(watcher.cmd),
            watcher.location,
        }});
    }
}

static constexpr size_t READBUF_SIZE = sizeof(inotify_event) + NAME_MAX + 1;

void inotify::exec(inotify_event *ev) noexcept {
    std::vector<const char *> argv;

    if (handlers->find(ev->wd) == handlers->end()) {
        util::err("Unknown watch descriptor: %d", ev->wd);
    }

    const auto &handler = handlers->at(ev->wd);

    for (const auto &arg : handler.args) {
        switch (arg.substitution) {
            case dsl::Name:
                if (ev->len) argv.push_back(ev->name);
                break;
            case dsl::Self:
                argv.push_back(handler.location.c_str());
                break;
            default:
                if (arg.arg.size()) argv.push_back(arg.arg.c_str());
        }
    }

    argv.push_back(nullptr);

    if (argv[0] && !fork()) {
        if (execvp((const char *) argv[0], (char *const *) argv.data())) {
            std::string cmd(argv[0]);
            for (size_t i = 1; i < argv.size() - 1; i++) {
                cmd = (cmd + " ") + argv[i];
            }

            util::err("Failed to exec `%s`: %s", cmd.data(), strerror(errno));
        }
    } }

void inotify::run() noexcept {
    static std::array<uint8_t, READBUF_SIZE> evb;
    inotify_event *ev;

    for (;;) {
        util::verbose("Blocking read on inotify");
        ssize_t rbytes = read(inotify_fd, evb.data(), READBUF_SIZE);
        util::verbose("Got event from inotify");
        ssize_t pbytes = 0;

        while (pbytes < rbytes) {
            ev = (inotify_event *) evb.data() + pbytes;
            util::verbose("At %d in %d", pbytes, rbytes);
            util::verbose("Event: %d 0x%08x", ev->wd, ev->mask);
            pbytes += ev->len + static_cast<ssize_t>(sizeof(inotify_event));
            exec(ev);
        }
    }
}
