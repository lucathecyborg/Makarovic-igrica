#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <ctime>
#include <string>
#include <vector>
#include <fstream>

#include "Render.hpp"
#include "entity.hpp"
#include "Player.hpp"
#include "button.hpp"
#include "levels.hpp"


struct ClueData {
    float x, y;
    float prevX, prevY;
    bool alive;
    bool solved;
};

struct EntityData {
    float x, y;
    float prevX, prevY;
    bool alive;
};

struct SDLRectData {
    int x, y, w, h;
};

struct LevelData {
    int levelNumber;
    SDLRectData srcRect;
    SDLRectData gateRectOpen;
    SDLRectData gateRectClosed;
};

Player Level::player1 = Player();


Level::Level(int levelNumber, int sizeX, int sizeY, int enemyType, int enemyNumber, Render window, SDL_Texture *Tex, SDL_Texture *start1,SDL_Texture *gateC,SDL_Texture *gateO)
{
    mapTex = Tex;
    this->levelNumber = levelNumber;
    this->sizeX = sizeX;
    this->sizeY = sizeY;
    this->enemyType = enemyType;
    this->enemyNumber = enemyNumber;
    this->window = window;
    setupTex=start1;
    gateClosed=gateC;
    gateOpen=gateO;


    // Initialize srcRect and dstRect after setting sizeX and sizeY

    srcRect = {(sizeX - 1920) / 2, (sizeY - 1080) / 2, 1920, 1080};
    dstRect = {0, 0, 1920, 1080};
    createWalls();
    createEnemies();
    createClues();

    
}

void Level::render()
{
    window.renderTexture(mapTex, srcRect, dstRect);
    for (int i = 0; i < enemyNumber; ++i)
    {
        window.renderEntity(enemies.at(i), enemies.at(i).getX(), enemies.at(i).getY(), 100, 100, enemies.at(i).Alive());
    }
    for (int i = 0; i < clues.size(); i++)
    {
        window.renderEntity(clues.at(i), clues.at(i).getX(), clues.at(i).getY(), 100, 100, clues.at(i).Alive());
    }

    if(!levelComplete){
        window.renderTexture1(gateClosed, gateRectClosed);
    }
    else{
        window.renderTexture1(gateOpen,gateRectOpen);
    }
}

void Level::moveL(int x, int y)
{
    srcRect.x += x;
    srcRect.y += y;
}

void Level::createEnemies()
{
    srand(time(NULL));
    SDL_Rect enemyTempRect;
    int EnemyX, EnemyY;
    bool collision;
    for (int i = 0; i < enemyNumber; i++)
    {
        
        do
        {   if(levelNumber==1){
            EnemyX = rand()% 2646 - 413; 
            EnemyY = rand()% 1440 - 227; 

        }
            else if(levelNumber==2){
            EnemyX = rand() %  4944 - 1555;
            EnemyY = rand() %  2727 - 873; 
        }
            else if(levelNumber==3){
            EnemyX = rand() %  7402-2791;
            EnemyY = rand() %  4120 - 1570; 
        }

            enemyTempRect = {EnemyX, EnemyY, 100, 100};
            collision = enemyCheckCollision(&enemyTempRect);
       
        } while (collision);

        if(levelNumber==1)
        enemies.push_back(Entity(EnemyX, EnemyY, window.loadTexture("src/res/gfx/ppl_textures/desert enemy/idle.png"), true));
        else if(levelNumber==2)
        enemies.push_back(Entity(EnemyX, EnemyY, window.loadTexture("src/res/gfx/ppl_textures/forest enemy/idle.png"), true));
        else if(levelNumber==3)
        enemies.push_back(Entity(EnemyX, EnemyY, window.loadTexture("src/res/gfx/ppl_textures/city enemy/idle.png"), true));
    }
}

Entity &Level::useEnemy(int i)
{
    return enemies.at(i);
}

vector<Entity> &Level::getEnemies()
{
    return enemies;
}

int Level::getEnemyNumber()
{
    return enemyNumber;
}

vector<SDL_Rect> &Level::getWalls()
{
    return walls;
}

void Level::resetLevel()
{
    enemies.clear();
    createEnemies();
}

