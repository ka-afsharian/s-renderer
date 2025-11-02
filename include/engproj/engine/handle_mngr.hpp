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
  using hdl = engine::handle<T>;
  static constexpr handle_id_type max_id_ = std::min(static_cast<handle_id_type>(65536),std::numeric_limits<handle_id_type>::max()-2);
  static inline handle_id_type invalid_id_ = engine::entity_hdl::invalid.id;
  static const size_t invalid_index_ = max_id_ + 1; //this should be fine
  //this includes 0 which is invalid
  //std::numeric_limits<handle_id_type>::max()-1 is the max possible entry but this should also be invalid so do -2
  handle_mngr() : hdl_id_to_index_(1000,invalid_index_),hdl_id_to_gen_(1000,0){
    free_ids_.reserve(2000);
    for(int i = 1001;i > 0;i--){//0 is invalid(idk why i did that)
      free_ids_.emplace_back(i);
    }
  }

  [[nodiscard]]
  std::optional<hdl> create_handle(){
    if(hdl_id_to_index_.size() == max_id_+1 && free_ids_.empty()){
      return std::nullopt;
    }
    if(free_ids_.empty()){
      expand();
    }
    handle_id_type id = free_ids_.back();
    free_ids_.pop_back();
    handle_gen_type gen = hdl_id_to_gen_[id];
    dense_hdl_.push_back({id,gen});
    hdl_id_to_index_[id]=dense_hdl_.size()-1;
    return dense_hdl_.back();
  }

  std::optional<hdl> destroy_handle(hdl handle){
    if(!has_handle(handle)){
      return std::nullopt;
    }
    hdl returnhdl;
    size_t index = static_cast<size_t>(hdl_id_to_index_[handle.id]);
    size_t last_index = dense_hdl_.size() - 1;
    if(index!=last_index){
      size_t id_last_index = dense_hdl_[last_index].id;
      std::swap(dense_hdl_[index],dense_hdl_[last_index]);
      hdl_id_to_index_[id_last_index] = index;
    }
    hdl_id_to_index_[handle.id] = invalid_index_;
    hdl_id_to_gen_[handle.id]++;
    returnhdl = dense_hdl_.back();
    dense_hdl_.pop_back();
    return returnhdl;
  }

  void expand(size_t amount = 1000){
    size_t old_size = hdl_id_to_index_.size();
    size_t new_size = std::min(old_size + amount,static_cast<size_t>(max_id_)+1);
    size_t newamount = new_size - old_size;
    hdl_id_to_index_.resize(new_size,invalid_index_);
    hdl_id_to_gen_.resize(new_size,0);
    free_ids_.reserve(newamount);
    for(int i = old_size;i < old_size+newamount;i++){//reverse this
      free_ids_.emplace_back(i);
    }
  }

  bool has_handle(hdl handle){
    if(!is_valid(handle) || !hdl_within_sparse_size(handle)){
      return false;
    }
    size_t index = static_cast<size_t>(hdl_id_to_index_[handle.id]);
    if(index == invalid_index_){
      return false;
    }
    return handle == dense_hdl_[index];
  }

  bool is_valid(hdl handle){
    if(handle.id > max_id_ || handle.id == invalid_id_){
      logger::e_logger.debug("handle<T> valid check false, handle_mngr class. handle<T> id:{}, gen:{}",handle.id,handle.gen);
      return false;
    }

    return true;
  }

private:
  std::vector<hdl> dense_hdl_;//livng count is just this .size
  std::vector<handle_gen_type> hdl_id_to_gen_;//sparse
  std::vector<handle_id_type> hdl_id_to_index_;//sparse
  std::vector<handle_id_type> free_ids_;



  bool hdl_within_sparse_size(hdl handle){//checks if entity id is within sparse size
    if(handle.id >= hdl_id_to_index_.size()){
      logger::e_logger.debug("Handle within sparse size check false, handle_mngr class. Handle id:{}, gen:{}",handle.id,handle.gen);
      return false;
    }
    return true;
  }
};

using entity_hdl_mngr = handle_mngr<engine::handletype::entity>;


}
