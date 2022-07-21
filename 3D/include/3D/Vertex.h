#pragma once

struct Vertex {
    float x;
    float y;
    float z;
    bool operator==(const Vertex& v) const { return (x == v.x) && (y == v.y) && (z == v.z); }
};
