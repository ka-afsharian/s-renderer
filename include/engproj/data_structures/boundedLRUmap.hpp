#pragma once
#include <functional>
#include <iterator>
#include <list>
#include <optional>
#include <vector>

namespace engproj::data_structures{


template<typename Key, typename Value>
requires std::copyable<Value>
class boundedLRUmap{
public:
  using getsize_predicate = std::function<std::size_t(Value)>;
  using order_it = typename std::list<Key>::iterator;

  struct Mapvalue {
    order_it iterator;
    Value value;
  };

  using map_it = typename std::unordered_map<Key, Mapvalue>::iterator;
//iterator for class
  class value_iterator{
  private:
    map_it inner_;
  public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = Value;
    using difference_type = std::ptrdiff_t;
    using pointer = Value*;
    using reference = Value&;

    explicit value_iterator(map_it it) : inner_(it){}

    reference operator*() const {return inner_->second.value;}
    pointer operator->() const {return &inner_->second.value;}

    value_iterator& operator++() {++inner_; return *this;}
    value_iterator operator++(int) {value_iterator tmp = *this; ++inner_; return tmp;}//idk if this works

    bool operator==(const value_iterator& other) const { return inner_ == other.inner_; }
    bool operator!=(const value_iterator& other) const { return inner_ != other.inner_; }

    const Key& key() const {return inner_->first;}
  };

  explicit boundedLRUmap(std::size_t max_capacity, getsize_predicate getsizepred) :
    max_capacity_(max_capacity) , getsize(getsizepred), curr_capacity_(0){}

  std::optional<std::vector<std::pair<Key,Value>>> insert(Key key,Value value){
    size_t value_size = getsize(value);

    if(value_size > max_capacity_){
      return std::nullopt;
    }

    auto it = map_.find(key);

    if(it != map_.end()){
      size_t old_size = getsize(it->second.value);
      curr_capacity_ = curr_capacity_ - old_size + value_size;
      it->second.value = value;
      touch(it);
    }else{
      curr_capacity_ += value_size;
      order_.push_back(key);
      map_[key] = {std::prev(order_.end()), value};
    }

    return cleanup();
  }

  std::optional<Value> get(const Key& key){
    auto it = map_.find(key);
    if(it==map_.end()){
      return std::nullopt;
    }
    touch(it);
    return it->second.value;
  }

  bool erase(const Key& key){
    auto it = map_.find(key);
    if(it==map_.end()) return false;
    curr_capacity_ -= getsize(it->second.value);
    order_.erase(it->second.iterator);
    map_.erase(it);
    return true;
  }

  bool contains(const Key& key) const{
    return map_.find(key) != map_.end();
  }

  size_t get_capacity() const{
    return curr_capacity_;
  }
  size_t get_size() const{
    return map_.size();
  }
  size_t get_max_capacity() const{
    return max_capacity_;
  }

  void set_max_capacity(std::size_t cap){
    max_capacity_=cap;
  }

  void clear(){
    order_.clear();
    map_.clear();
    curr_capacity_ = 0;
  }

  value_iterator begin() noexcept {return value_iterator(map_.begin());}
  value_iterator end() noexcept {return value_iterator(map_.end());}

private:
  std::size_t max_capacity_;
  getsize_predicate getsize;
  std::size_t curr_capacity_;
  std::list<Key> order_;
  std::unordered_map<Key, Mapvalue> map_;


  void touch(map_it it_){
    order_.erase(it_->second.iterator);
    order_.push_back(it_->first);
    it_->second.iterator = std::prev(order_.end());
  }

  std::vector<std::pair<Key,Value>> cleanup(){
    std::vector<std::pair<Key,Value>>removed_values{};
    while(curr_capacity_>max_capacity_ && !order_.empty()){
      auto oldkey = order_.front();
      auto map_it = map_.find(oldkey); //should always work

      size_t removed_size = getsize(map_it->second.value);
      curr_capacity_ -= removed_size;

      order_.pop_front();
      removed_values.push_back({oldkey,std::move(map_it->second.value)});
      map_.erase(oldkey);
    }
    return removed_values;
  }

};



}//end namespace
