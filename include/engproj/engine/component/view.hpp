#pragma once

#include "engproj/engine/component/componentpool.hpp"
#include "engproj/engine/component/iterablerange.hpp"

namespace engproj::engine::component{

template<typename... Components>
class view{
public:
  view(componentpool<Components>&... pools) : pools_(pools...){};

  auto each(){
    if constexpr(sizeof...(Components)>1){
      return iterablerange_multi<Components...>(pools_);
    }else if constexpr(sizeof...(Components)==1){
      return iterablerange<Components...>(std::get<0>(pools_));
    }else{
      static_assert(sizeof...(Components)>0,"view<> requries at least one component type");
    }
  }

private:
  std::tuple<componentpool<Components>&...> pools_;//i don't think this can be a reference because pools is parameter pack?



};
}
