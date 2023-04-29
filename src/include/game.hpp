#pragma once
#include <include/classes.hpp>
void initiliaze(){
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    SDL_ShowCursor(SDL_DISABLE);
    IMG_Init(IMG_INIT_JPG || IMG_INIT_PNG);
    int size[2] = {2, 1};
    cursor_img.init(scale_surface(IMG_Load("assets\\Spritesheets\\cursor.png"), 2));
    button_sprites.init(scale_surface(IMG_Load("assets\\Spritesheets\\buttons.png")), size);
    clicksound.init("assets\\Audio\\click.wav");
}