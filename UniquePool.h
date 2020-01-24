#ifndef UNIQUEPOOL_H
#define UNIQUEPOOL_H

#include <memory>
#include <stack>
#include <stdexcept>

#include <mutex>
#include <iostream>

template <class T, class D = std::default_delete<T>>
class SmartUniqueObjectPool
{
 private:
  struct ReturnToPool_Deleter {
    explicit ReturnToPool_Deleter(std::weak_ptr<SmartUniqueObjectPool<T, D>* > pool)
        : pool_(pool) {}

    void operator()(T* ptr) {
      if (auto pool_ptr = pool_.lock())
          (*pool_ptr.get())->add(std::unique_ptr<T, D>{ptr});
      else
          D{}(ptr);
    }
   private:
    std::weak_ptr<SmartUniqueObjectPool<T, D>* > pool_;
  };

 public:
  using ptr_type = std::unique_ptr<T, ReturnToPool_Deleter >;
  using mutex_locker = std::lock_guard<std::mutex>;

  SmartUniqueObjectPool() : this_ptr_(new SmartUniqueObjectPool<T, D>*(this)) {}
  virtual ~SmartUniqueObjectPool(){}

  void add(std::unique_ptr<T, D> t) {
    mutex_locker lock(m_mutex);
    pool_.push(std::move(t));
  }

  ptr_type acquire() {
    if (pool_.empty())
      throw std::out_of_range("Cannot acquire object from an empty pool.");

    mutex_locker lock(m_mutex);
    ptr_type tmp(pool_.top().release(),
                 ReturnToPool_Deleter{
                   std::weak_ptr<SmartUniqueObjectPool<T, D>*>{this_ptr_}});
    pool_.pop();
    return std::move(tmp);
  }

  bool empty() const {
    return pool_.empty();
  }

  size_t size() const {
    return pool_.size();
  }

 private:
  std::shared_ptr<SmartUniqueObjectPool<T, D>* > this_ptr_;
  std::stack<std::unique_ptr<T, D> > pool_;
  std::mutex m_mutex;
};

#endif // UNIQUEPOOL_H
