#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "sprite_renderer.h"

// Container object for holding all state relevant for a single
// game object entity. Each object in the game likely needs the
// minimal of state as described within GameObject.
class GameObject
{
public:
    // object state
    glm::vec2 Position, Size, Velocity;
    glm::vec3 Color;
    float Rotation;
    
    bool IsSolid; //related to coins
    bool Destroyed;
    bool IsDementor;
    bool IsWall;
    bool IsCoin;
    bool IsSnitch;
    // render state
    Texture2D Sprite;
    // constructor(s)
    GameObject();
    GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
    // draw sprite
    glm::vec2 Move(float dt, unsigned int window_width);
    virtual void Draw(SpriteRenderer &renderer);
};

#endif