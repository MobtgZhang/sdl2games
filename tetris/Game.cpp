# include "Game.h"
Game::Game():GS_height(768),GS_width(1024),
        renderer(NULL),window(NULL){
}

void Game::Destroy(){
    if(this->renderer!=NULL){
        SDL_DestroyRenderer(this->renderer);
        this->renderer = NULL;
    }
    if(this->window!=NULL){
        SDL_DestroyWindow(this->window);
        this->window = NULL;
    }
    SDL_Quit();
}

Game::~Game(){
    this->Destroy();   
}

void Game::Create(){
    this->Gameloop = true;
    this->initSDL();

    this->managerTexture.setRender(this->renderer);

    
}

void Game::initSDL(){
    if (SDL_Init(SDL_INIT_EVERYTHING)<0){
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
    }
    Uint32 flagsW = SDL_WINDOW_SHOWN |SDL_WINDOW_OPENGL;

    this->window = SDL_CreateWindow("Trtris",
                    SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,
                    this->GS_width,this->GS_height,flagsW);
    if(this->window==NULL){
        printf("SDL window not create! SDL Error: %s\n", SDL_GetError());
    }

    this->renderer = SDL_CreateRenderer(this->window,-1,SDL_RENDERER_ACCELERATED);

    if(this->renderer==NULL){
        printf("SDL render not create! SDL Error: %s\n", SDL_GetError());
    }

}

