#include "RigidBody2D.hpp"
#include <cassert>

void RigidBody2D::integrate(float dt) noexcept{
    assert(dt > 0.0f && "Erro: O timestep (dt) deve ser maior que zero!");

    velocity += acceleration * dt;

    position += velocity * dt;
}