#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "text_renderer.h"
#include <tuple>
#include <iostream>
#include <fstream>
#include <time.h>
using namespace std;

SpriteRenderer *Renderer;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_MENU), Keys(), Width(width), Height(height)
{
    Lightoff = 0;
}

Game::~Game()
{
}

// Initial size of the player paddle
const glm::vec2 PLAYER_SIZE(100.0f, 100.0f);
// Initial velocity of the player paddle

const float PLAYER_VELOCITY(150.0f);

GameObject *Player;

void Game::gen()
{
    ofstream myfile;
    myfile.open("./levels/one.lvl");

    for (int i = 0; i <= 9; i++)
    {
        for (int j = 0; j <= 9; j++)
        {
            if (i == 8 && (j == 4 || j == 5))
            {
                myfile << "0";
            }
            else if (i == 1 && j == 0)
                myfile << "4";
            else if (i == 1 && j == 1)
                myfile << "0";
            else if (i == 0 || j == 0 || i == 9 || j == 9)
                myfile << "1";

            else
            {
                int r = rand() % 20;

                if (r > 3)
                    r = 0;

                myfile << r;
            }
            myfile << " ";
        }
        myfile << endl;
    }
    myfile.close();

    myfile.open("./levels/two.lvl");

    for (int i = 0; i <= 9; i++)
    {
        for (int j = 0; j <= 9; j++)
        {
            if (i == 8 && (j == 4 || j == 5))
            {
                myfile << "0";
            }
            else if (i == 1 && j == 0)
                myfile << "4";
            else if (i == 0 || j == 0 || i == 9 || j == 9)
                myfile << "1";

            else
            {
                int r = rand() % 18;
                if (r > 3)
                    r = 0;
                myfile << r;
            }
            myfile << " ";
        }
        myfile << endl;
    }
    myfile.close();
    myfile.open("./levels/three.lvl");

    for (int i = 0; i <= 9; i++)
    {
        for (int j = 0; j <= 9; j++)
        {
            if (i == 8 && (j == 4 || j == 5))
            {
                myfile << "0";
            }
            else if (i == 1 && j == 0)
                myfile << "4";
            else if (i == 0 || j == 0 || i == 9 || j == 9)
                myfile << "1";

            else
            {
                int r = rand() % 16;
                if (r > 3)
                    r = 0;
                myfile << r;
            }
            myfile << " ";
        }
        myfile << endl;
    }
    myfile.close();
}

TextRenderer *Text;

void Game::Init()
{

    this->gen();
    Text = new TextRenderer(this->Width, this->Height);

    Text->Load("fonts/ocraext.TTF", 24);

    // load shaders
    ResourceManager::LoadShader("shaders/sprite.vs", "shaders/sprite.frag", nullptr, "sprite");
    // configure shaders
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
                                      static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    // set render-specific controls
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    // load textures
    // yaha se changes honge mostly
    ResourceManager::LoadTexture("textures/background.png", true, "background");
    ResourceManager::LoadTexture("textures/awesomeface.png", true, "face");
    //ResourceManager::LoadTexture("textures/wall.png", false, "block");

    ResourceManager::LoadTexture("textures/wall.png", true, "wall");

    ResourceManager::LoadTexture("textures/harry.png", true, "paddle");
    ResourceManager::LoadTexture("textures/coin.png", true, "coin");
    ResourceManager::LoadTexture("textures/dementor.png", true, "dementor");
    ResourceManager::LoadTexture("textures/snitch.png", true, "snitch");

    // load levels
    GameLevel one;
    one.Load("levels/one.lvl", this->Width, this->Height);
    GameLevel two;
    two.Load("levels/two.lvl", this->Width, this->Height);
    GameLevel three;
    three.Load("levels/three.lvl", this->Width, this->Height);
    GameLevel four;
    four.Load("levels/four.lvl", this->Width, this->Height);
    this->Levels.push_back(one);
    this->Levels.push_back(two);
    this->Levels.push_back(three);
    this->Levels.push_back(four);
    this->Level = 0;
    this->Score = 0;

    glm::vec2 playerPos = glm::vec2(
        this->Width / 2.0f - PLAYER_SIZE.x / 2.0f,
        this->Height - PLAYER_SIZE.y - 100.0f);
    Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));
}

void Game::NextLevel()
{
    if (this->Level == 0)
    {
        this->Levels[0].Load("levels/two.lvl", this->Width, this->Height / 2);
        ResourceManager::LoadTexture("textures/background1.png", true, "background");
        this->Level = 1;
    }
    else if (this->Level == 1)
    {
        this->Levels[1].Load("levels/three.lvl", this->Width, this->Height / 2);
        ResourceManager::LoadTexture("textures/background2.png", true, "background");
        this->Level = 2;
    }
    else if (this->Level == 2)
        this->State = GAME_WIN;
}

