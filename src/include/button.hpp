#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

class Button {
    SDL_Rect hitbox;
    int w, h;
    SDL_Texture* tex;
    bool pressed = false;

public:
    Button(int x, int y, int w1, int h1);
    bool GetPressed();
    void setPressed(bool status);
    bool IsPressed();
    SDL_Rect *getHitbox() ;
    void render(SDL_Renderer* renderer);
    void setPosition(int x, int y, int w, int h);
};