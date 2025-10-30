#pragma once

#include "engproj/engine/handle.hpp"
#include "engproj/engine/component/componentpool.hpp"

namespace engproj::engine::component{

template<typename... Components>
class view{
public:
  view(componentpool<Components>&... pools) : pools_(pools...){};
private:
  std::tuple<componentpool<Components>&...> pools_;



};
}
