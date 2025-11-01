#pragma once

#include "engproj/engine/handle.hpp"
#include "engproj/engine/component/componentpool.hpp"
#include "engproj/engine/component/iterablerange.hpp"

namespace engproj::engine::component{

template<typename... Components>
class view{
public:
  view(componentpool<Components>&... pools) : pools_(pools...){};

  auto each(){
    return iterablerange<Components...>(pools_);
  }

private:
  std::tuple<componentpool<Components>&...> pools_;//i don't think this can be a reference because pools is parameter pack?



};
}
