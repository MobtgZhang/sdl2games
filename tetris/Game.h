# ifndef GAME_HEADERS_H
# define GAME_HEADERS_H
# include<SDL2/SDL.h>
# include<SDL2/SDL_image.h>
# include<cstring>

# include "ManagerTexture.h"

class Game{
public:
    Game();
    ~Game();
    void Create();
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event   event;
    bool Gameloop;
    
    int GS_width;
    int GS_height;

    ManagerTexture managerTexture;

    
private:
    void initSDL();
    void Destroy();
};
# endif
