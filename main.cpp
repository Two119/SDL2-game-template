#include <include/game.hpp>
int main( int argc, char* args[] ) {
    initiliaze();
    while (playing){
        framestart = SDL_GetTicks();
        SDL_RenderClear(renderer);
        button_state = SDL_GetMouseState(&mouse_pos[0], &mouse_pos[1]);
        keyboard_state = SDL_GetKeyboardState(NULL);
        mousepos = {mouse_pos[0], mouse_pos[1]};
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT){
                playing = false;
            }
        }
        cursor_img.blit(mouse_pos);
        SDL_RenderPresent(renderer);
        frametime = SDL_GetTicks()-framestart;
        if (framedelay > frametime){
            SDL_Delay(framedelay - frametime);
        }
    }
    return 0;
}