void Level::moveAll(int x, int y)
{
    for (int i = 0; i < walls.size(); i++)
    {
        walls.at(i).x += x;
        walls.at(i).y += y;
    }
    for (int i = 0; i < deathBarriers.size(); i++)
    {
        deathBarriers.at(i).x += x;
        deathBarriers.at(i).y += y;
    }
    for (int i = 0; i < clues.size(); i++)
    {
        clues.at(i).Move(clues.at(i).getX() + x, clues.at(i).getY() + y);
    }

    gateRectClosed.x +=x;
    gateRectOpen.x +=x;
    gateRectOpen.y+=y;
    gateRectClosed.y+=y;

}

bool Level::checkCollision(SDL_Rect *playerHitbox)
{
    for (int i = 0; i < walls.size(); i++)
    {
        if (SDL_HasIntersection(playerHitbox, &walls.at(i)))
        {
            return true;
        }
    }
    return false;
}

void Level::createWalls()
{
    if (levelNumber == 1)
    {
        walls.push_back({-414, -233, 50, 650});
        walls.push_back({-414, 417, 650, 50});
        walls.push_back({186, 117, 50, 350});
        walls.push_back({186, -233, 50, 200});
        walls.push_back({1684, 613, 650, 50});
        walls.push_back({1684, 613, 50, 200});
        walls.push_back({1685, 963, 50, 350});
        walls.push_back({1684, 1263, 650, 50});
        walls.push_back({2284, 663, 50, 650});
        walls.push_back({-414, -233, 650, 60});
        walls.push_back({-369,982,222,89});

        deathBarriers.push_back({2231,-72,33,26});
        deathBarriers.push_back({2231,-233,77,161});
        deathBarriers.push_back({1663,-233,568,170});
        deathBarriers.push_back({1413,-233,250,43});

        gateRectOpen  = {2123,121,207,401};
        gateRectClosed  = {2314,121,31,414};
        }
    else if (levelNumber == 2)
    {
        walls.push_back({-1563, 879, 1229, 50});
        walls.push_back({1517, -879, 50, 968});
        walls.push_back({2607, -24, 50, 135});
        walls.push_back({1517, 64, 1140, 50});
        walls.push_back({-936, 1482, 50, 219});
        walls.push_back({-936, 1125, 50, 375});
        walls.push_back({2607, -879, 50, 603});
        walls.push_back({-384, 879, 50, 603});
        walls.push_back({-886, 1432, 502, 50});
        walls.push_back({-334, 1432, 502, 50});
        walls.push_back({-1563, 904, 50, 1055});
        walls.push_back({-1563, 1909, 1731, 50});

        deathBarriers.push_back({-399, 461, 296,200});
        deathBarriers.push_back({-428,-879,645,855});
        deathBarriers.push_back({-1544, 521, 1116, 279});

        gateRectOpen  = {-1567,-234,234,460};
        gateRectClosed  = {-1586,-241,35,460};
    }
}

bool Level::checkDeathCollision(SDL_Rect *playerHitbox)
{
    for (int i = 0; i < deathBarriers.size(); i++)
    {
        if (SDL_HasIntersection(playerHitbox, &deathBarriers.at(i)))
        {
            return false;
        }
    }
    return true;
}

bool Level::enemyCheckCollision(SDL_Rect *enemyHitbox)
{
    for (int i = 0; i < walls.size(); i++)
    {
        if (SDL_HasIntersection(enemyHitbox, &walls.at(i)))
        {
            return true;
        }
    }
    
    for (int i = 0; i < deathBarriers.size(); i++)
    {
        if (SDL_HasIntersection(enemyHitbox, &deathBarriers.at(i)))
        {
            return true;
        }
    }

 
    return false; // No collision
}

bool Level::checkComplete()
{
    levelComplete = true;
    for (int i = 0; i < enemies.size(); i++)
    {
        if (enemies.at(i).Alive())
        {
            levelComplete = false;
            goto return1;
        }
    }
    for (int i = 0; i < clues.size(); i++)
    {
        if (!clues.at(i).Alive())
        {
            levelComplete = false;
        }
    }
return1:
    return levelComplete;
}

