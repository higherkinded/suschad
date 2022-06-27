#pragma once

#include <initializer_list>
#include <string>
#include <vector>

#include <sys/inotify.h>

namespace dsl {
    enum event_type {
        Access = IN_ACCESS,        // File is accessed
        Modify = IN_MODIFY,        // File is modified
        Attrch = IN_ATTRIB,        // File metadata changed

        Open   = IN_OPEN,          // File was opened

        Close  = IN_CLOSE,         // File is closed
        CloseW = IN_CLOSE_WRITE,   // File is closed from write mode
        CloseN = IN_CLOSE_NOWRITE, // File is closed from read mode

        Moved  = IN_MOVE,          // File was moved
        MovedF = IN_MOVED_FROM,    // File was moved from
        MovedT = IN_MOVED_TO,      // File was moved to

        Create = IN_CREATE,        // Subfile created
        Delete = IN_DELETE,        // Subfile deleted
        DelSlf = IN_DELETE_SELF,   // Watched file/dir was deleted

        All    = IN_ALL_EVENTS,     // Just please don't
    };

    enum sub {
        None,
        Name,
        Self,
    };

    struct argument {
        const std::string arg = "";
        const sub substitution = sub::None;

        argument(const char *arg): arg(arg) {}
        argument(std::string &&arg): arg(arg) {}
        argument(sub substitution): substitution(substitution) {}
    };

    struct watcher {
        const std::string location;
        const uint32_t mode;
        const std::vector<argument> cmd;

        watcher(
            std::string &&location,
            std::initializer_list<event_type> types,
            std::vector<argument> &&cmd
        );
    };
}
