#pragma once

#include <algorithm>
#include <mutex>

namespace util {

namespace impl {

template <typename T>
class SharedPtrImpl {
public:
    explicit SharedPtrImpl(T *ptr = 0) : m_ptr(ptr), m_counter(1) { }

    ~SharedPtrImpl() {
        delete m_ptr;
    }

    T * operator->() const {
        return m_ptr;
    }

    T & operator*() const {
        return *m_ptr;
    }

    void hold() {
        std::lock_guard<std::mutex> lock(m_mutex);
        ++m_counter;
    }

    bool release() {
        std::lock_guard<std::mutex> lock(m_mutex);
        --m_counter;
        return m_counter == 0;
    }

private:
    T *m_ptr;
    size_t m_counter;
    std::mutex m_mutex;
};

} // namespace impl

template <typename T>
class SharedPtr {
public:
    explicit SharedPtr(T *ptr = 0) : m_impl(new impl::SharedPtrImpl<T>(ptr)) { }

    ~SharedPtr() {
        if (m_impl->release()) {
            delete m_impl;
        }
    }

    SharedPtr(const SharedPtr<T> &other) {
        m_impl = other.m_impl;
        m_impl->hold();
    }

    SharedPtr<T> & operator=(const SharedPtr<T> &other) {
        SharedPtr<T> tmp(other);
        swap(tmp);
        return *this;
    }

    T * operator->() const {
        return m_impl;
    }

    T & operator*() const {
        return **m_impl;
    }

    void swap(SharedPtr<T> &other) {
        std::swap(m_impl, other.m_impl);
    }

private:
    impl::SharedPtrImpl<T> *m_impl;
};

} // namespace util
