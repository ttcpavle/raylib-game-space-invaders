#pragma once
#include "raylib.h"

class Alien{
public:
    Alien(int type, Vector2 position, int health);
    void Update(int direction);
    void Draw();
    int GetType();
    static Texture2D alienImages[3];
    int type;
    int health;
    static void UnloadImages();
    Vector2 position;
    Rectangle getRect();
};