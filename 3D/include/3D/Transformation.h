#pragma once

#include <concepts>
#include <functional>

#include "Facet.h"
#include "Vertex.h"

using TransformV = std::function<Vertex(Vertex)>;

// template <class T>
// concept integral = std::is_integral_v<T>;

// template <typename T>
// concept TransformV = std::convertible_to<std::invoke_result<T, Facet>, Facet>;


// // template<TransformV t>
// template<typename t>
// Facet TransformF(Facet f)
// {
//     for (auto& v : f.vertices)
//         v = t(v);
//     return f;
// }



Facet applyTransform(const TransformV&, Facet);
// Facet apply(const TransformV&, const Mesh&);
