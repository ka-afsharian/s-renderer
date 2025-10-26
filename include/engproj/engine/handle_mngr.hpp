//this should be a template class that creates and destroys handles, with generation support
#pragma once

#include "engproj/engine/handle.hpp"

#include <cstdint>
namespace engproj::engine{



template<engproj::engine::handletype T>
struct handle_mngr{
public:
  static constexpr std::size_t max_entries = 65536;//could be even more?

private:
};

}
