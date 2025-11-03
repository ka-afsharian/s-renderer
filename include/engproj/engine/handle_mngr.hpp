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
  handle_mngr() : hdl_id_to_index_(1,invalid_index_),hdl_id_to_gen_(1,0){
    expand();//we don't add a free id with index 0 because 0 is invalid, we just expand, expand doesn't create freeid 0 now
    logger::e_logger.debug("Created new handle_mngr: sparse index size:{},sparse gen size:{},free_ids size:{},#handles:{}",
                           hdl_id_to_index_.size(),hdl_id_to_gen_.size(),free_ids_.size(),dense_hdl_.size());
  }

  [[nodiscard]]
  std::optional<hdl> create_handle(){
    if(hdl_id_to_index_.size() == max_id_ && free_ids_.empty()){
      return std::nullopt;
    }
    if(free_ids_.empty()){
      expand();
    }
    handle_id_type id = free_ids_.back();
    free_ids_.pop_back();
    handle_gen_type gen = hdl_id_to_gen_[id];
    dense_hdl_.emplace_back(id,gen);
    hdl_id_to_index_[id]=dense_hdl_.size()-1;
    return dense_hdl_.back();
  }

  std::optional<hdl> destroy_handle(hdl handle){
    if(!has_handle(handle)){
      logger::e_logger.debug("destroy_handle function in handle_mngr, handle_mngr can't delete handle that doesn't exit: Handle id:{},gen:{}",handle.id,handle.gen);
      return std::nullopt;
    }
    hdl returnhdl{0,0};
    size_t index = static_cast<size_t>(hdl_id_to_index_[handle.id]);
    size_t last_index = dense_hdl_.size() - 1;
    if(index!=last_index){
      size_t id_last_index = dense_hdl_[last_index].id;
      std::swap(dense_hdl_[index],dense_hdl_[last_index]);
      hdl_id_to_index_[id_last_index] = index;
    }
    hdl_id_to_index_[handle.id] = invalid_index_;
    hdl_id_to_gen_[handle.id]++;
    free_ids_.emplace_back(handle.id);
    returnhdl = dense_hdl_.back();
    dense_hdl_.pop_back();
    return returnhdl;
  }

  void expand(size_t amount = 1000){
    size_t old_size = hdl_id_to_index_.size();
    size_t new_size = std::min(old_size + amount,static_cast<size_t>(max_id_));
    size_t newamount = new_size - old_size;
    logger::e_logger.debug("Expanding handle_mngr sparse vectors: old_size:{},new_size{}",old_size,new_size);
    hdl_id_to_index_.resize(new_size,invalid_index_);
    hdl_id_to_gen_.resize(new_size,0);
    free_ids_.reserve(newamount);
    for(size_t i = 0;i < newamount;i++){
      free_ids_.emplace_back(old_size+newamount - 1 - i);
    }
    logger::e_logger.debug("Finished expanding handle_mngr sparse vecors: free_ids size:{}",free_ids_.size());
    /*
    for(size_t i = old_size;i < old_size+newamount;i++){//reverse this
      free_ids_.emplace_back(i);
    }
    */
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

  size_t size(){
    return dense_hdl_.size();
  }

  void debug_print(){
    logger::e_logger.debug("hdl_id_to_index_ size:{},hdl_id_to_gen_ size:{},free_ids_ size:{},dense_hdl size:{}",
                           hdl_id_to_index_.size(),hdl_id_to_gen_.size(),free_ids_.size(),dense_hdl_.size());
    logger::e_logger.debug("Printing handle_mngr:");
    for(auto& handle : dense_hdl_){
      logger::e_logger.debug("Handle id:{}, gen:{}",handle.id,handle.gen);
    }
  }

private:
  std::vector<hdl> dense_hdl_;//livng count is just this .size
  std::vector<handle_id_type> hdl_id_to_index_;//sparse
  std::vector<handle_gen_type> hdl_id_to_gen_;//sparse
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
