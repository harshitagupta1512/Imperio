#include "game_object.h"

GameObject::GameObject()
    : Position(0.0f, 0.0f), Size(1.0f, 1.0f), Velocity(0.0f), Color(1.0f), Rotation(0.0f), Sprite(), IsSolid(false), Destroyed(false) {}

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity)
    : Position(pos), Size(size), Velocity(velocity), Color(color), Rotation(0.0f), Sprite(sprite), IsSolid(false), Destroyed(false) {}

void GameObject::Draw(SpriteRenderer &renderer)
{
    renderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
}

glm::vec2 GameObject::Move(float dt, unsigned int window_width)
{
    // move the enemy
    this->Position += this->Velocity * dt;

    // check if outside window bounds; if so, reverse velocity and restore at correct position

    if (this->Position.x <= 0.0f)
    {
        this->Velocity.x = -this->Velocity.x;
        this->Position.x = 0.0f;
    }
    else if (this->Position.x + this->Size.x >= window_width)
    {
        this->Velocity.x = -this->Velocity.x;
        this->Position.x = window_width - this->Size.x;
    }
    if (this->Position.y <= 0.0f)
    {
        this->Velocity.y = -this->Velocity.y;
        this->Position.y = 0.0f;
    }
    else if (this->Position.y + this->Size.y >= 1000.0f)
    {
        this->Velocity.y = -this->Velocity.y;
        this->Position.y = 1000.0f - this->Size.y;
    }

    return this->Position;
}