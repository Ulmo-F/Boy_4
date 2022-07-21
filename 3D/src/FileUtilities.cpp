#include "FileUtilities.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

// #include "Facet.h"

using namespace std::literals;

namespace {

constexpr size_t STL_BINARY_HDR_SIZE = 80;

constexpr size_t STL_BINARY_META_SIZE = sizeof(uint32_t); // number of triangles

constexpr size_t STL_BINARY_TRIANGLE_SIZE = sizeof(Facet) + 2;
static_assert(STL_BINARY_TRIANGLE_SIZE == 0
    + 3 * sizeof(float)       // 1 normal
    + 3 * 3 * sizeof(float)   // 3 vertices
    + sizeof(uint16_t)        // 1 attribute
    // + 2                       // padding...
);

void consume(std::istream& is, const std::string& expected)
{
    std::string line, label;
    std::getline(is, line);
    std::istringstream ss(line);
    ss >> label;
    if (label != expected)
        throw std::runtime_error("Missing label: "s + expected);
}

std::istringstream getlinestream(std::istream& is)
{
    std::string line;
    std::getline(is, line);
    return std::istringstream(line);
}

void parse_ascii_facet(std::istream& is, Facet& facet)
{
    std::string label, sub;
    auto ss = getlinestream(is);
    ss >> label >> sub;
    if (label == "outer" && sub == "loop") {
        int vi = 0;
        do {
            auto line = getlinestream(is);
            line >> label;
            if (label == "vertex") {
                if (vi < 3) {
                    line >> facet.vertices[vi].x >> facet.vertices[vi].y >> facet.vertices[vi].z;
                }
                ++vi;
            }
        } while(label != "endloop");
        if (vi != 3)
            throw std::runtime_error("Invalid vertex count (only support triangles)");
    }
    else
        throw std::runtime_error("missing label: outer loop");
    consume(is, "endfacet");
}

void parse_ascii_solid(std::istream& is, Mesh& mesh)
{
    std::string line, kind, param;

    while (std::getline(is, line)) {
        std::istringstream ss(line);
        ss >> kind;
        if (kind == "endsolid") {
            break;
        }
        else if (kind == "facet") {
            Facet facet;
            ss >> param;
            if (param != "normal")
                throw std::runtime_error("missing label: normal");
            ss >> facet.normal.x >> facet.normal.y >> facet.normal.z;
            parse_ascii_facet(is, facet);
            mesh.facets.push_back(facet);
        }
        else
            throw std::runtime_error("missing label: endsolid");
    }
}

void parse_ascii_stream(std::istream& is, Mesh& mesh)
{
    std::string line, kind;
    while (std::getline(is, line)) {
        std::istringstream ss(line);
        ss >> kind;
        if (kind == "solid") {
            ss.ignore(line.length(), ' ');
            std::getline(ss, mesh.name);
            parse_ascii_solid(is, mesh);
        }
        else
            throw std::runtime_error("missing label: solid");
    }
}

void parse_binary_stream(std::istream & is, Mesh & mesh)
{
    char header[STL_BINARY_HDR_SIZE + 1]; // header plus null byte
    is.read(header, STL_BINARY_HDR_SIZE);
    header[STL_BINARY_HDR_SIZE] = '\0'; // null terminate just in case
//    mesh.header = header;

    uint32_t num_triangles = 0;
    is.read(reinterpret_cast<char*>(&num_triangles), sizeof(num_triangles));

    mesh.facets.resize(num_triangles);

    unsigned short int unused;
    for (uint32_t ti = 0; ti < num_triangles; ++ti) {
//        Facet facet;
        is.read(reinterpret_cast<char*>(mesh.facets.data() + ti), sizeof(Facet));
        // This field is unused, but must be present
        is.read(reinterpret_cast<char*>(&unused), sizeof(unused));
//        mesh.facets.push_back(facet);
    }
}

void writeAscii(const Mesh& mesh, std::ostream& os)
{
    std::string solid_name = "TBD";
    // std::string solid_name = name;
    // if (solid_name.size() == 0)
    //     solid_name = header;
    // if (solid_name.size() == 0)
    //     solid_name = "stlloader";
    os << "solid " << solid_name << '\n';
    for (const auto &facet : mesh.facets) {
        os << "facet "
           << "normal " << facet.normal.x << " " << facet.normal.y << " " << facet.normal.z << '\n';
        os << "outer loop\n";
        for (int vi = 0; vi < 3; ++vi) {
            os << "vertex " << facet.vertices[vi].x << " " << facet.vertices[vi].y << " " << facet.vertices[vi].z << '\n';
        }
        os << "endloop\n";
        os << "endfacet\n";
    }
    os << "endsolid " << solid_name << '\n';
}

void writeBinary(const Mesh& mesh, std::ostream& os)
{
    std::string padded_header = "header";
    // std::string padded_header = header;
    // if (padded_header.size() == 0) padded_header = name;
    // if (padded_header.size() == 0) padded_header = "stlloader";
    padded_header.resize(STL_BINARY_HDR_SIZE, '\0');
    os << padded_header;
    size_t num_triangles = mesh.facets.size();
    os.write(reinterpret_cast<const char*>(&num_triangles), sizeof(num_triangles));
    constexpr unsigned short int unused = 0;
    for (auto & facet : mesh.facets) {
        os.write(reinterpret_cast<const char*>(&facet), sizeof(Facet));
        // This field is unused, but must be present
        os.write(reinterpret_cast<const char*>(&unused), sizeof(unused));
    }
}

}

void writeSTL(const Mesh& mesh, std::ostream& os, Format format)
{
    switch(format) {
    case Format::ascii:
        writeAscii(mesh, os);
        break;
    case Format::binary:
        writeBinary(mesh, os);
        break;
    }
}

Mesh readSTL(std::istream& is)
{
    Mesh result;

    // Read enough of file to determine its type.
    char header_start[6] = "";
    is.read(header_start, 5);
    header_start[5] = '\0';
    is.seekg(0, is.end);
    size_t file_size = is.tellg();

    // Rewind so parsers can start at the beginning.
    is.seekg(0, is.beg);

    // Ascii files start with "solid"
    bool is_ascii = (std::string(header_start) == "solid");

    // WAR: Some binary files appear to be ASCII, since they violate
    // the rule of not using "solid" in their metadata. Try to detect
    // this
    size_t geom_size = file_size - STL_BINARY_HDR_SIZE - STL_BINARY_META_SIZE;

    if (is_ascii &&
       geom_size > 0 &&
       geom_size % STL_BINARY_TRIANGLE_SIZE == 0) {
        //std::cout << "File looks suspiciously like a binary STL\n";
        is_ascii = false;
    }

    if (is_ascii) {
        parse_ascii_stream(is, result);
    }
    else {
        parse_binary_stream(is, result);
    }

    return result;
}

void writeSTL(const Mesh& mesh, const std::string& filename, Format format)
{
    std::ofstream inputMesh(filename);
    writeSTL(mesh, inputMesh, format);

}

Mesh readSTL(const std::string& filename)
{
    std::ifstream inputMesh(filename);
    return readSTL(inputMesh);
}
