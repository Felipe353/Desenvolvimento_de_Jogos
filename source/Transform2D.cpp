#include "Transform2D.hpp"
#include "Vector2D.hpp"
#include <cmath>

Transform2D Transform2D::translation(float tx, float ty) noexcept{
    Transform2D trans;
    trans.m[0][2] = tx;
    trans.m[1][2] = ty;
    return trans;
}

Transform2D Transform2D::rotation(float angle_rad) noexcept{
    Transform2D rot;
    rot.m[0][0] = std::cos(angle_rad);
    rot.m[0][1] = -std::sin(angle_rad);
    rot.m[1][0] = std::sin(angle_rad);
    rot.m[1][1] = std::cos(angle_rad);
    return rot;
}

Transform2D Transform2D::scale(float sx, float sy) noexcept{
    Transform2D sca;
    sca.m[0][0] = sx;
    sca.m[1][1] = sy;
    return sca;
}

Transform2D Transform2D::operator*(const Transform2D& rhs) const noexcept{
    int i,j,k;
    Transform2D result;
    for(i=0;i<3;i++)
    {
        result.m[i][i] = 0;
    }
    for(i=0;i<3;i++)
    {
        for(j=0;j<3;j++)
        {
            for(k=0;k<3;k++)
                result.m[i][j] += m[i][k] * rhs.m[k][j];
        }
    }
    return result;
}

Transform2D& Transform2D::operator*=(const Transform2D& rhs) noexcept{
    int i,j,k;
    Transform2D result;
    for(i=0;i<3;i++)
    {
        result.m[i][i] = 0;
    }
    for(i=0;i<3;i++)
    {
        for(j=0;j<3;j++)
        {
            for(k=0;k<3;k++)
                result.m[i][j] += m[i][k] * rhs.m[k][j];
        }
    }
    for(i=0;i<3;i++)
    {
        for(j=0;j<3;j++)
        {
            m[0][0] = result.m[0][0];
        }
    }
    return *this;
 }

 Vector2D Transform2D::transform_point(const Vector2D& point) const noexcept{
    Vector2D tranformed;
    tranformed.x = m[0][0]*point.x + m[0][1]*point.y + m[0][2];
    tranformed.y = m[1][0]*point.x + m[1][1]*point.y + m[1][2];
    return tranformed;
 }

 Vector2D Transform2D::transform_vector(const Vector2D& direction) const noexcept{
    Vector2D tranformed;
    tranformed.x = m[0][0]*direction.x + m[0][1]*direction.y;
    tranformed.y = m[1][0]*direction.x + m[1][1]*direction.y;
    return tranformed;
}