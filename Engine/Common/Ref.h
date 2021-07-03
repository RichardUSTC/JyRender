#pragma once

#include <atomic>
#include <cstdint>
#include <cassert>

class Ref
{
public:
    virtual void Destroy() = 0;

    void Acquire()
    {
        mRefCount.fetch_add(1);
    }

    void Release()
    {
        if (mRefCount.fetch_sub(1) == 1)
            Destroy();
    }

    uint64_t GetRefCount() const
    {
        return mRefCount.load();
    }

    virtual ~Ref()
    {
        assert(mRefCount.load() == 0);
    }

private:
    std::atomic_uint64_t mRefCount{0};
};

template <typename T>
class OwnerPtr
{
public:
    OwnerPtr() : mPtr(nullptr) {}

    OwnerPtr(T* ptr) : mPtr(ptr)
    {
        static_assert(std::is_base_of<Ref, T>::value, "T should inherit from `Ref'.");
        if (ptr)
            ptr->Acquire();
    }

    OwnerPtr(const OwnerPtr<T>& ptr)
    {
        if (mPtr)
            mPtr->Release();
        mPtr = ptr.mPtr;
        if (mPtr)
            mPtr->Acquire();
    }

    OwnerPtr<T>& operator =(const OwnerPtr<T>& ptr)
    {
        if (this != &ptr)
        {
            if (mPtr)
                mPtr->Release();
            mPtr = ptr.mPtr;
            if (mPtr)
                mPtr->Acquire();
        }
        return *this;
    }

    OwnerPtr(OwnerPtr<T>&& ptr) noexcept
    {
        if (mPtr)
            mPtr->Release();
        mPtr     = ptr.mPtr;
        ptr.mPtr = nullptr;
    }

    OwnerPtr<T>& operator=(OwnerPtr<T>&& ptr) noexcept
    {
        if (mPtr)
            mPtr->Release();
        mPtr     = ptr.mPtr;
        ptr.mPtr = nullptr;
        return *this;
    }

    T* Get() { return mPtr; }

    const T* Get() const { return mPtr; }

    void Reset(T* ptr = nullptr)
    {
        if (mPtr)
            mPtr->Release();
        mPtr = ptr;
        if (mPtr)
            mPtr->Acquire();
    }

    ~OwnerPtr()
    {
        if (mPtr)
            mPtr->Release();
    }

private:
    T* mPtr{nullptr};
};
