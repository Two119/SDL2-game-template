#pragma once
#include <include/classes.hpp>
void initiliaze(){
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    SDL_ShowCursor(SDL_DISABLE);
    IMG_Init(IMG_INIT_JPG || IMG_INIT_PNG);
}