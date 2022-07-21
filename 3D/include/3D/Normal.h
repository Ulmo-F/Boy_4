#pragma once

struct Normal {
    float x;
    float y;
    float z;
    bool operator==(const Normal& n) const { return (x == n.x) && (y == n.y) && (z == n.z); }
    float dotProduct(const Normal&) const;
    Normal crossProduct(const Normal&) const;
};