void Level::createClues()
{
    if (levelNumber == 1)
    {
        clues.push_back(Clue(-250, -133, false, window));
        clues[0].SetClueType("question", "What colour is the star on the big crate?", "purple");
        clues.push_back(Clue(-376, 888,  false, window));
        clues[1].SetClueType("clue", "You should look inbetween the bushes");
        clues.push_back(Clue(1490,-70,  false, window));
        clues[2].SetClueType("clue", "Maybe that last house has something");
        clues.push_back(Clue(1793, 1112,  false, window));
        clues[3].SetClueType("clue", "That big rock on the left looks suspicious");
    }
    else if(levelNumber==2){
        clues.push_back(Clue(-540,1215,false,window));
        clues[0].SetClueType("question", "question 1 which will still be made", "answer");
        clues.push_back(Clue(3028,1399,false,window));
        clues[1].SetClueType("clue", "first clue");
        clues.push_back(Clue(-866,-190,false,window));
        clues[2].SetClueType("clue", "second clue");
        clues.push_back(Clue(1753,-791,false,window));
        clues[3].SetClueType("clue", "third clue");
        clues.push_back(Clue(619,629,false,window));
        clues[4].SetClueType("clue", "fourth clue");
    }
}

bool Level::checkClues(Entity *player1)
{
 
    for (int i = 0; i < clues.size(); i++)
    {
        if (SDL_HasIntersection(player1->getHitbox(), clues.at(i).getHitbox()) && clues.at(i).Alive() == false)
        {
            
           clues.at(i).solve(*this);
            clues.at(i).setAlive(clues.at(i).getSolved());
            return false;
        }
    }
    return true;


 

}



void Level::start(){

    window.clear();
    window.renderTexture(setupTex,{0,0,1920,1080},dstRect);
    window.display();
    SDL_Event event;
    bool running=true;

    while(running){
        while(SDL_PollEvent(&event)){
            if(event.type=SDL_KEYDOWN){
                if(event.key.keysym.sym==SDLK_RETURN){
                    running=false;
                }
            }
        }
    }


}

SDL_Rect& Level::getgateRect() {
    if (!levelComplete) {
        return gateRectClosed;
    } else {
        return gateRectOpen;
    }
}



int Level::clueDistance() {
    int closest = 0; 

    for (int i = 0; i < clues.size(); i++) {
        if (!clues[i].Alive()) { 
            if (clues[i].Circle(clues[i].getX(), clues[i].getY(), 200)) {
                closest = std::max(closest, 5); 
            } else if (clues[i].Circle(clues[i].getX(), clues[i].getY(),350)) {
                closest = std::max(closest, 4);
            } else if (clues[i].Circle(clues[i].getX(), clues[i].getY(), 500)) {
                closest = std::max(closest, 3);
            } else if (clues[i].Circle(clues[i].getX(), clues[i].getY(), 650)) {
                closest = std::max(closest, 2);
            } else if (clues[i].Circle(clues[i].getX(), clues[i].getY(), 800)) {
                closest = std::max(closest, 1);
            }
        }
    }
    

    return closest; 
}

void Level::loadPlayer(Entity &x){
    Player temp(&x);
    player1=temp;
   

    
}

vector<Clue>& Level::getClues()
{
    return clues;
}

