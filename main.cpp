#include "raylib/raylib.h"
#include "box2d/box2d.h"
#include <vector>

// Simple entity wrapper
struct Entity {
    b2BodyId bodyId{};
    b2Vec2 extent{};
    Texture texture{};
};

void DrawEntity(const Entity& e) {
    b2Vec2 p = b2Body_GetWorldPoint(e.bodyId, {-e.extent.x, -e.extent.y});
    float radians = b2Rot_GetAngle(b2Body_GetRotation(e.bodyId));
    Vector2 pos{p.x, p.y};
    DrawTextureEx(e.texture, pos, RAD2DEG * radians, 1.0f, WHITE);
}

int main() {
    const int screenW = 800, screenH = 600;
    InitWindow(screenW, screenH, "Spawn boxes with mouse");
    SetTargetFPS(60);

    // Box2D setup
    const float PPM = 32.0f; // pixels per meter
    b2SetLengthUnitsPerMeter(PPM);

    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity.y = 9.8f * PPM;
    b2WorldId worldId = b2CreateWorld(&worldDef);

    // Textures
    Texture groundTex = LoadTexture("./assets/long_ground.png");
    Texture boxTex    = LoadTexture("./assets/box.png");

    b2Vec2 groundExtent{0.5f * groundTex.width, 0.5f * groundTex.height};
    b2Vec2 boxExtent{0.5f * boxTex.width, 0.5f * boxTex.height};

    b2Polygon groundPoly = b2MakeBox(groundExtent.x, groundExtent.y);
    b2Polygon boxPoly    = b2MakeBox(boxExtent.x, boxExtent.y);

    // Ground entity
    std::vector<Entity> entities;

    Entity ground{};
    b2BodyDef bd = b2DefaultBodyDef();
    bd.position = {screenW / 2.0f, screenH - groundExtent.y};
    ground.bodyId = b2CreateBody(worldId, &bd);
    ground.extent = groundExtent;
    ground.texture = groundTex;

    b2ShapeDef sd = b2DefaultShapeDef();
    b2CreatePolygonShape(ground.bodyId, &sd, &groundPoly);

    entities.push_back(ground);

    bool pause = false;

    while (!WindowShouldClose()) {
      if (IsKeyPressed(KEY_P)) pause = !pause;

      if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        Vector2 mouse = GetMousePosition();

        Entity circle{};
        b2BodyDef bd = b2DefaultBodyDef();
        bd.type = b2_dynamicBody;
        bd.position = {mouse.x, mouse.y};
        // bd.motionLocks.angularZ = true;
        circle.bodyId = b2CreateBody(worldId, &bd);
        circle.extent = boxExtent;
        circle.texture = boxTex;

        b2ShapeDef sd = b2DefaultShapeDef();
        sd.density = 100.0f;
        sd.material.friction = 20.0f;
        // b2CreatePolygonShape(box.bodyId, &sd, &boxPoly);
        //
        // entities.push_back(box);
      }

      if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse = GetMousePosition();

        Entity box{};
        b2BodyDef bd = b2DefaultBodyDef();
        bd.type = b2_dynamicBody;
        bd.position = {mouse.x, mouse.y};
        bd.motionLocks.angularZ = true;
        box.bodyId = b2CreateBody(worldId, &bd);
        box.extent = boxExtent;
        box.texture = boxTex;

        b2ShapeDef sd = b2DefaultShapeDef();
        sd.density = 100.0f;
        sd.material.friction = 20.0f;
        b2CreatePolygonShape(box.bodyId, &sd, &boxPoly);

        entities.push_back(box);
      }

      if (!pause) {
        float dt = GetFrameTime();
        b2World_Step(worldId, dt, 4);
      }

      BeginDrawing();
      ClearBackground(DARKGRAY);

      DrawText("Click to spawn boxes. Press P to pause.", 10, 10, 20, LIGHTGRAY);

      for (auto& e : entities) DrawEntity(e);

      EndDrawing();
    }

    UnloadTexture(groundTex);
    UnloadTexture(boxTex);
    CloseWindow();
    return 0;
}
