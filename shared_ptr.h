#pragma once

#include <functional>
#include <algorithm>
#include <atomic>

namespace util {

namespace impl {

class DefaultDeleter {
public:
    template <typename T>
    void operator()(T *ptr) {
        delete ptr;
    }
};

template <typename T>
class SharedPtrImpl {
public:
    template <typename D = DefaultDeleter>
    SharedPtrImpl(T *ptr = nullptr, D deleter = D()) : m_ptr(ptr), m_deleter(deleter), m_counter(1) { }

    ~SharedPtrImpl() {
        if (m_deleter) {
            m_deleter(m_ptr);
        } else {
            delete m_ptr;
        }
    }

    T * operator->() const {
        return m_ptr;
    }

    T & operator*() const {
        return *m_ptr;
    }

    T * get() const {
        return m_ptr;
    }

    void hold() {
        ++m_counter;
    }

    bool release() {
        --m_counter;
        return m_counter == 0;
    }

private:
    T *m_ptr;
    std::function<void(T *)> m_deleter;
    std::atomic<unsigned int>  m_counter;
};

} // namespace impl

template <typename T>
class SharedPtr {
public:
    template <typename D = impl::DefaultDeleter>
    explicit SharedPtr(T *ptr = nullptr, D deleter = D()) : m_impl(new impl::SharedPtrImpl<T>(ptr, deleter)) { }

    template <typename U, typename D = impl::DefaultDeleter>
    explicit SharedPtr(U *ptr = nullptr, D deleter = D()) : m_impl(new impl::SharedPtrImpl<T>(ptr, deleter)) { }

    ~SharedPtr() {
        if (m_impl->release()) {
            delete m_impl;
        }
    }

    SharedPtr(const SharedPtr<T> &other) {
        m_impl = other.m_impl;
        m_impl->hold();
    }

    template <typename U>
    SharedPtr(const SharedPtr<U> &other) {
        m_impl = other.m_impl;
        m_impl->hold();
    }

    SharedPtr<T> & operator=(const SharedPtr<T> &other) {
        SharedPtr<T> tmp(other);
        swap(tmp);
        return *this;
    }

    template <typename U>
    SharedPtr<T> & operator=(const SharedPtr<U> &other) {
        SharedPtr<T> tmp(other);
        swap(tmp);
        return *this;
    }

    T * operator->() const {
        return m_impl->operator->();
    }

    T & operator*() const {
        return **m_impl;
    }

    template <typename U>
    bool operator<(const SharedPtr<U> &other) {
        return m_impl->get() < other.get();
    }

    T * get() const {
        return m_impl->get();
    }

    void swap(SharedPtr<T> &other) {
        std::swap(m_impl, other.m_impl);
    }

private:
    impl::SharedPtrImpl<T> *m_impl;
};

template <typename T>
void swap(util::SharedPtr<T> &left, util::SharedPtr<T> &right) {
    left.swap(right);
}

// TODO: make_shared()

} // namespace util
