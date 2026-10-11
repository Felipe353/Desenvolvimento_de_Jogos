#include <iostream>
#include "Vector2D.hpp"
#include "RigidBody2D.hpp"
#include "Collision2D.hpp"
#include "Transform2D.hpp"
#include "Global.hpp"

int main() {
    std::cout << "=== INICIANDO TESTES DO MOTOR ===" << std::endl;

    // --- 1. Testando Vector2D ---
    Vector2D v1(3.0f, 4.0f);
    std::cout << "[Vector2D] Comprimento de (3, 4): " << v1.length() << " (Esperado: 5)" << std::endl;

    // --- 2. Testando RigidBody2D (Integração de Euler) ---
    RigidBody2D body;
    body.position = Vector2D(0.0f, 0.0f);
    body.velocity = Vector2D(10.0f, 0.0f);     // Move 10 pixels/s em X
    body.acceleration = Vector2D(0.0f, 5.0f);   // Acelera 5 pixels/s² para baixo em Y

    // Avança o tempo em 1 segundo
    body.integrate(1.0f);
    std::cout << "[RigidBody2D] Posicao apos 1s -> x: " << body.position.x << ", y: " << body.position.y << std::endl;

    // --- 3. Testando Collision2D e AABB ---
    Collision2D boxCollider;
    AABB boxA = boxCollider.bounds(Vector2D(100.0f, 100.0f)); // Caixa centrada em (100, 100)
    AABB boxB = boxCollider.bounds(Vector2D(120.0f, 120.0f)); // Caixa centrada em (120, 120)
    AABB boxC = boxCollider.bounds(Vector2D(300.0f, 300.0f)); // Caixa centrada em (300, 300)

    if (boxA.intersects(boxB)) {
        std::cout << "[Collision2D] Caixa A e Caixa B colidem! (Correto)" << std::endl;
    } else {
        std::cout << "[Collision2D] Caixa A e Caixa B NAO colidem." << std::endl;
    }

    if (boxA.intersects(boxC)) {
        std::cout << "[Collision2D] Caixa A e Caixa C colidem." << std::endl;
    } else {
        std::cout << "[Collision2D] Caixa A e Caixa C NAO colidem. (Correto)" << std::endl;
    }

    // --- 4. Testando Transform2D ---
    Transform2D trans = Transform2D::translation(50.0f, 20.0f);
    Vector2D pontoOriginal(10.0f, 10.0f);
    Vector2D pontoTransformado = trans.transform_point(pontoOriginal);

    std::cout << "[Transform2D] Ponto (10, 10) transladado por (50, 20) -> x: " 
              << pontoTransformado.x << ", y: " << pontoTransformado.y << " (Esperado: 60, 30)" << std::endl;

    std::cout << "=== TESTES CONCLUÍDOS ===" << std::endl;
    return 0;
}