#include "Vector2D.hpp"
#include <cmath>

Vector2D Vector2D::normalized() const{
    float len = length();
    return *this/len;
}

void Vector2D::normalize(){
    float len = length();
    x = x/len;
    y = y/len;
}

bool Vector2D::equals(const Vector2D& rhs, float tolerance = EPSILON) const noexcept{
    if(std::abs(rhs.x- x)<tolerance && std::abs(rhs.y-y)< tolerance)
        return true;
    return  false;
}

Vector2D Vector2D::operator+(const Vector2D& rhs) const{
    return Vector2D(x+rhs.x,y+rhs.y);
}

Vector2D Vector2D::operator-(const Vector2D& rhs) const{
    return Vector2D(x-rhs.x,y-rhs.y);
}

Vector2D Vector2D::operator*(float scalar) const{
    return Vector2D(x*scalar,y*scalar);
}

Vector2D Vector2D::operator/(float scalar) const{
    if(scalar<EPSILON)
        return;
    return Vector2D(x/scalar,y/scalar);
}