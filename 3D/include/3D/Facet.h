#pragma once

#include "Normal.h"
#include "Vertex.h"

struct Facet {
    Normal normal;
    Vertex vertices[3];
//    unsigned short int unused;
    void computeNormal();
    bool operator==(const Facet&) const;
    Normal getVerticesCrossProduct() const;
    bool isParallel(const Facet&) const;
    bool hasSameSide(const Facet& other) const;
};
