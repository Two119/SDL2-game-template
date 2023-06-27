#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <include/effolkronium/random.hpp>
#include <iostream>
#include <memory>
#include <string>
#include <cmath>
#include <vector>
#include <functional>
#include <SDL2_rotozoom.c>
using namespace std;
using Random = effolkronium::random_static;
int mouse_pos[2];
SDL_Point mousepos;
const Uint8 *keyboard_state;
SDL_Window *win = SDL_CreateWindow("SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1280, 720, SDL_RENDERER_ACCELERATED);
SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
SDL_Rect window_rect;
bool playing = true;
SDL_Event event;
Uint32 black = (255<<24) + (int(0)<<16) + (int(0)<<8) + int(0);
Uint32 white = (255<<24) + (int(255)<<16) + (int(255)<<8) + int(255);
Uint32 red = (255<<24) + (int(255)<<16) + (int(0)<<8) + int(0);
Uint32 green = (255<<24) + (int(0)<<16) + (int(255)<<8) + int(0);
Uint32 blue = (255<<24) + (int(0)<<16) + (int(0)<<8) + int(255);
SDL_Surface *window_surface;
Uint32 button_state;
int FRAMES_PER_SECOND = 60;
int framestart;
int frametime;
int framedelay = ( 1000 / FRAMES_PER_SECOND );
SDL_Surface * scale_surface(SDL_Surface *surf, float factor=4.0){
    return zoomSurface(surf, factor, factor, SMOOTHING_OFF);
}
SDL_Surface * rotate_surface(SDL_Surface *surf, double angle){
    return rotozoomSurface(surf, angle, 1.00, SMOOTHING_OFF);
}
SDL_Point getsize(SDL_Texture *texture) {
    SDL_Point size;
    SDL_QueryTexture(texture, NULL, NULL, &size.x, &size.y);
    return size;
}
SDL_Color get_at(SDL_Surface* pSurface, const int X, const int Y)
{
  // Bytes per pixel
  const Uint8 Bpp = pSurface->format->BytesPerPixel;

  /*
  Retrieve the address to a specific pixel
  pSurface->pixels  = an array containing the SDL_Surface' pixels
  pSurface->pitch       = the length of a row of pixels (in bytes)
  X and Y               = the offset on where on the image to retrieve the pixel; (0, 0) is the upper left corner
  */
  Uint8* pPixel = (Uint8*)pSurface->pixels + Y * pSurface->pitch + X * Bpp;

  Uint32 PixelData = *(Uint32*)pPixel;

  SDL_Color Color = {0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE};

  // Retrieve the RGB values of the specific pixel
  SDL_GetRGB(PixelData, pSurface->format, &Color.r, &Color.g, &Color.b);

  return Color;
}
class Image{
    public:
    SDL_Rect srcrect;
    SDL_Rect dstrect;
    SDL_Texture* tex;
    SDL_Surface* tex_surf;
    void init(SDL_Surface* tex_, Uint32 colorkey = black, bool set = true){
        tex_surf = tex_;         
        if (set){SDL_SetColorKey(tex_surf, SDL_TRUE, colorkey);}
        tex = SDL_CreateTextureFromSurface(renderer, tex_surf);
        srcrect.x = 0;
        srcrect.y = 0;
        srcrect.w = tex_surf->w;
        srcrect.h = tex_surf->h;
        dstrect.w = tex_surf->w;
        dstrect.h = tex_surf->h;
    }
    void init_tex(SDL_Texture* surf){
        tex=surf;
        srcrect.x = 0;
        srcrect.y = 0;
        dstrect.w = getsize(surf).x; 
        dstrect.h = getsize(surf).y; 
        srcrect.w = getsize(surf).x;
        srcrect.h = getsize(surf).y; 
    }
    void blit(int pos[2]){
        dstrect.x = pos[0];
        dstrect.y = pos[1];
        SDL_RenderCopy(renderer, tex, &srcrect, &dstrect);
    }
    SDL_Rect get_src(){
        return srcrect;
    }
    SDL_Rect get_dst(){
        return dstrect;
    }
};
Image scale_image(Image img, float factor=4.0){
    Image new_img;
    new_img.init(zoomSurface(img.tex_surf, factor, factor, SMOOTHING_OFF));
    return new_img;
}
Image rotate_image(Image img, double angle){
    Image new_img;
    new_img.init(rotozoomSurface(img.tex_surf, angle, 1.00, SMOOTHING_OFF));
    return new_img;
}
void SDL_RenderOnTexture(SDL_Texture *src, SDL_Texture *dst, SDL_Rect dstrect, SDL_Rect srcrect){
    SDL_SetRenderTarget(renderer, dst);
    SDL_SetTextureBlendMode(dst, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(renderer, src, &srcrect, &dstrect);
    SDL_SetRenderTarget(renderer, NULL);
}
class Spritesheet{
    public:
    int size[2] = {0, 0};
    vector<vector<SDL_Texture *>> sheet;
    void init(SDL_Surface* sheet_, int size_[2], Uint32 colorkey = black, int spacing=0){
        size[0] = sheet_->w/size_[0];
        size[1] = sheet_->h/size_[1];
        SDL_Rect srcrect;
        srcrect.w = size[0];
        srcrect.h = size[1];
        SDL_Rect dstrect;
        dstrect.w = size[0];
        dstrect.h = size[1];
        dstrect.x = 0;
        dstrect.y = 0;
        vector<SDL_Texture *> vec = {};
        SDL_Texture *sheet_tex = SDL_CreateTextureFromSurface(renderer, sheet_);
        for (int i = 0; i < size_[1]; i++){
            sheet.push_back(vec);
            for (int j = 0; j < size_[0]; j++){
                SDL_Texture *surf = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, size[0]-spacing, size[1]-spacing);
                srcrect.x = (size[0]*j)+spacing;
                srcrect.y = (size[1]*i)+spacing;
                SDL_SetRenderTarget(renderer, surf);
                SDL_SetTextureBlendMode(surf, SDL_BLENDMODE_BLEND);
                SDL_RenderCopy(renderer, sheet_tex, &srcrect, &dstrect);
                sheet[i].push_back(surf);
            }
        }
        SDL_SetRenderTarget(renderer, NULL);
    }
    SDL_Texture * get(int loc[2]){
        return sheet[loc[1]][loc[0]];
    }
};
class Surf_Spritesheet{
    public:
    int size[2] = {0, 0};
    vector<vector<SDL_Surface *>> sheet;
    void init(SDL_Surface* sheet_, int size_[2], Uint32 colorkey = black, int spacing=0){
        size[0] = sheet_->w/size_[0];
        size[1] = sheet_->h/size_[1];
        SDL_Rect srcrect;
        srcrect.w = size[0];
        srcrect.h = size[1];
        SDL_Rect dstrect;
        dstrect.w = size[0];
        dstrect.h = size[1];
        dstrect.x = 0;
        dstrect.y = 0;
        vector<SDL_Surface *> vec = {};
        for (int i = 0; i < size_[1]; i++){
            sheet.push_back(vec);
            for (int j = 0; j < size_[0]; j++){
                SDL_Surface *surf = SDL_CreateRGBSurface(0,size[0]-spacing,size[1]-spacing,32,0,0,0,SDL_ALPHA_OPAQUE);
                srcrect.x = (size[0]*j)+spacing;
                srcrect.y = (size[1]*i)+spacing;
                SDL_BlitSurface(sheet_, &srcrect, surf, &dstrect);
                SDL_SetColorKey(surf, SDL_TRUE, colorkey);
                sheet[i].push_back(surf);
            }
        }
    }
    SDL_Surface * get(int loc[2]){
        return sheet[loc[1]][loc[0]];
    }
};
class Sound {
    public:
    int audio_rate = 22050;
    Uint16 audio_format = AUDIO_S16SYS; 
    int audio_channels = 2; 
    int audio_buffers = 4096;
    Mix_Chunk *sound = NULL;
    int channel;
    int result;
    void init(const char * filename){
        result = Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers);
        result = Mix_AllocateChannels(4);
        sound = Mix_LoadWAV(filename);
    }
    void play(int channel = 0){
        channel = Mix_PlayChannel(channel, sound, 0);
    }
};
class Text{
    public:
    TTF_Font *font;
    SDL_Texture* fontSurface;
    SDL_Color fColor;
    SDL_Rect fontRect;
    void init(const char*filename, int fontsize){
        font = TTF_OpenFont(filename, fontsize);
        fColor.r = 255;
        fColor.g = 255;
        fColor.b = 255;
    }
    void render(const char *c, int pos[2]){
        fontSurface = SDL_CreateTextureFromSurface(renderer, TTF_RenderText_Solid(font, c, fColor));
        fontRect.x = pos[0];
        fontRect.y = pos[1];
        fontRect.w = getsize(fontSurface).x;
        fontRect.h = getsize(fontSurface).y;
        SDL_RenderCopy(renderer, fontSurface, NULL, &fontRect);
    }

};
Image cursor_img;
class Sprite{
    public:
    vector<SDL_Rect> outline_rects;
    Image image;
    Uint32 colkey;
    int position[2];
    vector<vector<int>> outline;
    double scal;
    void init(SDL_Surface *img, int pos[2], double scale = 1, SDL_Color colorkey = {255, 255, 255}){
        scal = scale;
        colkey = (255<<24) + (int(colorkey.b)<<16) + (int(colorkey.g)<<8) + int(colorkey.r);
        image.init(scale_surface(img, scale), colkey);
        bool on_border = false;
        SDL_Rect rec;
        rec.w = scale;
        rec.h = scale;
        for (int j=0; j<img->h; j=j+1){
            for(int i=0; i<img->w; i=i+1){
                on_border = false;
                if (!(get_at(img, i, j).r==colorkey.r && get_at(img, i, j).g==colorkey.g && get_at(img, i, j).b==colorkey.b)){
                    if (i-1>=0){
                        if (get_at(img, i-1, j).r==colorkey.r && get_at(img, i-1, j).g==colorkey.g && get_at(img, i-1, j).b==colorkey.b){
                            on_border = true;
                        }
                    }
                    else{
                        if (!(get_at(img, i, j).r==colorkey.r && get_at(img, i, j).g==colorkey.g && get_at(img, i, j).b==colorkey.b)){
                            on_border = true;
                        }
                    }
                    if (j-1>=0){
                        if (get_at(img, i, j-1).r==colorkey.r && get_at(img, i, j-1).g==colorkey.g && get_at(img, i, j-1).b==colorkey.b){
                            on_border = true;
                        }
                    }
                    else{
                        if (!(get_at(img, i, j).r==colorkey.r && get_at(img, i, j).g==colorkey.g && get_at(img, i, j).b==colorkey.b)){
                            on_border = true;
                        }
                    }
                    if (i+1<img->w){
                        if (get_at(img, i+1, j).r==colorkey.r && get_at(img, i+1, j).g==colorkey.g && get_at(img, i+1, j).b==colorkey.b){
                            on_border = true;
                        }
                    }
                    else{
                        if (!(get_at(img, i, j).r==colorkey.r && get_at(img, i, j).g==colorkey.g && get_at(img, i, j).b==colorkey.b)){
                            on_border = true;
                        }
                    }
                    if (j+1<img->h){
                        if (get_at(img, i, j+1).r==colorkey.r && get_at(img, i, j+1).g==colorkey.g && get_at(img, i, j+1).b==colorkey.b){
                            on_border = true;
                        }
                    }
                    else{
                        if (!(get_at(img, i, j).r==colorkey.r && get_at(img, i, j).g==colorkey.g && get_at(img, i, j).b==colorkey.b)){
                            on_border = true;
                        }
                    }
                }
                if (on_border){
                    vector<int> pos = {i, j};
                    outline.push_back(pos);
                }
            }
        }
        position[0] = pos[0];
        position[1] = pos[1];
        for (int l = 0; l<outline.size(); l=l+1){
            rec.x = pos[0]+outline[l][0]*scale;
            rec.y = pos[1]+outline[l][1]*scale;
            outline_rects.push_back(rec);
        }
        //cout << outline_rects.size() << endl;
    }  
    void update(){ 
        image.blit(position);
        for (int l = 0; l<outline_rects.size(); l=l+1){
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderFillRect(renderer, &outline_rects[l]);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        }
    }
    
};
class AnimatedSprite{
    public:
    vector<Sprite> frames;
    Surf_Spritesheet image;
    int position[2];
    int frame[2] = {0, 0};
    int delay = 0;
    int length;
    Uint32 colkey;
    int framechange;
    void init(SDL_Surface *img, int pos[2], int size_[2], int spacing=1,double scale = 1, SDL_Color colorkey = {255, 255, 255}, int framechange_=8){
        colkey = (255<<24) + (int(colorkey.b)<<16) + (int(colorkey.g)<<8) + int(colorkey.r);
        image.init(img, size_, colkey, spacing);
        for (int i = 0; i < size_[0]; i=i+1){
            Sprite sprite;
            frame[0] = i;
            sprite.init(image.get(frame), pos, scale, colorkey);
            frames.push_back(sprite);
        }    
        length = size_[0]-1;
        framechange = framechange_;
    } 
    void update(){ 
        delay += 1;
        if (delay%framechange==0){
            frame[0]+=1;
            if (frame[0]>length){
                frame[0]=0;
            }
        }
        frames[frame[0]].update();
    }
    
};
AnimatedSprite playersprite;
class SpriteStack{
    public:
    vector<SDL_Surface*> layers;
    float spread = 0;
    SDL_FRect dstrect;
    SDL_Surface *surf;
    SDL_FPoint dst;
    SDL_Texture *tex;
    SDL_Texture *dst_tex;
    SDL_Point tex_size;
    vector<SDL_Texture*> tex_cache;
    void init(SDL_Surface *stack, int size[2], float spread_ = 1){
        spread = spread_;
        Surf_Spritesheet s;
        SDL_SetColorKey(stack, SDL_TRUE, white);
        s.init(stack, size);
        for(int i =0; i< s.sheet.size(); i++){
            layers.push_back(s.sheet[s.sheet.size()-1-i][0]);
        }
        dst_tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, layers[0]->w*2+60, layers[0]->h*2+60);
        tex_size = getsize(dst_tex);
        dstrect.w = layers[0]->w*2;
        dstrect.h = layers[0]->h*2;
        dstrect.x = layers[0]->w;
        dstrect.y = layers[0]->h;
        for (int ang = 0; ang < 361; ang++){
            
            tex_cache.push_back(SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, layers[0]->w*2+60, layers[0]->h*2+60));
            SDL_SetRenderTarget(renderer, tex_cache[ang]);
            SDL_RenderClear(renderer);
            for (int i = 0; i < layers.size(); i++){
                SDL_FreeSurface(surf);
                surf = rotate_surface(layers[i], ang);
                SDL_SetColorKey(surf, SDL_TRUE, black);
                dstrect.x = (float)tex_size.x/2-surf->w/2;
                dstrect.y = (float)tex_size.y/2-surf->h/2-(i*spread);
                dstrect.w = surf->w;
                dstrect.h = surf->h;
                SDL_DestroyTexture(tex);
                tex = SDL_CreateTextureFromSurface(renderer, surf);
                SDL_RenderCopyF(renderer, tex, NULL, &dstrect);
                
            }
            SDL_SetRenderTarget(renderer, NULL);
            
        }
    }
    void render(SDL_Point p, int angle=0, double scale=1){
        dstrect.x = p.x;
        dstrect.y = p.y;
        dstrect.w = layers[0]->w*scale;
        dstrect.h = layers[0]->h*scale;
        if (angle<0) SDL_RenderCopyF(renderer, tex_cache[360+angle%360], NULL, &dstrect);
        if (angle>360) SDL_RenderCopyF(renderer, tex_cache[angle%360], NULL, &dstrect);
        if (angle>=0 && angle<=360) SDL_RenderCopyF(renderer, tex_cache[angle], NULL, &dstrect);
    }
};
template < typename ret, typename T, typename... Rest > using fn = std::function< ret(T, Rest...) >;
template < typename ret, typename T, typename... Rest > ret wrapper(fn< ret, T, Rest... > f, T t, Rest... rest)
{
  return f(t, rest...);
}

template < typename ret, typename T, typename... Rest > ret wrapper(fn< ret, T &, Rest &... > f, T &t, Rest &...rest)
{
  return f(t, rest...);
}
int test(int in){
    cout << "Button works!" << endl;
    return 1;
}
auto test_func = fn<int, int>(test);
