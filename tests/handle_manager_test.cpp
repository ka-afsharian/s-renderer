#include "engproj/logger/logger.hpp"
#include "engproj/engine/world.hpp"
#include "engproj/engine/component/component.hpp"
#include "engproj/engine/component/componentpool.hpp"
#include "engproj/engine/handle_mngr.hpp"
#include "engproj/engine/component/iterator.hpp"
#include "engproj/engine/component/iterablerange.hpp"
using namespace engproj::engine;
int main(){
  entity_hdl_mngr handle_manager{};
  std::vector<entity_hdl> myvec{};
  for(int i = 0; i!=1000;i++){
    entity_hdl temp =*handle_manager.create_handle();
    myvec.push_back(temp);
  }
  for(int i = 0; i!=1000;i++){
    handle_manager.destroy_handle(myvec.back());
    myvec.pop_back();
  }
  for(int i = 0; i!=5;i++){
    entity_hdl temp = *handle_manager.create_handle();
    myvec.push_back(temp);
  }
  engproj::logger::t_logger.debug("Done adding entities, handle_mngr size:{}",handle_manager.size());
  handle_manager.debug_print();
  //world myworld{};
}
