#include "options.hh"
#include "util.hh"

#include <string>
#include <vector>

using std::string;
using std::vector;

options::options(const int argc, const char **argv) {
    if (argc > 64) util::die("Too many arguments");

    const vector<string> args(argv + 1, argv + argc);

    for (const auto &arg : args) {
        if (arg == "-n") {
            daemonize = false;
            continue;
        }

        if (arg == "-v") {
            verbose = true;
            continue;
        }
    }
}
