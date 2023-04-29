#include <include/core_funcs.hpp>
class Button {
    public:
    int current[2] = {0, 0};
    bool clicked = false;
    SDL_Rect result;
    SDL_Rect rect;
    SDL_Rect src_rect;
    int pos[2];
    void init(int pos_[2]){
        pos[0] = pos_[0];
        pos[1] = pos_[1];
        rect.w = button_sprites.get(current)->w;
        rect.h = button_sprites.get(current)->h;
        src_rect.w = button_sprites.get(current)->w;
        src_rect.h = button_sprites.get(current)->h;
        src_rect.x = 0;
        src_rect.y = 0;
        rect.x = pos[0];
        rect.y = pos[1];
    }
    void update(auto func){
        SDL_BlitSurface(button_sprites.get(current), &src_rect, window_surface, &rect);
        if (SDL_PointInRect(&mousepos, &rect) == SDL_TRUE){
            current[0] = 1;
            if (button_state&SDL_BUTTON(1)){
                wrapper(func, 0);
                if (!clicked)  clicksound.play();
                clicked = true;
                return;
            }
            clicked = false;
        }
        else {
            current[0] = 0;
        }
    }
};
class Slider {
    public:
    int pos[2];
    SDL_Surface *bar_surf;
    SDL_Surface *slider_surf;
    SDL_Rect bar_rect;
    SDL_Rect slider_rect;
    SDL_Rect dstbar;
    SDL_Rect dstslider;
    int value;
    double offset;
    int min;
    int max;
    bool keyboard_controlled;
    void init(int pos_[2], int max_ = 110, int min_=0, bool key = false){
        offset = min_;
        value = min_/10;
        max = max_;
        min = min_;
        pos[0] = pos_[0];
        pos[1] = pos_[1];
        keyboard_controlled = key;
        bar_surf = SDL_CreateRGBSurface(0,110,20,32,0,0,0,SDL_ALPHA_OPAQUE);
        slider_surf = SDL_CreateRGBSurface(0,10,40,32,0,0,0,SDL_ALPHA_OPAQUE);
        bar_rect.x = 0;
        bar_rect.y = 0;
        bar_rect.w = 110;
        bar_rect.h = 20;
        slider_rect.x = 0;
        slider_rect.y = 0;
        slider_rect.w = 10;
        slider_rect.h = 40;
        dstslider.x = pos[0]+min_;
        dstslider.y = pos[1];
        dstslider.w = 10;
        dstslider.h = 40;
        dstbar.x = pos[0];
        dstbar.y = pos[1]+10;
        dstbar.w = 110;
        dstbar.h = 20;
        SDL_FillRect(bar_surf, &bar_rect, blue);
        SDL_FillRect(slider_surf, &slider_rect, white);
    }
    void update(){
        SDL_BlitSurface(bar_surf, &bar_rect, window_surface, &dstbar);
        SDL_BlitSurface(slider_surf, &slider_rect, window_surface, &dstslider);
        if (SDL_PointInRect(&mousepos, &dstbar) == SDL_TRUE && (mouse_pos[0]<pos[0]+max) && (mouse_pos[0]>pos[0]+min)){
            if (button_state&SDL_BUTTON(1)){
                dstslider.x = mouse_pos[0];
                offset = mouse_pos[0]-pos[0];
                value = offset/10;
            }
        }
        if (keyboard_controlled){
            if (keyboard_state[SDL_SCANCODE_RIGHT] && offset<84.5){
                offset+=0.75;
            }
            if (keyboard_state[SDL_SCANCODE_LEFT] && offset>10.5){
                offset-=0.75;
            }
            dstslider.x = pos[0]+offset;
            value = offset/10;
        }
        //cout << offset << ", " << value << endl;
    }
};


