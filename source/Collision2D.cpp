#include "Collision2D.hpp"
#include "Global.hpp"
#include "Vector2D.hpp"

bool AABB::intersects(const AABB& other) const noexcept{
    bool overlapX = (min.x <= other.max.x) && (max.x >= other.min.x);
    bool overlapY = (min.y <= other.max.y) && (max.y >= other.min.y);

    return overlapX && overlapY;
}

AABB Collision2D::bounds(const Vector2D& position) const noexcept{
    return AABB{ position - halfExtents, position + halfExtents };
}