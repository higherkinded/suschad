#include "dsl.hh"

auto merge(std::initializer_list<dsl::event_type> events) -> uint32_t {
    uint32_t result = 0;
    for (const auto &ev : events) result |= static_cast<uint32_t>(ev);
    return result;
}

dsl::watcher::watcher(
    std::string &&location,
    std::initializer_list<event_type> types,
    std::vector<argument> &&cmd
): location(location), mode(merge(types)), cmd(cmd) {}
