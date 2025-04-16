#include "alien.h"

Texture2D Alien::alienImages[3] = {};

Alien::Alien(int type, Vector2 position, int health) {
    this->position = position;
    this->type = type;
    this->health = health;

    if(alienImages[type - 1].id == 0){
        switch(type){
            case 1:
                alienImages[0] = LoadTexture("assets/Graphics/alien_1.png");
                break;
            case 2:
                alienImages[1] = LoadTexture("assets/Graphics/alien_2.png");
                break;
            case 3:
                alienImages[2] = LoadTexture("assets/Graphics/alien_3.png");
                break;
            default:
                alienImages[0] = LoadTexture("assets/Graphics/alien_1.png");
        }
    }
}

void Alien::Draw()
{
    DrawTextureV(alienImages[type - 1], position, WHITE);
}

int Alien::GetType(){
    return type;
}

void Alien::UnloadImages()
{
    for(int i = 0; i < 3; i++){
        UnloadTexture(alienImages[i]);
    }
}

void Alien::Update(int direction){
    position.x += direction;
}

Rectangle Alien::getRect()
{
    return {position.x, position.y,
    float(alienImages[type - 1].width),
    float(alienImages[type - 1].height)
    };
}