void Level::loadFromFile(const std::string& filename, Render &window) {
    std::ifstream inFile("level.bin", std::ios::binary);
    if (!inFile) {
        std::cerr << "Error opening file for reading!" << std::endl;
        return;
    }

    // 🔹 Load Level Number
    inFile.read(reinterpret_cast<char*>(&levelNumber), sizeof(levelNumber));

    // 🔹 Load srcRect
    inFile.read(reinterpret_cast<char*>(&srcRect), sizeof(SDL_Rect));

    // 🔹 Load Gate Rects
    inFile.read(reinterpret_cast<char*>(&gateRectOpen), sizeof(SDL_Rect));
    inFile.read(reinterpret_cast<char*>(&gateRectClosed), sizeof(SDL_Rect));

    // 🔹 Load Clues
    size_t clueSize;
    inFile.read(reinterpret_cast<char*>(&clueSize), sizeof(clueSize));
    clues.clear();
    for (size_t i = 0; i < clueSize; i++) {
        ClueData clueData;
        inFile.read(reinterpret_cast<char*>(&clueData), sizeof(ClueData));

        Clue clue;
        clue.Move(clueData.x, clueData.y);
        clue.setAlive(clueData.alive);
        clues.push_back(clue);
    }

    // 🔹 Load Enemies
    size_t enemySize;
    inFile.read(reinterpret_cast<char*>(&enemySize), sizeof(enemySize));
    enemies.clear();
    for (size_t i = 0; i < enemySize; i++) {
        EntityData enemyData;
        inFile.read(reinterpret_cast<char*>(&enemyData), sizeof(EntityData));

        Entity enemy;
        enemy.Move(enemyData.x, enemyData.y);
        enemy.setAlive(enemyData.alive);
        enemies.push_back(enemy);
    }

    // 🔹 Load Walls
    size_t wallSize;
    inFile.read(reinterpret_cast<char*>(&wallSize), sizeof(wallSize));
    walls.clear();
    for (size_t i = 0; i < wallSize; i++) {
        SDL_Rect wall;
        inFile.read(reinterpret_cast<char*>(&wall), sizeof(SDL_Rect));
        walls.push_back(wall);
    }

    // 🔹 Load Death Barriers
    size_t deathBarrierSize;
    inFile.read(reinterpret_cast<char*>(&deathBarrierSize), sizeof(deathBarrierSize));
    deathBarriers.clear();
    for (size_t i = 0; i < deathBarrierSize; i++) {
        SDL_Rect barrier;
        inFile.read(reinterpret_cast<char*>(&barrier), sizeof(SDL_Rect));
        deathBarriers.push_back(barrier);
    }

    inFile.close();
    std::cout << "Level loaded successfully!" << std::endl;
}





void Level::saveToFile(const std::string& filename) {
    std::ofstream outFile("level.bin", std::ios::binary);
    if (!outFile) {
        std::cerr << "Error opening file for writing!" << std::endl;
        return;
    }

    // 🔹 Save Level Number
    outFile.write(reinterpret_cast<const char*>(&levelNumber), sizeof(levelNumber));

    // 🔹 Save srcRect
    outFile.write(reinterpret_cast<const char*>(&srcRect), sizeof(SDL_Rect));

    // 🔹 Save Gate Rects
    outFile.write(reinterpret_cast<const char*>(&gateRectOpen), sizeof(SDL_Rect));
    outFile.write(reinterpret_cast<const char*>(&gateRectClosed), sizeof(SDL_Rect));

    // 🔹 Save Clues
    size_t clueSize = clues.size();
    outFile.write(reinterpret_cast<const char*>(&clueSize), sizeof(clueSize));
    for (auto& clue : clues) {  // Removed `const auto&` to ensure you can call non-const methods
        ClueData clueData = {clue.getX(), clue.getY(), clue.getPrevX(), clue.getPrevY(), clue.Alive(), clue.getSolved()};
        outFile.write(reinterpret_cast<const char*>(&clueData), sizeof(ClueData));
    }

    // 🔹 Save Enemies
    size_t enemySize = enemies.size();
    outFile.write(reinterpret_cast<const char*>(&enemySize), sizeof(enemySize));
    for (auto& enemy : enemies) {  // Removed `const auto&` to ensure you can call non-const methods
        EntityData enemyData = {enemy.getX(), enemy.getY(), enemy.getPrevX(), enemy.getPrevY(), enemy.Alive()};
        outFile.write(reinterpret_cast<const char*>(&enemyData), sizeof(EntityData));
    }

    // 🔹 Save Walls
    size_t wallSize = walls.size();
    outFile.write(reinterpret_cast<const char*>(&wallSize), sizeof(wallSize));
    for (auto& wall : walls) {
        outFile.write(reinterpret_cast<const char*>(&wall), sizeof(SDL_Rect));
    }

    // 🔹 Save Death Barriers
    size_t deathBarrierSize = deathBarriers.size();
    outFile.write(reinterpret_cast<const char*>(&deathBarrierSize), sizeof(deathBarrierSize));
    for (auto& barrier : deathBarriers) {
        outFile.write(reinterpret_cast<const char*>(&barrier), sizeof(SDL_Rect));
    }

    outFile.close();
    std::cout << "Level saved successfully!" << std::endl;
}





