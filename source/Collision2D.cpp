#include "Collision2D.hpp"
#include "Global.hpp"
#include "Vector2D.hpp"

bool AABB::intersects(const AABB& other) const noexcept{
    if(other.max.x<=max.x && other.min.x>=min.x)
    {
        if(other.max.y<=max.y && other.min.y>=min.y)
        {
            return true;
        }
    }
    return false;
}

AABB Collision2D::bounds(const Vector2D& position) const noexcept{
    return AABB{ position - halfExtents, position + halfExtents };
}