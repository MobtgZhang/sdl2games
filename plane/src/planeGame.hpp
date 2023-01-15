# ifndef __PLANEGAME_H__
# define __PLANEGAME_H__
# include<SDL2/SDL.h>
# include<SDL2/SDL_image.h>
# include<SDL2/SDL_ttf.h>
# include<vector>
# include<random>

# define FRAMERATE 60 //定义游戏中的帧率


//游戏状态信息
enum GameState{
    PLANEGAME_START,
    PLANEGAME_END,
    PLANEGAME_PLAYING
};
//子弹类
class Bullet{
public:
    Bullet(SDL_Renderer* renderer){
        this->m_log = fopen("Bullet.log","w");
        if(this->m_log!=NULL){
            exit(-1);
        }
        this->m_imageSurface = IMG_LoadPNG_RW(SDL_RWFromFile("bullet.png","rb"));
        if(this->m_imageSurface==NULL){
            fprintf(this->m_log,"Cannot open bullet.png! Error: %s\n",IMG_GetError());
            exit(-1);
        }
        this->m_width = this->m_imageSurface->w;
        this->m_height = this->m_imageSurface->h;

        this->m_imageTexture = SDL_CreateTextureFromSurface(renderer,this->m_imageSurface);
        SDL_FreeSurface(m_imageSurface);
        m_imageSurface = NULL;

        if(this->m_imageSurface==NULL){
            fprintf(this->m_log,"Cannot create texture!\n");
            exit(-1);
        }
    }
    ~Bullet(){
        if(this->m_log!=NULL){
            fclose(this->m_log);
        }
        if(this->m_imageTexture!=NULL){
            SDL_DestroyTexture(this->m_imageTexture);
        }
    }
    //子弹类的渲染表示
    void render(){

    }
    std::vector<SDL_Rect> m_position; //存储子弹的位置信息
    int m_v;//表示子弹的速度
private:
    SDL_Surface* m_imageSurface;
    SDL_Texture* m_imageTexture;
    FILE* m_log;//日志

    uint32_t m_start;
    int m_width; //图片的宽
    int m_height; //图片的高
};
//敌人飞机类
class EnemyPlane{
public:
    EnemyPlane(SDL_Renderer* renderer):m_imageSurface(NULL),m_imageTexture(NULL){
        this->m_log = fopen("EnemyPlane.log","w");
        if(this->m_log!=NULL){
            exit(-1);
        }
        this->m_imageSurface = IMG_LoadPNG_RW(SDL_RWFromFile("enemy_plane.png","rb"));
        if(this->m_imageSurface!=NULL){
            fprintf(m_log,"Cannot open enemy_plane.png! Error: %s\n",IMG_GetError());
            exit(-1);
        }
        // 获取图片的宽和高
        this->m_height = this->m_imageSurface->h;
        this->m_width = this->m_imageSurface->w;

        this->m_imageTexture = SDL_CreateTextureFromSurface(renderer,this->m_imageSurface);
        SDL_FreeSurface(this->m_imageSurface);
        this->m_imageSurface = NULL;
        if(this->m_imageTexture==NULL){
            fprintf(this->m_log,"Cannot create texture!\n");
            exit(-1);
        }
    }
    ~EnemyPlane(){
        if(this->m_log!=NULL){
            fclose(this->m_log);
        }
        if(this->m_imageTexture){
            SDL_DestroyTexture(this->m_imageTexture);
        }
    }
    //开始计时
    void start(){
        this->m_start = SDL_GetTicks();
    }
    //渲染
    void render(SDL_Renderer* renderer,int weight){
        uint32_t stop = SDL_GetTicks();
        static std::uniform_int_distribution<int> m_u(0,weight);
        //每间隔一段时间创新新飞机
        if(stop-this->m_start >= 550){
            //创建新飞机
            this->m_e.seed((unsigned)time(NULL)+m_e()); //这里表示更新种子
            
            int w = m_u(this->m_e);
            SDL_Rect srcrect = {w,0,this->m_width,this->m_height};
            
            fprintf(this->m_log,"更新位置:{%d,0%d,%d}",w,this->m_width,this->m_height);
            
            m_position.push_back(srcrect);

            this->m_start = stop; // 重置计时器
        }

        // 飞机移动
        if(!m_position.empty()){
            for(int k =0;k<m_position.size();k++){
                m_position[k].y += 1; // 更新位置信息
                SDL_RenderCopy(renderer,this->m_imageTexture,NULL,&this->m_position[k]);//渲染
            }
        }
    }
    std::vector<SDL_Rect> m_position; // 存储位置信息
    int m_height;
    int m_width;
private:
    SDL_Surface* m_imageSurface;
    SDL_Texture* m_imageTexture;
    FILE* m_log;//日志
    uint32_t m_start;
    
