#pragma once

#include <cstdint>
#include <string>
#include <memory>
#include <unordered_map>
#include <vector>

#include <sys/inotify.h>

#include "dsl.hh"

class inotify {
    private:
        struct wfd_data {
            std::vector<dsl::argument> args;
            std::string location;
        };

        std::unordered_map<int, wfd_data> handlers;
        int inotify_fd;

        void exec(inotify_event *ev) const noexcept;

    public:
        inotify(std::vector<dsl::watcher> &&watchers) noexcept;
        void run() const noexcept;
};