void Game::ResetPlayer()
{
    // reset player
    Player->Size = PLAYER_SIZE;
    glm::vec2 playerPos = glm::vec2(
        this->Width / 2.0f - PLAYER_SIZE.x / 2.0f,
        this->Height - PLAYER_SIZE.y - 100.0f);

    Player->Position = playerPos;
}

Collision CheckCollision(GameObject &one, GameObject &two);

void Game::Update(float dt)
{

    ResourceManager::GetShader("sprite").Use().SetInteger("Lightoff", this->Lightoff);
    ResourceManager::GetShader("sprite").Use().SetVector2f("PlayerPosition", Player->Position);

    this->DoCollisions();

    for (GameObject &box : this->Levels[this->Level].Bricks)
    {
        Collision collision = CheckCollision(*Player, box);

        if (box.IsSnitch)
        {
            if (std::get<0>(collision)) // if collision is true
            {
                this->ResetPlayer();
                this->NextLevel();
            }
        }

        if (box.IsDementor)
        {
            box.Move(dt, this->Width);

            if (std::get<0>(collision)) // if collision is true
            {
                this->Score = 0;
                this->State = GAME_OVER;
                this->ResetPlayer();
            }
        }
    }
}

void Game::ProcessInput(float dt)
{
    if (this->State == GAME_ACTIVE)
    {
        float velocity = PLAYER_VELOCITY * dt;
        // move playerboard
        if (this->Keys[GLFW_KEY_A])
        {
            if (Player->Position.x >= 0.0f)
                Player->Position.x -= velocity;
        }
        if (this->Keys[GLFW_KEY_D])
        {
            if (Player->Position.x <= this->Width - Player->Size.x)
                Player->Position.x += velocity;
        }
        if (this->Keys[GLFW_KEY_W])
        {
            if (Player->Position.y >= 0.0f)
                Player->Position.y -= velocity;
        }
        if (this->Keys[GLFW_KEY_S])
        {
            if (Player->Position.y <= this->Height - Player->Size.y)
                Player->Position.y += velocity;
        }
    }

    if (this->State == GAME_MENU)
    {
        if (this->Keys[GLFW_KEY_ENTER] && !this->KeysProcessed[GLFW_KEY_ENTER])
        {
            this->begin_time = clock();
            this->State = GAME_ACTIVE;
            this->KeysProcessed[GLFW_KEY_ENTER] = true;
        }
    }
}

void Game::Render()
{
    if (this->State == GAME_ACTIVE || this->State == GAME_MENU)
    {
        // draw background
        Renderer->DrawSprite(ResourceManager::GetTexture("background"),
                             glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);
        // draw level
        this->Levels[this->Level].Draw(*Renderer);
        // render player
        Player->Draw(*Renderer);
        unsigned int x = this->Level;
        string str = to_string(x);
        Text->RenderText("LEVEL:" + str, 10.0f, 5.0f, 1.50f, glm::vec3(0.0f, 0.0f, 0.0f));
        unsigned int y = this->Score;
        str = to_string(y);
        Text->RenderText("SCORE:" + str, 800.0f, 5.0f, 1.50f, glm::vec3(0.0f, 0.0f, 0.0f));

        unsigned int z = 5 * (clock() - this->begin_time) / CLOCKS_PER_SEC;

        str = to_string(z);

        Text->RenderText("TIME:" + str, 400.0f, 5.0f, 1.50f, glm::vec3(0.0f, 0.0f, 0.0f));
    }

    if (this->State == GAME_WIN)
    {
        Text->RenderText(
            "GAME FINISHED !! YOU WON !!", 300.0, Height / 2 - 200.0, 1.0, glm::vec3(0.0, 1.0, 0.0));
        Text->RenderText(
            "PRESS ESC TO QUIT", 350.0, Height / 2, 1.0, glm::vec3(1.0, 1.0, 0.0));
    }

    if (this->State == GAME_OVER)
    {
        Text->RenderText(
            "GAME OVER", 400.0, Height / 2 - 200.0, 1.0, glm::vec3(0.0, 1.0, 0.0));
        Text->RenderText(
            "PRESS ESC TO QUIT", 350.0, Height / 2, 1.0, glm::vec3(1.0, 1.0, 0.0));
    }

    if (this->State == GAME_MENU)
    {
        Text->RenderText("PRESS ENTER TO START", 300.0f, Height / 2, 1.0f);
    }
}

//collision detection and resolution

Direction VectorDirection(glm::vec2 target)
{
    glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f),  // up
        glm::vec2(1.0f, 0.0f),  // right
        glm::vec2(0.0f, -1.0f), // down
        glm::vec2(-1.0f, 0.0f)  // left
    };

    float max = 0.0f;
    unsigned int best_match = -1;

    for (unsigned int i = 0; i < 4; i++)
    {
        float dot_product = glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max)
        {
            max = dot_product;
            best_match = i;
        }
    }
    return (Direction)best_match;
}

