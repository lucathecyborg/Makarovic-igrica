#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <ctime>
#include <string>
#include <fstream>

#include "inputANDenemyAI.hpp"
#include "Render.hpp"
#include "Entity.hpp"
#include "Player.hpp"
#include "button.hpp"
#include "menu.hpp"

int menu(Render &window, Level levels[], int &levelNumber) { // Pass levelNumber by reference
    //writeText(window);
    string names[3]={"level1","level2","level3"};
    bool menu = true;
    SDL_Event event;
    SDL_Texture *menuTex = window.loadTexture("src/res/dev/main menu.png");
    SDL_Texture *saveTex = window.loadTexture("src/res/dev/save pressed.png");
    SDL_Texture *loadTex = window.loadTexture("src/res/dev/load pressed.png");
    SDL_Texture *playTex = window.loadTexture("src/res/dev/play pressed.png");
    SDL_Texture *creditsTex = window.loadTexture("src/res/dev/credits pressed.png");
    SDL_Texture *optionsTex = window.loadTexture("src/res/dev/options pressed.png");
    SDL_Texture *quitTex = window.loadTexture("src/res/dev/quit pressed.png");
    Button start(771, 380, 378, 86, playTex);
    Button quit(771, 850, 378, 86, quitTex);
    Button creditsButton(771, 692, 378, 86, creditsTex);
    Button optionsButton(771, 540, 378, 86, optionsTex);
    Button saveButon(177,540,378,86, saveTex);
    Button loadButon(177,727,378,86, loadTex);
    while (menu)
    {   window.renderTexture(menuTex, {0, 0, 1920, 1080}, {0, 0, 1920, 1080});
        int mouseX, mouseY;
                    SDL_GetMouseState(&mouseX, &mouseY);
                    SDL_Rect mouseRect = {mouseX, mouseY, 1, 1};
                    if (SDL_HasIntersection(&mouseRect, start.getHitbox()))
                    {
                        window.renderTexture(playTex, {0, 0, 378, 86}, {771, 380, 378, 86});
                    }
                    else if (SDL_HasIntersection(&mouseRect, quit.getHitbox()))
                    {
                       window.renderTexture(quitTex, {0, 0, 378, 86}, {771, 850, 378, 86});
                    }
                    else if (SDL_HasIntersection(&mouseRect, creditsButton.getHitbox()))
                    {
                       window.renderTexture(creditsTex, {0, 0, 378, 86}, {771, 692, 378, 86});
                    }
                    else if (SDL_HasIntersection(&mouseRect, optionsButton.getHitbox()))
                    {
                       window.renderTexture(optionsTex, {0, 0, 378, 86}, {771, 540, 378, 86});
                    }
                    else if (SDL_HasIntersection(&mouseRect, saveButon.getHitbox()))
                    {
                       window.renderTexture(saveTex, {0, 0, 378, 86}, {177, 540, 378, 86});
                    }
                    else if (SDL_HasIntersection(&mouseRect, loadButon.getHitbox()))
                    {   
                        window.renderTexture(loadTex, {0, 0, 378, 86}, {177, 727, 378, 86});
                    }
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    
                    if (SDL_HasIntersection(&mouseRect, start.getHitbox()))
                    {
                        menu = false;
                    }
                    else if (SDL_HasIntersection(&mouseRect, quit.getHitbox()))
                    {
                        menu = false;
                        Quit(window);
                        return levelNumber; // Return the current levelNumber
                    }
                    else if (SDL_HasIntersection(&mouseRect, creditsButton.getHitbox()))
                    {
                        credits(window);
                    }
                    else if (SDL_HasIntersection(&mouseRect, optionsButton.getHitbox()))
                    {
                        options(window);
                    }
                    else if (SDL_HasIntersection(&mouseRect, saveButon.getHitbox()))
                    {
                        levels[levelNumber].saveToFile(names[levelNumber]);
                    }

// Load logic
                    else if (SDL_HasIntersection(&mouseRect, loadButon.getHitbox()))
                    {   levelNumber=levels[levelNumber].numberCheck()-1;
                        levels[levelNumber].loadFromFile(names[levelNumber], window);
                       // Exit the menu after loading
                    }
                }
            }
            if (event.type == SDL_QUIT)
            {

                menu = false;
                Quit(window);
                return levelNumber; // Return the current levelNumber
            }
            if (event.type == SDL_KEYUP)
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    menu = false;
                }
            }
        }
        
        window.display();
    }

    return levelNumber; // Return the updated levelNumber
}

void credits(Render &window)
{
  
    bool credits = true;
    SDL_Event event;
    SDL_Texture *quitTex = window.loadTexture("src/res/dev/quit pressed3.png");
    Button back(1507, 960, 378, 86, quitTex);
    SDL_Texture *creditsTex = window.loadTexture("src/res/dev/credits.png");
    while (credits)
    {
        window.renderTexture(creditsTex, {0, 0, 1920, 1080}, {0, 0, 1920, 1080});
        int mouseX, mouseY;
                    SDL_GetMouseState(&mouseX, &mouseY);
                    SDL_Rect mouseRect = {mouseX, mouseY, 1, 1};
        if(SDL_HasIntersection(back.getHitbox(), &mouseRect))
        {
            window.renderTexture(quitTex, {0, 0, 378, 86}, {1507, 960, 378, 86});
        }
        
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_KEYUP)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    
                    if (SDL_HasIntersection(&mouseRect, back.getHitbox()))
                    {
                        credits = false;
                    }
                }
                else if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    credits = false;
                }
            }
            if (event.type == SDL_QUIT)
            {
                credits = false;
            }
        }
       
        window.display();
    }
    
}

void options(Render &window)
{
    bool options = true;
    SDL_Event event;
    float brightness = 0;
    SDL_Texture *buttonTex = window.loadTexture("src/res/dev/quit pressed2.png");
    Button back(1506, 979, 378, 86, buttonTex);
    SDL_Texture *optionsTex = window.loadTexture("src/res/dev/options.png");
   
    while (options)
    {
        window.renderTexture(optionsTex, {0, 0, 1920, 1080}, {0, 0, 1920, 1080});
        int mouseX, mouseY;
                    SDL_GetMouseState(&mouseX, &mouseY);
                    SDL_Rect mouseRect = {mouseX, mouseY, 1, 1};

        if (SDL_HasIntersection(back.getHitbox(), &mouseRect))
        {
            window.renderTexture(buttonTex, {0, 0, 378, 86}, {1506, 979, 378, 86});
        }
        
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_KEYUP)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    
                    if (SDL_HasIntersection(&mouseRect, back.getHitbox()))
                    {
                        options = false;
                    }
                }
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    options = false;
                }
            }
            if (event.type == SDL_QUIT)
            {
                options = false;
            }

            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_UP)
                {
                    brightness -= 0.1;
                    SDL_SetWindowBrightness(window.getWindow(), 1 - brightness);
                }
                if (event.key.keysym.sym == SDLK_DOWN)
                {
                    brightness += 0.1;
                    SDL_SetWindowBrightness(window.getWindow(), 1 - brightness);
                }
                if (event.key.keysym.sym == SDLK_RETURN)
                {
                    brightness = 0;
                    SDL_SetWindowBrightness(window.getWindow(), 1);
                }
            }
        }

       
        window.display();
    }
    
}


