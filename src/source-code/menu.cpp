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

bool menu(Render &window, Level levels[], int levelNumber){
    //writeText(window);
    string names[3]={"level1","level2","level3"};
    bool menu = true;
    SDL_Event event;
    SDL_Texture *menuTex = window.loadTexture("src/res/dev/main menu.png");
    Button start(775, 380, 378, 86);
    Button quit(775, 850, 378, 86);
    Button creditsButton(760, 690, 378, 86);
    Button optionsButton(775, 540, 378, 86);
    Button saveButon(162,540,378,86);
    Button loadButon(162,727,378,86);
    while (menu)
    {
        
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    int mouseX, mouseY;
                    SDL_GetMouseState(&mouseX, &mouseY);
                    SDL_Rect mouseRect = {mouseX, mouseY, 1, 1};
                    if (SDL_HasIntersection(&mouseRect, start.getHitbox()))
                    {
                        menu = false;
                    }
                    else if (SDL_HasIntersection(&mouseRect, quit.getHitbox()))
                    {
                        menu = false;
                        Quit(window);
                        return false;
                    }
                    else if (SDL_HasIntersection(&mouseRect, creditsButton.getHitbox()))
                    {
                        menu = credits(window);
                    }
                    else if (SDL_HasIntersection(&mouseRect, optionsButton.getHitbox()))
                    {
                        menu = options(window);
                    }
                    else if (SDL_HasIntersection(&mouseRect, saveButon.getHitbox()))
                    {
                        levels[levelNumber].saveToFile(names[levelNumber]);
                    }

// Load logic
                    else if (SDL_HasIntersection(&mouseRect, loadButon.getHitbox()))
                    {
                        levelNumber = levels[levelNumber].loadFromFile(names[levelNumber], window);
                        menu = false; // Exit the menu after loading
                    }
                }
            }
            if (event.type == SDL_QUIT)
            {

                menu = false;
                Quit(window);
                return false;
            }
            if (event.type == SDL_KEYUP)
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    menu = false;
                }
            }
        }
        window.renderTexture(menuTex, {0, 0, 1920, 1080}, {0, 0, 1920, 1080});
        window.display();
    }

    return levelNumber; // Return the updated levelNumber
}

bool credits(Render &window)
{
  
    bool credits = true;
    SDL_Event event;
    Button back(1507, 960, 378, 86);
    SDL_Texture *creditsTex = window.loadTexture("src/res/dev/credits.png");
    while (credits)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_KEYUP)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    int mouseX, mouseY;
                    SDL_GetMouseState(&mouseX, &mouseY);
                    SDL_Rect mouseRect = {mouseX, mouseY, 1, 1};
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
        window.renderTexture(creditsTex, {0, 0, 1920, 1080}, {0, 0, 1920, 1080});
        window.display();
    }
    return true;
}

bool options(Render &window)
{
    bool options = true;
    SDL_Event event;
    float brightness = 0;
    Button back(1455, 962, 378, 86);
    SDL_Texture *optionsTex = window.loadTexture("src/res/dev/options.png");
    while (options)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_KEYUP)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    int mouseX, mouseY;
                    SDL_GetMouseState(&mouseX, &mouseY);
                    SDL_Rect mouseRect = {mouseX, mouseY, 1, 1};
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

        window.renderTexture(optionsTex, {0, 0, 1920, 1080}, {0, 0, 1920, 1080});
        window.display();
    }
    return true;
}


