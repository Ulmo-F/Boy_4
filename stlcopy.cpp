#include <algorithm>
#include <bits/ranges_algo.h>
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <list>
#include <numeric>
#include <ranges>

#include "3D/FileUtilities.h"
#include "3D/Mesh.h"
#include "3D/Plane.h"
#include "3D/Transformation.h"
// #include "Generator.h"

//import std.ranges;

template<typename T, std::ranges::viewable_range R>
void merge_data(std::vector<T>& res, R r) {
    // res.insert(
    //     res.end(),
    //     std::begin(r),
    //     std::end(r)
    // //   std::make_move_iterator(r.begin()),
    // //   std::make_move_iterator(r.end())
    // );
    for (const auto& t : r)
        res.push_back(t);
}

template<typename T, std::ranges::viewable_range R, typename... Args>
void merge_data(std::vector<T>& res, R r, Args... args)
{
    merge_data(res, r);
    merge_data(res, args...);
}

template<typename T, typename... Args>
void func_into_vector(std::vector<T>& res, Args... args) // recursive variadic function
{
//    size_t total_size = (res.size() + (... + args.size()));
//    res.reserve(total_size);
    merge_data(res, args...);
}

int main()
{
    Mesh mesh = readSTL("/home/users/bfanchon/sources/3D/Boy.stl");

//    STL::Plane p{ 1.0, 0.0, 0.0, 0.0 };
    // auto cutter = [&p](const STL::Facet& f) { return STL::cut(f, p); };

    TransformV flipV = [](Vertex v) { v.x = -v.x; return v; };
    TransformV translateV = [](Vertex v) { v.y += 100.; return v; };

    TransformV rotate_90 = [](Vertex v) { std::swap(v.x, v.y); v.x = -v.x; return v; };
    TransformV rotate_180 = [](Vertex v) { v.x = -v.x; v.y = -v.y; return v; };
    TransformV rotate_270 = [](Vertex v) { std::swap(v.x, v.y); v.y = -v.y; return v; };

    TransformV lantern = [](Vertex v) {
            float r = std::hypot(v.x, v.y);
            float t = std::atan2(v.x, v.y) * 3. / 4.;
            v.x = r * std::cos(t);
            v.y = r * std::sin(t);
            return v;
        };


    // auto halfspace = [](const Facet& f) -> bool { 
    //     return std::ranges::all_of(f.vertices, [](auto v) { return v.x < 0; });
    //     //for (int i = 0; i < 3; ++i) if (f.vertices[i].x < 0.) return false; return true;
    // };

    std::function<Facet(Facet)> translate = [](Facet f) { for (auto& v : f.vertices) v.y += 100.; return f;};

    Plane p1{ 0.5, -std::sqrt(3.f)/2.f, 0., 0. };
    Plane p2{ 1., 0., 0., 0. };

//    auto t1 = std::views::filter(mesh.facets, halfspace);
    // auto t2 = std::views::transform(std::views::filter(mesh.facets, halfspace), [translateV](const Facet& f){ return apply(translateV, f); });

    auto truc = mesh.facets
                    | std::views::transform(p1.cutter())
                    | std::views::join
                    | std::views::transform(p2.cutter())
                    | std::views::join
                    | std::views::transform([lantern](const Facet& f){ return applyTransform(lantern, f); })
                ;

    // auto truc1 = mesh.facets
    //         | std::views::transform([&p](const Facet& f){ return p1.cut(f);})
    //         | std::views::join;
    // // auto truc1 = mesh.facets | std::views::filter(halfspace);
    // auto truc2 = mesh.facets
    //                     | std::views::transform([translateV](const Facet& f){ return applyTransform(translateV, f); })
    //                     | std::views::filter(halfspace)
    //                     ;

    //for (const auto& f : 
    //        truc
    //    )
//    for (const auto& f : mesh.facets)
        //out.push_back(f);
    Mesh out;
    out.name = "Boy";

    func_into_vector(out.facets
            , truc
            , truc | std::views::transform([rotate_90](const Facet& f){ return applyTransform(rotate_90, f); })
            , truc | std::views::transform([rotate_180](const Facet& f){ return applyTransform(rotate_180, f); })
            , truc | std::views::transform([rotate_270](const Facet& f){ return applyTransform(rotate_270, f); })
        );

    // for (Facet& f : out.facets)
    //     f.computeNormal();

    writeSTL(out, "/home/users/bfanchon/sources/3D/Boy_out.stl", Format::ascii);

    return 0;
}
