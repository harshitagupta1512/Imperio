#include "game_level.h"

#include <fstream>
#include <sstream>

void GameLevel::Load(const char *file, unsigned int levelWidth, unsigned int levelHeight)
{
    // clear old data
    this->Bricks.clear();
    // load from file
    unsigned int tileCode;
    GameLevel level;
    std::string line;
    std::ifstream fstream(file);
    std::vector<std::vector<unsigned int>> tileData;
    if (fstream)
    {
        while (std::getline(fstream, line)) // read each line from level file
        {
            std::istringstream sstream(line);
            std::vector<unsigned int> row;
            while (sstream >> tileCode) // read each word separated by spaces
                row.push_back(tileCode);
            tileData.push_back(row);
        }
        if (tileData.size() > 0)
            this->init(tileData, levelWidth, levelHeight);
    }
}

void GameLevel::Draw(SpriteRenderer &renderer)
{
    for (GameObject &tile : this->Bricks)
        if (!tile.Destroyed)
            tile.Draw(renderer);
}

bool GameLevel::IsCompleted()
{
    for (GameObject &tile : this->Bricks)
        if (!tile.IsSolid && !tile.Destroyed)
            return false;
    return true;
}

void GameLevel::init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight)
{
    srand(time(0));
    // calculate dimensions
    unsigned int height = tileData.size();
    unsigned int width = tileData[0].size(); // note we can index vector at [0] since this function is only called if height > 0
    float unit_width = levelWidth / static_cast<float>(width), unit_height = levelHeight / height;

    // initialize level tiles based on tileData

    for (unsigned int y = 0; y < height; ++y)
    {
        for (unsigned int x = 0; x < width; ++x)
        {
            // check block type from level data (2D level array)
            if (tileData[y][x] == 1) // solid
            {
                glm::vec2 pos(100.0f * x, 100.0f * y);
                glm::vec2 size(100.0f, 100.0f);
                GameObject obj(pos, size, ResourceManager::GetTexture("wall"));
                obj.IsSolid = true;
                obj.IsDementor = false;
                obj.IsWall = true;
                obj.IsCoin = false;
                obj.IsSnitch = false;
                this->Bricks.push_back(obj);
            }
            else if (tileData[y][x] == 2)
            {
                glm::vec2 pos(100.0f * x, 100.0f * y);
                glm::vec2 size(100.0f, 100.0f);
                GameObject obj(pos, size, ResourceManager::GetTexture("coin"));
                obj.IsSolid = false;
                obj.IsWall = false;
                obj.IsDementor = false;
                obj.IsCoin = true;
                obj.IsSnitch = false;
                this->Bricks.push_back(obj);
            }
            else if (tileData[y][x] == 3)
            {
                glm::vec2 pos(100.0f * x, 100.0f * y);
                glm::vec2 size(100.0f, 100.0f);

                float x = rand() % 100 - rand() % 20;
                float y = rand() % 20 - rand() % 100;

                const glm::vec2 INITIAL_VELOCITY(x, y);

                GameObject obj(pos, size, ResourceManager::GetTexture("dementor"), glm::vec3(1.0f), INITIAL_VELOCITY);
                obj.IsSolid = true;
                obj.IsDementor = true;
                obj.IsSnitch = false;
                obj.IsCoin = false;
                obj.IsWall = false;
                this->Bricks.push_back(obj);
            }
            else if (tileData[y][x] == 4)
            {
                glm::vec2 pos(100.0f * x, 100.0f * y);
                glm::vec2 size(100.0f, 100.0f);
                GameObject obj(pos, size, ResourceManager::GetTexture("snitch"));
                obj.IsSolid = false;
                obj.IsDementor = false;
                obj.IsWall = false;
                obj.IsSnitch = true;
                obj.IsCoin = true;
                this->Bricks.push_back(obj);
            }
        }
    }
}