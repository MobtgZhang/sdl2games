# ifndef __PLANEGAME_H__
# define __PLANEGAME_H__
# include<SDL2/SDL.h>
# include<SDL2/SDL_image.h>

//游戏状态信息
enum GameState{
    PLANEGAME_START,
    PLANEGAME_END,
    PLANEGAME_PLAYING
};

//主游戏程序
class PlaneGame{
public:
    PlaneGame():m_log(NULL),m_state(PLANEGAME_START),m_window(NULL),m_renderer(NULL),
        m_background_surface(NULL),m_background_texture(NULL){
        this->width = 640;
        this->height = 480;
        this->m_window = NULL;
        this->m_renderer = NULL;
        this->m_log = fopen("plane.log","w");
        if(this->m_log==NULL){
            exit(-1);
        }
        IMG_Init(IMG_INIT_JPG|IMG_INIT_PNG);
    }
    ~PlaneGame(){
        if (this->m_log!=NULL){
            fclose(this->m_log);
        }
        if (this->m_background_surface!=NULL){
            SDL_FreeSurface(this->m_background_surface);
        }
        if(this->m_background_texture!=NULL){
            SDL_DestroyTexture(this->m_background_texture);
        }
        if(this->m_renderer!=NULL){
            SDL_DestroyRenderer(this->m_renderer);
        }
        if(this->m_window!=NULL){
            SDL_DestroyWindow(this->m_window);
        }
        SDL_Quit();
    }
    void loadJPG(char* filename){
        this->m_background_surface = IMG_LoadJPG_RW(SDL_RWFromFile(filename,"wb"));
        if(!this->m_background_surface){
            fprintf(this->m_log,"Cannot open the %s!\n",filename);
            exit(-1);
        }
        this->m_background_texture = SDL_CreateTextureFromSurface(this->m_renderer,this->m_background_surface);
        SDL_FreeSurface(m_background_surface);
        this->m_background_surface = NULL;
    }
    void loadPNG(char* filename){
        this->m_background_surface = IMG_LoadJPG_RW(SDL_RWFromFile(filename,"wb"));
        if(!this->m_background_surface){
            fprintf(this->m_log,"Cannot open the %s!\n",filename);
            exit(-1);
        }
    }
    void init(){
        if(SDL_Init(SDL_INIT_VIDEO)<0){
            fprintf(this->m_log,"Failed to init SDL2! Error:%s\n",SDL_GetError());
            exit(-1);
        }
        this->m_window = SDL_CreateWindow("飞机大战",
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        this->width,
                                        this->height,
                                        SDL_WINDOW_SHOWN);
        if(this->m_window==NULL){
            fprintf(this->m_log,"Cannot create the window! Error: %s\n",SDL_GetError());
            exit(-1);
        }
        this->m_renderer = SDL_CreateRenderer(this->m_window,-1,SDL_RENDERER_ACCELERATED);
        if(this->m_renderer==NULL){
            fprintf(this->m_log,"Cannot create the renderer! Error: %s\n",SDL_GetError());
            exit(-1);
        }
    }
    //主循环函数
    void mainLoop(){
        SDL_Event event;
        while(true){
            this->render();
            if (SDL_PollEvent(&event)){
                if(event.type == SDL_KEYDOWN){
                    //按键事件
                }
                if(event.type == SDL_QUIT){
                    break;
                }
            }
        }
    }
    // 渲染函数
    void render(){
        SDL_SetRenderDrawColor(this->m_renderer,0xFF,0xFF,0xFF,0xFF);
        SDL_RenderClear(this->m_renderer);
        SDL_RenderPresent(this->m_renderer);
    }
private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    SDL_Surface* m_background_surface;
    SDL_Texture* m_background_texture;
    FILE* m_log;
    int width;
    int height;
    GameState m_state;
};
# endif