#pragma once
#include <vector>

template <typename T>
class Vector
{
public:
    Vector() = default;

    Vector(size_t size): mVector(size) {}

    Vector(std::initializer_list<T> init): mVector(init) {}

    size_t Size() { return mVector.size(); }

    void Reserve(size_t size) { mVector.reserve(size); }

    void PushBack(const T& value) { mVector.push_back(value); }

    void PushBack(T&& value) { mVector.push_back(std::move(value)); }

    template <typename ...Args>
    T& EmplaceBack(Args&&...args)
    {
        mVector.emplace_back(args...);
        return mVector.back();
    }

    T& Back() { return mVector.back(); }

    void PopBack() { mVector.pop_back(); }

private:
    std::vector<T> mVector;
};
