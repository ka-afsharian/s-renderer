//this should be a template class that creates and destroys handles, with generation support
#pragma once

#include "engproj/engine/handle.hpp"
#include "engproj/logger/logger.hpp"

#include <numeric>
#include <algorithm>
namespace engproj::engine{



template<engproj::engine::handletype T>
struct handle_mngr{
public:
  static constexpr handle_id_type max_id_ = std::min(static_cast<handle_id_type>(65536),std::numeric_limits<handle_id_type>::max()-2);
  static inline handle_id_type invalid_id_ = engine::entity_hdl::invalid.id;
  //this includes 0 which is invalid
  //std::numeric_limits<handle_id_type>::max()-1 is the max possible entry but this should also be invalid so do -2
  handle_mngr();

  bool is_valid(engine::handle<T> handle){
    if(handle.id > max_id_ || handle.id == invalid_id_){
      logger::e_logger.debug("handle<T> valid check false, handle_mngr class. handle<T> id:{}, gen:{}",handle.id,handle.gen);
      return false;
    }
    return true;
  }

private:
};

using entity_hdl_mngr = handle_mngr<engine::handletype::entity>;


}
