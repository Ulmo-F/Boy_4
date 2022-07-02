#include "stlloader.h"
// #include "Generator.h"
#include <ranges>
#include <algorithm>
#include "cutter.h"

//import std.ranges;

int main()
{
    STL::Mesh mesh;
    STL::parse_file("/home/users/bfanchon/sources/3D/Boy.stl", mesh);

    STL::Mesh out;
    out.name = "Boy";

//    STL::Plane p{ 1.0, 0.0, 0.0, 0.0 };
    // auto cutter = [&p](const STL::Facet& f) { return STL::cut(f, p); };

    auto halfspace = [](const STL::Facet& f) { for (int i = 0; i < 3; ++i) if (f.vertices[i].x < 0.) return false; return true; };

    auto truc =
    //    std::ranges::join(
        mesh.facets | std::ranges::views::filter(halfspace)
//        mesh.facets | std::ranges:: transform(dummy)
    //    )
        ;
    //for (const auto& f : 
    //        truc
    //    )
//    for (const auto& f : mesh.facets)
        //out.push_back(f);
    out.facets = std::vector(std::ranges::begin(truc), std::ranges::end(truc));

    out.write_file("/home/users/bfanchon/sources/3D/Boy_out.stl", STL::Format::ascii);

    return 0;
}

