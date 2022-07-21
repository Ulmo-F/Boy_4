#pragma once

#include <functional>
#include <vector>

#include "Facet.h"
#include "Vertex.h"

struct Plane {
    float cx, cy, cz, cc;
    float dot(const Vertex&) const;
    bool isIn(const Vertex&) const;
    bool isOnBorder(const Vertex&) const;
    bool isIn(const Facet&) const;
    std::vector<Facet> cut(const Facet&) const;

    struct Cutter
    {
        const Plane& p;
        std::vector<Facet> operator()(const Facet& f) const;
    };
    Cutter cutter() const;
};
