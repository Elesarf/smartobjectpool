#ifndef SHAREDPOOL_H
#define SHAREDPOOL_H

#include <memory>
#include <stack>
#include <stdexcept>
#include <mutex>
#include <iostream>

template <class T, class D = std::default_delete<T>>
class SmartSharedObjectPool
{
public:
    struct ReturnToPool_Deleter
    {
        explicit ReturnToPool_Deleter(std::weak_ptr<SmartSharedObjectPool<T, D>* > pool)
            : pool_(pool) {}

        void operator()(T *ptr)
        {
            if (auto pool_ptr = pool_.lock())
                try
                {
                    (*pool_ptr.get())->add({ptr});
                    return;
                }
                catch (std::bad_alloc &) {}
            else
                D{}(ptr);
        }
    private:
        std::weak_ptr<SmartSharedObjectPool<T, D>* > pool_;
    };
    using ptr_type = std::shared_ptr<T>;
    using mutex_locker = std::lock_guard<std::mutex>;

    SmartSharedObjectPool(size_t size, size_t growFactor) :
        this_ptr_(new SmartSharedObjectPool<T, D>*(this)),
        m_size(size),
        m_growfactor(growFactor)
    {
        if(size > 0)
        {
            mutex_locker lock(m_mutex);
            for(size_t i = 0; i < size; ++i)
            {
                pool_.push(new T());
            }
        }
    }

    virtual ~SmartSharedObjectPool()
    {
        while (pool_.size() > 0)
        {
            delete pool_.top();
            pool_.pop();
        }
    }

    void add(T *t)
    {
        mutex_locker lock(m_mutex);
        pool_.push(t);
    }

    ptr_type fetchItem()
    {
        if (pool_.empty())
        {
            if(m_growfactor == 0)
            {
                throw std::out_of_range("Cannot acquire object from an empty pool.");
            }
            else
            {
                std::cout << "grow to " << pool_.size() + m_growfactor << std::endl;
                grow();
            }
        }

        mutex_locker lock(m_mutex);
        ptr_type tmp(pool_.top(), ReturnToPool_Deleter{std::weak_ptr<SmartSharedObjectPool<T, D>*>{this_ptr_}});

        pool_.pop();
        std::cout << "pool size : " << pool_.size() << std::endl;
        return tmp;
    }

    ptr_type getItem()
    {
        if (pool_.empty())
            throw std::out_of_range("Cannot acquire object from an empty pool.");

        mutex_locker lock(m_mutex);
        ptr_type tmp(pool_.top(),
                     ReturnToPool_Deleter
        {
            std::weak_ptr<SmartSharedObjectPool<T, D>*>{this_ptr_}});

        pool_.pop();
        std::cout << "pool size : " << pool_.size() << std::endl;
        return tmp;
    }

    bool empty() const
    {
        return pool_.empty();
    }

    size_t size() const
    {
        return pool_.size();
    }

    size_t growthFactor() const
    {
        return  m_growfactor;
    }

private:

    void grow()
    {
        mutex_locker lock(m_mutex);

        for(size_t i = 0; i < m_growfactor; ++i)
        {
            pool_.push(new T());
        }
    }

    std::shared_ptr<SmartSharedObjectPool<T, D>* > this_ptr_;
    std::stack<T *> pool_;
    std::mutex m_mutex;

    size_t m_size;
    size_t m_growfactor;
};

#endif // SHAREDPOOL_H
