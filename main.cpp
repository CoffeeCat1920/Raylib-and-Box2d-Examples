#include "box2d/box2d.h"
#include "box2d/id.h"
#include "box2d/types.h"
#include "core/b2r.hpp"
#include "raylib/raylib.h"
#include <iostream>

class PhysicsHandler {
  
private:

  float timeStep;

  b2WorldId worldId;
  b2BodyId groundId;
  b2BodyId dynamicId;

public:

  PhysicsHandler() {
    
    timeStep = 1.0f / 60.0f;

    // World
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = (b2Vec2){0.0f, -10.0f}; 
    worldId = b2CreateWorld(&worldDef);
   
    // Ground
    b2BodyDef groundBodyDef = b2DefaultBodyDef();
    groundBodyDef.position = (b2Vec2){0.0f, -10.0f};
    groundId = b2CreateBody(worldId, &groundBodyDef);

    b2Polygon groundBox = b2MakeBox(50.0f, 10.0f);
    b2ShapeDef groundShapeDef = b2DefaultShapeDef();
    b2CreatePolygonShape(groundId, &groundShapeDef, &groundBox);

    // Dynamic
    b2BodyDef dynamicBody = b2DefaultBodyDef();
    dynamicBody.type = b2_dynamicBody;
    dynamicBody.position = (b2Vec2){0.0f, 4.0f};
    dynamicId = b2CreateBody(worldId, &dynamicBody);

    b2Polygon dynamicBox = b2MakeBox(1.0f, 1.0f);
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = 1.0f;
    shapeDef.material.friction = 0.3f;

    b2CreatePolygonShape(dynamicId, &shapeDef, &dynamicBox);
  }

  ~PhysicsHandler() {
    b2DestroyWorld(worldId);
  }

  void Update() {
    b2World_Step(worldId, timeStep, 4);
  } 

  void Draw() {
    b2Vec2 position = b2Body_GetPosition(dynamicId);
    b2Rot rotation = b2Body_GetRotation(dynamicId);
    printf("%4.2f %4.2f %4.2f\n", position.x, position.y, b2Rot_GetAngle(rotation));
    std::cout << "Position: x:" << position.x << " y:" << position.y << "\n"; 

    Rectangle rect = b2r::ConvertBodyToRect(dynamicId);
    DrawRectangleRec(rect, RED);
  }
  
};

int main () {

  PhysicsHandler phyHandler;
  
  InitWindow( 64*5 , 64*5, "Animation Test");

  while ( !WindowShouldClose() )
  { 

    phyHandler.Update();

    BeginDrawing();

    phyHandler.Draw();

    ClearBackground(GRAY);

    EndDrawing();
    
  }

  CloseWindow();

  return 0;
}
