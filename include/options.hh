#pragma once

struct options {
    bool daemonize = true;
    bool verbose = false;

    options(const int argc, const char **argv);
};
