#pragma once
#include <include/classes.hpp>
Image player;
void initiliaze(){
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    SDL_ShowCursor(SDL_DISABLE);
    IMG_Init(IMG_INIT_JPG || IMG_INIT_PNG);
    int button_sprites_size[2] = {2, 1};
    //int pos[2] = {100, 100};
    cursor_img.init(scale_surface(IMG_Load("assets\\Spritesheets\\cursor.png"), 2));
    //playersprite.init(IMG_Load("assets\\Spritesheets\\card.png"), pos, size, 1, 8);
    button_sprites.init(scale_surface(IMG_Load("assets\\Spritesheets\\buttons.png")), button_sprites_size);
    clicksound.init("assets\\Audio\\click.wav");
}