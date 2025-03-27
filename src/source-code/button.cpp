#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>

#include "Render.hpp"
#include "Entity.hpp"
#include "Player.hpp"
#include "button.hpp"


Button::Button(int x, int y, int w1, int h1) {
    hitbox = {x, y, w1, h1};
}

void Button::setPressed(bool status){
    pressed = status;
}

bool Button::GetPressed(){
    return pressed;
}

bool Button::IsPressed() {
    return pressed;
}

void Button::render(SDL_Renderer* renderer) {
    // Render the button using the renderer
    SDL_RenderFillRect(renderer, &hitbox);
}

void Button::setPosition(int x, int y, int w1, int h1) {
    hitbox = {x, y, w1, h1};
}

SDL_Rect *Button::getHitbox() {
    return &hitbox;
}

