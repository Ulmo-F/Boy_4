#include "Facet.h"
#include <cmath>

Normal Facet::getVerticesCrossProduct() const
{
    const float v1_x = vertices[1].x - vertices[0].x;
    const float v1_y = vertices[1].y - vertices[0].y;
    const float v1_z = vertices[1].z - vertices[0].z;
    const float v2_x = vertices[2].x - vertices[1].x;
    const float v2_y = vertices[2].y - vertices[1].y;
    const float v2_z = vertices[2].z - vertices[1].z;

    Normal result;
    result.x = v1_y * v2_z - v1_z * v2_y;
    result.y = v1_z * v2_x - v1_x * v2_z;
    result.z = v1_x * v2_y - v1_y * v2_x;
    return result;    
}

void Facet::computeNormal()
{
    // const float v1_x = vertices[1].x - vertices[0].x;
    // const float v1_y = vertices[1].y - vertices[0].y;
    // const float v1_z = vertices[1].z - vertices[0].z;
    // const float v2_x = vertices[2].x - vertices[1].x;
    // const float v2_y = vertices[2].y - vertices[1].y;
    // const float v2_z = vertices[2].z - vertices[1].z;

    // // v1_x v2_x
    // // v1_y v2_y
    // // v1_z v2_z
    // // v1_x v2_x
    // // v1_y v2_y
    // normal.x = v1_y * v2_z - v1_z * v2_y;
    // normal.y = v1_z * v2_x - v1_x * v2_z;
    // normal.z = v1_x * v2_y - v1_y * v2_x;

    normal = getVerticesCrossProduct();

    float norm = std::hypot(normal.x, normal.y, normal.z);
    if (norm > 0.0)
    {
        normal.x /= norm;
        normal.y /= norm;
        normal.z /= norm;
    }
    else
    {
        normal.x = 1.0;
    }
}

bool Facet::operator==(const Facet& other) const {
    bool result = (normal == other.normal);
    for (size_t i = 0; i < 3; ++i)
        result = result && (vertices[i] == other.vertices[i]);
    return result;
}

bool Facet::isParallel(const Facet& other) const
{
    Normal normal = getVerticesCrossProduct();
    Normal other_normal = other.getVerticesCrossProduct();
    return normal.crossProduct(other_normal).x == 0.
        && normal.crossProduct(other_normal).y == 0.
        && normal.crossProduct(other_normal).z == 0.;
}

bool Facet::hasSameSide(const Facet& other) const
{
    Normal normal = getVerticesCrossProduct();
    Normal other_normal = other.getVerticesCrossProduct();
    return normal.dotProduct(other_normal) > 0.;
}
