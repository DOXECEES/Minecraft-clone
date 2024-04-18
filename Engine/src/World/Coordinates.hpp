#pragma once

#include <map>

struct Coordinates
{
    float x;
    float y;
    float z;

    // bool operator==(const Coordinates &rhs) const
    // {
    //     return ((this->x == rhs.x) && (this->y == rhs.y) && (this->z == rhs.z));
    // }

    // std::size_t operator()(const Coordinates &key) const
    // {
    //     size_t h1 = std::hash<decltype(key.x)>()(key.x);
    //     size_t h2 = std::hash<decltype(key.y)>()(key.y);
    //     size_t h3 = std::hash<decltype(key.z)>()(key.z);
    //     return ((h1 ^ (h2 << 1)) ^ h3);
    // }
};

inline bool operator==(const Coordinates &c1, const Coordinates &c2)
{
    return ((c1.x == c2.x) && (c1.y == c2.y) && (c1.z == c2.z));
}

namespace std
{
    template <>
    struct hash<Coordinates>
    {
        size_t operator()(const Coordinates &key) const
        {
            size_t h1 = std::hash<decltype(key.x)>()(key.x);
            size_t h2 = std::hash<decltype(key.y)>()(key.y);
            size_t h3 = std::hash<decltype(key.z)>()(key.z);
            return ((h1 ^ (h2 << 1)) ^ h3);
        }
    };

};