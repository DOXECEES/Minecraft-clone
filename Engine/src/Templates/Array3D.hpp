#pragma once
#include <vector>

template <class T>
class Array3D
{
public:
    Array3D(size_t x, size_t y, size_t z)
        : x(x), y(y), z(z)
    {
        vec.resize(x * y * z, T());
    }

    void Push(const T &&data)
    {
        vec.push_back(data);
    }

    const T &operator()(size_t _x, size_t _y, size_t _z) const
    {
        return vec[_x + _y * x + _z * y * z];
    }

    T &operator()(size_t _x, size_t _y, size_t _z)
    {
        return vec[_x + _y * x + _z * y * z];
    }

    T *Data()
    {
        return vec.data();
    }

private:
    std::vector<T> vec;
    size_t x;
    size_t y;
    size_t z;
};
