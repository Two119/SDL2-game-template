# Cpp-template
A template for an SDL2 game made using C++. Use this as a sort of blank slate with all the basic classes, functions and necessary variables you need to create your own games.
# Classes
1) Image class (handles texture rendering for you)
2) Sound class (handles audio playing for you, can play multiple sounds together using as many channels as you like if you modify the number in the Mix_AllocateChannels function on line 199 of core_funcs.hpp)
3) Text class (handles text rendering for you, renders text in white by default but can render in any color if you modify fColor externally)
4) Spritesheet class (splits a SDL_Surface spritesheet into various SDL_Texture s and stores them in an array, which you can access with Spritesheet::get)
5) Surf_Spritesheet class (splits a SDL_Surface spritesheet into various SDL_Surface s and stores them in an array, which you can access with Surf_Spritesheet::get)
6) Sprite class (stores an image along with a chain of rects that represent its outline, can be used for pixel perfect collision)
7) AnimatedSprite class (splits a SDL_Surface spritesheet into various SDL_Texture s and assigns each one to a Sprite object, then updates them one by one to play the animation with rects stored for pixel perfect collision.)
8) Button class (a clickable button capable of taking a function as an argument and executing it when clicked. Also comes with a sound effect)
9) Slider class (a draggable ui slider that can be operated with both keyboard and mouse)
10) fn<> (allows you to store a function as a variable. meant to be used to pass a function as an argument to a button)
11) SpriteStack class (allows you to render sprite-stacks).
    
Important information about the classes: 

- The sprite class requires an SDL_Surface at its original size to be passed in, and the scale you want the image to be can be put in as an argument. By default the Sprite::update method will also render the outline rects. This is a debugging feature and you can remove the for loop at the end of Sprite::update if you just want the image to render. 

- Also, the AnimatedSprite class takes an optional framechange_ argument. This represents the number of frames the program waits before moving on from one frame to the next. By default, this value is set at 8.

- The two spritesheet classes, the SpriteStack class and also the AnimatedSprite class take in a size argument. This represents the number of rows and columns of frames in a spritesheet. So for example for this image:
![image](https://user-images.githubusercontent.com/99664810/235335349-07f310d3-5f31-4038-ba25-1bc1a0f9940d.png) which is a spritesheet for a button, the size would be [2, 1] since there are 2 columns and 1 row.

- The AnimatedSprite class as of currently can only use spritesheets with a single row. This may be improved in the future depending on my requirements for my own games.

- To use fn<>, for example in the following case:
```
int test(const char* c, bool to_print){
    if (to_print) cout << c << endl;
    return 1;
}
```
You have to pass in the return type, then the various types of the arguments like this: ```auto test_func = fn<int, const char*, bool>(test);```

- To use the SpriteStack class, you have to first create a SpriteStack object, then initialize it by passing in the sprite sheet containing your sprite-stack layers, the size (number of rows and columns) of your spritesheet (as explained in the above note about spritesheets) and the spread value of your sprite-stack. Simply load the spritesheet as a surface and then pass it in as an argument; don't pass it in as a ```Surf_Spritesheet``` or ```Spritesheet``` object. WARNING: This class only works if your sprite sheet containing all the sprite-stack layers has all its sprites vertically one on top of the other. Horizontal spritesheet alignment won't work.

# Functions
1) scale_surface (scales a surface up or down)
2) rotate_surface (rotates a surface by a particular angle in degrees)
3) getsize (returns width and height of a texture as an SDL_Point. Width can be accessed as the x value and height as the y value of the point.)
4) get_at (returns the RGB color value of a particular pixel in a SDL_Surface)
5) scale_image (meant to be used on an Image object to scale it up or down)
6) rotate_image (meant to be used on an Image object to rotate it by a particular angle in degrees)
7) SDL_RenderOnTexture (renders a texture on another texture)

# Necessary Variables
1) There are five Uint32 color variable meant to be used for setting colorkeys. These are black, white, red, green and blue.
2) There is a Uint32 * variable button_state, which is updated every frame using SDL_GetMouseState and stores the current state of the mouse buttons. The SDL_Point mousepos variable is also updated along with it.
3) There is a Uint8 * variable keyboard_state which is updated every frame using SDL_GetKeyboardState and stores the current state of the keyboard
4) There is a FRAMES_PER_SECOND value with which you can set the fps at which the game will run.
# 
Finally, the excellent random library by effolkronium (https://github.com/effolkronium/) is also included and the namespace Random is used in core_funcs.hpp. 
