#pragma once

#include <iostream>
//#include <fstream>
#include <functional>
#include <vector>

//export module STL;

//import std.core;

namespace STL {

struct Vertex {
    float x;
    float y;
    float z;
    void read(std::istream& is);
    void print(std::ostream&) const;
};

inline std::istream& operator>>(std::istream& is, Vertex& v) { v.read(is);  return is; }
inline std::ostream& operator<<(std::ostream& os, const Vertex& v) { v.print(os);  return os; }

using Transform = std::function<Vertex(Vertex)>;

struct Normal {
    float x;
    float y;
    float z;
    void read(std::istream& is);
    void print(std::ostream&) const;
};

inline std::istream& operator>>(std::istream& is, Normal& n) { n.read(is);  return is; }
inline std::ostream& operator<<(std::ostream& os, const Normal& n) { n.print(os);  return os; }

struct Facet {
    Vertex vertices[3];
    Normal normal;
    void print(std::ostream&) const;
};

//using Transform = std::function<Vertex(Vertex)>;
Facet apply(const Transform& func, const Facet& f);

inline std::ostream& operator<<(std::ostream& os, const Facet& f) { f.print(os);  return os; }

enum class Format { ascii, binary };

struct Mesh {
    std::vector<Facet> facets;
    std::string name;
    std::string header;
    void print(std::ostream&) const;
    void write_stream(std::ostream& os, Format format = Format::ascii) const;
    void write_file(const char* filename, Format format = Format::ascii) const;
};

inline std::ostream& operator<<(std::ostream& os, const Mesh& m) { m.print(os);  return os; }

void parse_stream(std::istream & is, Mesh & mesh);

//void parse_stream(const char * filename, Mesh & mesh);
void parse_file(const char* filename, Mesh& mesh);

} // namespace stlloader
