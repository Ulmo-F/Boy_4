#include "Normal.h"

float Normal::dotProduct(const Normal& n) const
{
    return x * n.x + y * n.y + z * n.z;
}

Normal Normal::crossProduct(const Normal& n) const
{
    Normal result;
    result.x = y * n.z - z * n.y;
    result.y = z * n.x - x * n.z;
    result.z = x * n.y - y * n.x;
    return result;    
}
