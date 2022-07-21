#include "Plane.h"

float Plane::dot(const Vertex& v) const
{
    return cx * v.x + cy * v.y + cz * v.z + cc;
}

bool Plane::isIn(const Vertex& v) const
{
    return dot(v) <= 0.;
}

bool Plane::isOnBorder(const Vertex& v) const
{
    return dot(v) == 0.;
}

bool Plane::isIn(const Facet& f) const
{
    return isIn(f.vertices[0]) && isIn(f.vertices[1]) && isIn(f.vertices[2]);
}

std::vector<Facet> Plane::cut(const Facet& f) const
{
    float dots[3];
    for (int i = 0; i < 3; ++i)
        dots[i] = dot(f.vertices[i]);

    const int first_strict_positive = dots[0] > 0 ? 0 : dots[1] > 0 ? 1 : dots[2] > 0 ? 2 : 3;
    const int first_strict_negative = dots[0] < 0 ? 0 : dots[1] < 0 ? 1 : dots[2] < 0 ? 2 : 3;
    const int third_index = 3 - first_strict_positive - first_strict_negative;
    auto project = [points = f.vertices, &dots](int to_move, int target) {
        return Vertex{
            .x = (dots[target] * points[to_move].x - dots[to_move] * points[target].x) / (dots[target] - dots[to_move]),
            .y = (dots[target] * points[to_move].y - dots[to_move] * points[target].y) / (dots[target] - dots[to_move]),
            .z = (dots[target] * points[to_move].z - dots[to_move] * points[target].z) / (dots[target] - dots[to_move])
        };
    };

    std::vector<Facet> result;
    if (first_strict_positive == 3) // Keep
    {
        result.push_back(f);
//        return result;
    }
    else if (first_strict_negative == 3) { // Drop
//        return result;
    }
    else if (dots[third_index] >= 0) // Cut
    {
        // TODO respecter l'orientation
        Facet newFacet(f);
        newFacet.vertices[third_index] = project(third_index, first_strict_negative);
        newFacet.vertices[first_strict_positive] = project(first_strict_positive, first_strict_negative);
        result.push_back(newFacet);
        // result.push_back({
        //     .normal = f.normal,
        //     .vertices {
        //         f.vertices[first_strict_negative],
        //         project(third_index, first_strict_negative),
        //         project(first_strict_positive, first_strict_negative)
        //    }
        // });
//        return result;
    }
    else { // Split
        Facet newFacet(f);
        newFacet.vertices[first_strict_positive] = project(first_strict_positive, third_index);
        result.push_back(newFacet);

        newFacet.vertices[third_index] = newFacet.vertices[first_strict_positive];
        newFacet.vertices[first_strict_positive] = project(first_strict_positive, first_strict_negative);
        result.push_back(newFacet);

        // Vertex v = project(first_strict_positive, third_index);


        // // TODO respecter l'orientation
        // result.push_back({
        //     .normal = f.normal,
        //     .vertices {
        //         f.vertices[first_strict_negative],
        //         f.vertices[third_index],
        //         v
        //     }
        // });
        // // TODO respecter l'orientation
        // result.push_back({
        //     .normal = f.normal,
        //     .vertices {
        //         f.vertices[first_strict_negative],
        //         v,
        //         project(first_strict_positive, first_strict_negative)
        //     }
        // });
    }

    return result;
}

std::vector<Facet> Plane::Cutter::operator()(const Facet& f) const
{
    return p.cut(f);
}

Plane::Cutter Plane::cutter() const
{
    return Cutter{*this};
}
