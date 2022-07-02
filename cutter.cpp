#include "cutter.h"

STL::Facet dummy(const STL::Facet& f) {
    return f;
}

std::vector<STL::Facet> cutter(const STL::Facet& f) {
    std::vector<STL::Facet> result;

    result.push_back(f);

    return result;
}
