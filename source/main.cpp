#include <iostream>
#include "Vector2D.hpp"
#include "RigidBody2D.hpp"
#include "Collision2D.hpp"
#include "Transform2D.hpp"
#include "Dungeon2D.hpp"
#include "TileMap.hpp"
#include "Global.hpp"

int main() {
    std::cout << "=== INICIANDO TESTES DO MOTOR ===" << std::endl;

    // --- 1. Testando Vector2D ---
    std::cout << "[DEBUG] Testando Vector2D..." << std::endl;
    Vector2D v1(3.0f, 4.0f);
    std::cout << "[Vector2D] Comprimento de (3, 4): " << v1.length() << " (Esperado: 5)" << std::endl;

    // --- 2. Testando RigidBody2D (Integração de Euler) ---
    std::cout << "[DEBUG] Testando RigidBody2D..." << std::endl;
    RigidBody2D body;
    body.position = Vector2D(0.0f, 0.0f);
    body.velocity = Vector2D(10.0f, 0.0f);    
    body.acceleration = Vector2D(0.0f, 5.0f);   
    body.integrate(1.0f);
    std::cout << "[RigidBody2D] Posicao apos 1s -> x: " << body.position.x << ", y: " << body.position.y << std::endl;

    // --- 3. Testando Collision2D e AABB ---
    std::cout << "[DEBUG] Testando Collision2D..." << std::endl;
    Collision2D boxCollider;
    AABB boxA = boxCollider.bounds(Vector2D(100.0f, 100.0f)); 
    AABB boxB = boxCollider.bounds(Vector2D(120.0f, 120.0f)); 
    AABB boxC = boxCollider.bounds(Vector2D(300.0f, 300.0f)); 

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
    std::cout << "[DEBUG] Testando Transform2D..." << std::endl;
    Transform2D trans = Transform2D::translation(50.0f, 20.0f);
    Vector2D pontoOriginal(10.0f, 10.0f);
    Vector2D pontoTransformado = trans.transform_point(pontoOriginal);

    std::cout << "[Transform2D] Ponto (10, 10) transladado por (50, 20) -> x: " 
              << pontoTransformado.x << ", y: " << pontoTransformado.y << " (Esperado: 60, 30)" << std::endl;

    // --- 5. Testando Dungeon2D e TileMap ---
    std::cout << "[DEBUG] Iniciando Dungeon2D..." << std::endl;
    try {
        Dungeon2D dungeon(80, 50);
        dungeon.generate(42); 

        std::cout << "[DEBUG] Dungeon2D gerado com sucesso. Checando root..." << std::endl;
        if (dungeon.root != nullptr && !dungeon.corridors.empty()) {
            std::cout << "[Dungeon2D] Geracao BSP concluida! Corredores: " << dungeon.corridors.size() << std::endl;
        } else {
            std::cout << "[Dungeon2D] ERRO: root nulo ou corredores vazios." << std::endl;
        }

        std::cout << "[DEBUG] Iniciando TileMap..." << std::endl;
        TileMap tileMap;
        tileMap.loadFromDungeon(dungeon);

        std::cout << "[TileMap] Carregado -> Dimensoes: " << tileMap.width() << "x" << tileMap.height() << std::endl;

        int emptyCol = 0, emptyRow = 0;
        tileMap.randomEmptyCell(emptyCol, emptyRow);
        std::cout << "[TileMap] Celula vazia em -> col: " << emptyCol << ", row: " << emptyRow << std::endl;

        Vector2D worldPos = tileMap.toWorld(emptyCol, emptyRow);
        int cCol = -1, cRow = -1;
        tileMap.toIndices(worldPos, cCol, cRow);
        std::cout << "[TileMap] Coordenadas ok. Salvando CSV..." << std::endl;

        std::string csvPath = "dungeon_test.csv";
        if (tileMap.saveCSV(csvPath)) {
            std::cout << "[TileMap] Salvo com sucesso em '" << csvPath << "'." << std::endl;
        } else {
            std::cout << "[TileMap] Falha ao salvar CSV." << std::endl;
        }

    } catch (const std::exception& e) {
        std::cout << "[EXCEÇÃO CAPTURADA]: " << e.what() << std::endl;
    } catch (...) {
        std::cout << "[EXCEÇÃO DESCONHECIDA CAPTURADA]" << std::endl;
    }

    std::cout << "=== TESTES CONCLUÍDOS ===" << std::endl;
    return 0;
}