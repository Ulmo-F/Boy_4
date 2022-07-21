#pragma once

#include <iostream>

#include "Mesh.h"

enum class Format {
    ascii, binary
};

void writeSTL(const Mesh&, std::ostream&, Format);
Mesh readSTL(std::istream&);

void writeSTL(const Mesh&, const std::string&, Format);
Mesh readSTL(const std::string&);
