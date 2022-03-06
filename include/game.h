#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "game_level.h"
#include <tuple>

enum Direction
{
    UP,
    RIGHT,
    DOWN,
    LEFT
};

typedef std::tuple<bool, Direction, glm::vec2> Collision;

enum GameState
{
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN,
    GAME_OVER
};

class Game
{
public:
    GameState State;
    bool Keys[1024];
    unsigned int Width, Height;
    bool KeysProcessed[1024];
    clock_t begin_time;

    unsigned int Score;
    int Lightoff;
    Game(unsigned int width, unsigned int height);
    ~Game();

    void Init();
    void ProcessInput(float dt);
    void Update(float dt);
    void DoCollisions();
    void Render();
    std::vector<GameLevel> Levels;
    unsigned int Level;

    // reset
    void ResetLevel();
    void NextLevel();
    void ResetPlayer();
    void gen();
};

#endif