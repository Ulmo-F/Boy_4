#include "Transformation.h"

//using Transform = std::function<Vertex(Vertex)>;

Facet applyTransform(const TransformV& func, Facet f)
{
    // https://timur.audio/how-to-make-a-container-from-a-c20-range
//     auto truc = std::views::transform(f.vertices, nono );
//     template <std::ranges::input_range R>
// vector(R&& r) : vector(r.begin(), r.end()) {
// }
//     f.vertices = to<decltype(Facet::vertices)>(truc);
//     f.vertices = std::views::transform(f.vertices, nono );
//    f.normal = func(f.normal);
//  auto t =   std::views::transform(f.vertices, nono );
//    std::views::transform(f.vertices, [](const Vertex& v) { v; } );
    // std::views::transform(f.vertices, [func](Vertex v) { return func(v); });
    for (auto& v : f.vertices)
        v = func(v);
    return f;
}