    std::default_random_engine m_e;//随机数引擎
};
//玩家类
class Player{
public:
    Player(SDL_Renderer* renderer):m_playerRenderer(renderer),
        m_imageSurface(NULL),m_imageTexture(NULL){
            this->m_imageSurface = IMG_LoadPNG_RW(SDL_RWFromFile("myPlane.png","rb"));
            this->m_height = this->m_imageSurface->w;
            this->m_width = this->m_imageSurface->h;
            this->m_imageTexture = SDL_CreateTextureFromSurface(this->m_playerRenderer,this->m_imageSurface);
            SDL_FreeSurface(this->m_imageSurface);
            this->m_imageSurface = NULL;
    }
    ~Player(){
        if(this->m_imageTexture!=NULL){
            SDL_DestroyTexture(this->m_imageTexture);
        }
    }
    //对于飞机类的渲染效果
    void render(Sint32 x,Sint32 y){
        // x,y 是飞机的位置信息
        SDL_ShowCursor(false);

        SDL_Rect srcrect = {x,y,this->m_width,this->m_height};
        SDL_RenderCopy(this->m_playerRenderer,this->m_imageTexture,NULL,&srcrect);
    }
private:
    SDL_Surface* m_imageSurface;
    SDL_Texture* m_imageTexture;

    SDL_Renderer* m_playerRenderer;
public:
    int m_width;
    int m_height;
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
        IMG_Quit();

        SDL_Quit();
    }
    void loadJPG(char* filename){
        this->m_background_surface = IMG_LoadJPG_RW(SDL_RWFromFile(filename,"rb"));
        if(!this->m_background_surface){
            fprintf(this->m_log,"Cannot open the %s!\n",filename);
            exit(-1);
        }
        this->m_background_texture = SDL_CreateTextureFromSurface(this->m_renderer,this->m_background_surface);
        SDL_FreeSurface(m_background_surface);
        this->m_background_surface = NULL;
    }
    void loadPNG(char* filename){
        this->m_background_surface = IMG_LoadPNG_RW(SDL_RWFromFile(filename,"rb"));
        if(!this->m_background_surface){
            fprintf(this->m_log,"Cannot open the %s!\n",filename);
            exit(-1);
        }
        this->m_background_texture = SDL_CreateTextureFromSurface(this->m_renderer,this->m_background_surface);
        SDL_FreeSurface(m_background_surface);
        this->m_background_surface = NULL;
    }
    void init(){
        if(SDL_Init(SDL_INIT_VIDEO)<0){
            fprintf(this->m_log,"Failed to init SDL2! Error:%s\n",SDL_GetError());
            exit(-1);
        }
        this->m_background_surface = IMG_LoadJPG_RW(SDL_RWFromFile("start.jpg","rb"));
        this->width = this->m_background_surface->w * 1.5;
        this->height = this->m_background_surface->h * 1.5;
        this->m_window = SDL_CreateWindow("飞机大战",
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        this->width,
                                        this->height,
                                        SDL_WINDOW_SHOWN);
        SDL_FreeSurface(m_background_surface);
        if(this->m_window==NULL){
            fprintf(this->m_log,"Cannot create the window! Error: %s\n",SDL_GetError());
            exit(-1);
        }
        this->m_renderer = SDL_CreateRenderer(this->m_window,-1,SDL_RENDERER_ACCELERATED);
        if(this->m_renderer==NULL){
            fprintf(this->m_log,"Cannot create the renderer! Error: %s\n",SDL_GetError());
            exit(-1);
        }
        this->loadJPG("start.jpg");
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

        switch (m_state){
        case PLANEGAME_START:
            this->loadJPG("start.jpg");
            SDL_RenderCopy(this->m_renderer,this->m_background_texture,NULL,NULL);
            break;
        case PLANEGAME_END:
            this->loadJPG("end.jpg");
            SDL_RenderCopy(this->m_renderer,this->m_background_texture,NULL,NULL);
            break;
        default:
            break;
        }
        SDL_RenderPresent(this->m_renderer);
    }
private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    SDL_Surface* m_background_surface;
    SDL_Texture* m_background_texture;

    char* m_imagefile; // 正在使用的图片
    FILE* m_log; // 日志
    GameState m_state; // 游戏状态

    //窗口大小
    int width;
    int height;
};
# endif