Collision CheckCollision(GameObject &one, GameObject &two) // AABB - Circle collision
{
    glm::vec2 half_extents(two.Size.x / 2.0f, two.Size.y / 2.0f);

    glm::vec2 center_two(two.Position.x + half_extents.x, two.Position.y + half_extents.y);
    glm::vec2 center_one(one.Position.x + half_extents.x, one.Position.y + half_extents.y);

    glm::vec2 difference = center_one - center_two;

    Direction d = VectorDirection(difference);

    bool collisionX, collisionY;

    if (two.IsCoin && !one.IsDementor)
    {
        collisionX = one.Position.x + one.Size.x * 0.5 >= two.Position.x &&
                     two.Position.x + two.Size.x * 0.5 >= one.Position.x;

        collisionY = one.Position.y + one.Size.y * 0.5 >= two.Position.y &&
                     two.Position.y + two.Size.y * 0.5 >= one.Position.y;
    }
    else
    {
        collisionX = one.Position.x + one.Size.x >= two.Position.x &&
                     two.Position.x + two.Size.x >= one.Position.x;

        collisionY = one.Position.y + one.Size.y >= two.Position.y &&
                     two.Position.y + two.Size.y >= one.Position.y;
    }

    if (collisionX && collisionY)
        return std::make_tuple(true, VectorDirection(difference), difference);
    else
        return std::make_tuple(false, UP, glm::vec2(0.0f, 0.0f));
}

// calculates which direction a vector is facing (N,E,S or W)

void Game::DoCollisions()
{

    for (GameObject &box : this->Levels[this->Level].Bricks)
    {
        if (!box.Destroyed)
        {
            Collision collision = CheckCollision(*Player, box);

            if (std::get<0>(collision)) // if collision is true
            {
                // collect coin and snitch
                if (!box.IsSolid)
                {
                    if (this->Lightoff == 1)
                    {
                        this->Score += 20;
                    }
                    else
                    {
                        this->Score += 10;
                    }
                    box.Destroyed = true;
                    continue;
                    //no other resolution/changes required
                }

                //player collided with wall or dementor
                // collision resolution

                if (box.IsWall)
                {
                    Direction dir = std::get<1>(collision);
                    glm::vec2 diff_vector = std::get<2>(collision);

                    if (dir == LEFT || dir == RIGHT) // horizontal collision
                    {
                        // relocate
                        if (dir == RIGHT)
                            Player->Position.x += ((box.Position.x + box.Size.x) - Player->Position.x); // move player to right
                        else
                            Player->Position.x -= ((Player->Position.x + Player->Size.x) - box.Position.x); // move player to left
                    }
                    else // vertical collision
                    {

                        if (dir == UP)
                            Player->Position.y += ((box.Position.y + box.Size.y) - Player->Position.y); // move player back up
                        else
                            Player->Position.y -= ((Player->Position.y + Player->Size.y) - box.Position.y);
                        ; // move player back down
                    }
                }
                else
                {
                    //player-dementor collision
                }
            }
        }
    }

    // check collisions for dementors

    for (GameObject &dem : this->Levels[this->Level].Bricks)
    {
        if (!dem.Destroyed)
        {
            if (dem.IsDementor)
            {

                for (GameObject &box : this->Levels[this->Level].Bricks)
                {
                    if (dem.Position == box.Position)
                        continue;

                    if (!box.Destroyed)
                    {

                        Collision result = CheckCollision(dem, box);

                        if (std::get<0>(result)) // if collision is true
                        {

                            if (box.IsWall || box.IsCoin || box.IsDementor)
                            {

                                Direction dir = get<1>(result);

                                glm::vec2 diff_vector = get<2>(result);

                                if (dir == LEFT || dir == RIGHT) // horizontal collision
                                {
                                    // relocate
                                    dem.Velocity.x = -dem.Velocity.x; // reverse vertical velocity

                                    if (dir == RIGHT)
                                        dem.Position.x += ((box.Position.x + box.Size.x) - dem.Position.x); // move player to right
                                    else
                                        dem.Position.x -= ((dem.Position.x + dem.Size.x) - box.Position.x); // move player to left
                                }
                                else // vertical collision
                                {
                                    dem.Velocity.y = -dem.Velocity.y; // reverse vertical velocity

                                    if (dir == UP)
                                        dem.Position.y += ((box.Position.y + box.Size.y) - dem.Position.y); // move player back up
                                    else
                                        dem.Position.y -= ((dem.Position.y + dem.Size.y) - box.Position.y);
                                    // move player back down
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
