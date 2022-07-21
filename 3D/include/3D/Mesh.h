#pragma once

#include <string>
#include <vector>

#include "Facet.h"

struct Mesh {
    std::vector<Facet> facets;
    std::string name;
    // std::string header;
    // void print(std::ostream&) const;
    // void write_stream(std::ostream& os, Format format = Format::ascii) const;
    // void write_file(const char* filename, Format format = Format::ascii) const;
};
