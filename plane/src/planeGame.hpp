#ifndef __PLANEGAME_H__
#define __PLANEGAME_H__

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <random>
#include <vector>

#define FRAMERATE 60 // 帧率

// 游戏状态
enum GameState {
	START,
	END,
	PLAYING
};

// 敌机类
class EnemyPlane
{
public:
	EnemyPlane(SDL_Renderer *renderer) : m_imageSurface(NULL), m_imageTexture(NULL) {
		m_log = fopen("EnemyPlane.log","w");
		if (!m_log) {
			exit(-1);
		}
		
		m_imageSurface = IMG_LoadPNG_RW(SDL_RWFromFile("enemy_plane.png","rb"));
		if (!m_imageSurface) {
			fprintf(m_log,"Cannot open enemy_plane.png! Error: %s\n",IMG_GetError());
			exit(-5);
		}
		
		// 获取图片的宽和高
		m_w = m_imageSurface->w;
		m_h = m_imageSurface->h;
		
		m_imageTexture = SDL_CreateTextureFromSurface(renderer,m_imageSurface);
		SDL_FreeSurface(m_imageSurface);
		m_imageSurface = NULL;
		
		if (!m_imageTexture) {
			fprintf(m_log, "Cannot create texture!\n");
			exit(-3);
		}
	}
	
	~EnemyPlane() {
		if (m_log) {
			fclose(m_log);
		}
		
		if (m_imageTexture) {
			SDL_DestroyTexture(m_imageTexture);
		}
	}
	
	// 开始计时
	void start() {
		m_start = SDL_GetTicks();
	}
	
	// 渲染
	void render(SDL_Renderer *renderer,int weight) {
		uint32_t stop = SDL_GetTicks();
		static std::uniform_int_distribution<int> m_u(0,weight);
		
		// 每隔一段时间创建新飞机
		if (stop - m_start >= 550) {
			// 创建新飞机
			m_e.seed((unsigned)time(NULL) + m_e()); // 更新种子
			
			int w = m_u(m_e);
			SDL_Rect srcrect = {w,0,m_w,m_h};
			
			fprintf(m_log, "更新位置：{%d,0,%d,%d}\n",w,m_w,m_h);
			
			m_position.push_back(srcrect);
			
			m_start = stop; // 重置计时器
		}
		
		// 飞机移动
		if (!m_position.empty()) {
			int i = 0;
			for (i = 0; i < m_position.size(); ++i) {
				m_position[i].y += 1; // 更新位置
				SDL_RenderCopy(renderer,m_imageTexture,NULL,&m_position[i]); // 渲染
			}
		}
	}
	
	std::vector<SDL_Rect> m_position; // 储存位置信息
	
private:
	SDL_Surface *m_imageSurface;
	SDL_Texture *m_imageTexture;
	FILE *m_log; // 日志
	
	uint32_t m_start;
	
	std::default_random_engine m_e; // 随机数引擎
	
public:
	int m_w; // 图片的宽
	int m_h; // 图片的高
};

// 玩家类
class Player
{
public:
	Player(SDL_Renderer *renderer) : m_playerRenderer(renderer), m_imageSurface(NULL),m_imageTexture(NULL) {
		m_imageSurface = IMG_LoadPNG_RW(SDL_RWFromFile("myPlane.png","rb"));
		if (!m_imageSurface) {
			exit(-1);
		}
		
		m_w = m_imageSurface->w;
		m_h = m_imageSurface->h;
		
		m_imageTexture = SDL_CreateTextureFromSurface(m_playerRenderer,m_imageSurface);
		SDL_FreeSurface(m_imageSurface);
		m_imageSurface = NULL;
	}
	
	~Player() {
		if (m_imageTexture) {
			SDL_DestroyTexture(m_imageTexture);
		}
	}
	
	void render(Sint32 x, Sint32 y) {
		SDL_ShowCursor(false);
		
		SDL_Rect srcrect = {x,y,m_w,m_h};
		SDL_RenderCopy(m_playerRenderer,m_imageTexture,NULL,&srcrect);
	}
	
private:
	SDL_Surface *m_imageSurface;
	SDL_Texture *m_imageTexture;
	
 	SDL_Renderer *m_playerRenderer;
	
public:
	int m_w;
	int m_h;
};

// 子弹类
class Bullet
{
public:
	Bullet(SDL_Renderer *renderer) : m_v(500),m_imageSurface(NULL), m_imageTexture(NULL) {
		m_log = fopen("Bullet.log","w");
		if (!m_log) {
			exit(-1);
		}
		
		m_imageSurface = IMG_LoadPNG_RW(SDL_RWFromFile("bullet.png","rb"));
		if (!m_imageSurface) {
			fprintf(m_log,"Cannot open bullet.png! Error: %s\n",IMG_GetError());
			exit(-5);
		}
		
		// 获取图片的宽和高
		m_w = m_imageSurface->w;
		m_h = m_imageSurface->h;
		
		m_imageTexture = SDL_CreateTextureFromSurface(renderer,m_imageSurface);
		SDL_FreeSurface(m_imageSurface);
		m_imageSurface = NULL;
		
		if (!m_imageTexture) {
			fprintf(m_log, "Cannot create texture!\n");
			exit(-3);
		}
	}
	
	~Bullet() {
		if (m_log) {
			fclose(m_log);
		}
		
		if (m_imageTexture) {
			SDL_DestroyTexture(m_imageTexture);
		}
	}
	
	// 开始计时
	void start() {
		m_start = SDL_GetTicks();
	}
	
	// 渲染
	void render(SDL_Renderer *renderer,int x, int y,Player *player,int score) {
		uint32_t stop = SDL_GetTicks();
		
		// 每隔一段时间创建新子弹
		if (stop - m_start >= m_v) {
			// 创建新子弹
			SDL_Rect srcrect = {x + (player->m_w / 2),y,m_w,m_h};
			
			m_position.push_back(srcrect);
			
			if (score >= 100) {
				SDL_Rect srcrect1 = {x,y,m_w,m_h};
			
				m_position.push_back(srcrect1);
				
				SDL_Rect srcrect2 = {x + player->m_w,y,m_w,m_h};
			
				m_position.push_back(srcrect2);
			}
			
			m_start = stop; // 重置计时器
		}
		
		// 子弹移动
		if (!m_position.empty()) {
			int i = 0;
			for (i = 0; i < m_position.size(); ++i) {
				m_position[i].y -= 2; // 更新位置
				SDL_RenderCopy(renderer,m_imageTexture,NULL,&m_position[i]); // 渲染
			}
		}
	}
	
	std::vector<SDL_Rect> m_position; // 储存位置信息
	int m_v; // 子弹射速
	
private:
	SDL_Surface *m_imageSurface;
	SDL_Texture *m_imageTexture;
	FILE *m_log; // 日志
	
	uint32_t m_start;
	
	int m_w; // 图片的宽
	int m_h; // 图片的高
};

class PlaneGame
{
public:
	PlaneGame() : m_font(NULL),m_colour({0,0,0,255}),m_imagefile("NULL"),
        m_log(NULL), m_state(START), m_window(NULL),
        m_renderer(NULL), m_backgroundSurface(NULL), m_backgroundTexture(NULL) {
		m_log = fopen("PlaneGame.log","w");
		if (m_log == NULL) {
			exit(-1);
		}
		
		IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
		TTF_Init();
		
		m_font = TTF_OpenFont("simkai.ttf",16);
	}
	
	// 加载图片
	void loadJPG(char *file) {
		if (m_imagefile != file) {
			m_imagefile = file;
		} else {
			return;
		}
		
		m_backgroundSurface = IMG_LoadJPG_RW(SDL_RWFromFile(file,"rb"));
		if (!m_backgroundSurface) {
			fprintf(m_log, "Cannot open %s!\n",file);
			exit(-5);
		}
		
		m_backgroundTexture = SDL_CreateTextureFromSurface(m_renderer,m_backgroundSurface);
		SDL_FreeSurface(m_backgroundSurface);
		m_backgroundSurface = NULL;
		
		SDL_RenderSetScale(m_renderer,1.5,1.5);
	}
	
	~PlaneGame() {
		if (m_log) {
			fclose(m_log);
		}
		
		if (m_backgroundSurface) {
			SDL_FreeSurface(m_backgroundSurface);
		}
		
		if (m_backgroundTexture) {
			SDL_DestroyTexture(m_backgroundTexture);
		}
		
		if (m_renderer) {
			SDL_DestroyRenderer(m_renderer);
		}
		
		if (m_fontTexture) {
			SDL_DestroyTexture(m_fontTexture);
		}
		
		if (m_window) {
			SDL_DestroyWindow(m_window);
		}
		
		if (m_enemyPlane) {
			delete m_enemyPlane;
		}
		
		if (m_font) {
			TTF_CloseFont(m_font);
		}
		
		if (m_bullet) {
			delete m_bullet;
		}
		
		if (m_player) {
			delete m_player;
		}
		
		IMG_Quit();
		
		TTF_Quit();
		
		SDL_Quit();
	}
	
	// 游戏初始化
	void init() {
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			fprintf(m_log,"Failed to init SDL2! Error: %s\n",SDL_GetError());
			exit(-2);
		}
		
		m_backgroundSurface = IMG_LoadJPG_RW(SDL_RWFromFile("start.jpg","rb"));
		m_w = m_backgroundSurface->w * 1.5;
		m_h = m_backgroundSurface->h * 1.5;
		
		m_window = SDL_CreateWindow("飞机大战",
									SDL_WINDOWPOS_CENTERED,
									SDL_WINDOWPOS_CENTERED,
									m_backgroundSurface->w * 1.5,m_backgroundSurface->h * 1.5,
									SDL_WINDOW_SHOWN
   								);
		
		SDL_FreeSurface(m_backgroundSurface);
		
		if (!m_window) {
			fprintf(m_log,"Cannot create window! Error: %s\n",SDL_GetError());
			exit(-3);
		}
		
		m_renderer = SDL_CreateRenderer(m_window,-1,SDL_RENDERER_ACCELERATED);
		if (!m_renderer) {
			fprintf(m_log,"Cannot create renderer! Error: %s\n",SDL_GetError());
			exit(-4);
		}
		
		this->loadJPG("start.jpg");
	}
	
	// 主循环
	void mainLoop() {
		m_bullet = new Bullet(m_renderer);
		m_player = new Player(m_renderer);
		m_enemyPlane = new EnemyPlane(m_renderer);
		
		SDL_Event event;
		uint32_t begin,end,_time,rate;
		int32_t delay;
		
		while (true) {
			begin = SDL_GetTicks();
			render();
			
			while (SDL_PollEvent(&event)) {
				if (event.type == SDL_KEYDOWN) {
					switch (event.key.keysym.sym) {
						case SDLK_SPACE:
							if (m_state != PLAYING) {
								m_enemyPlane->start();
								this->loadJPG("background.jpg"); // 加载背景图片
								m_state = PLAYING; // 切换状态
							}
							break;
							
						case SDLK_ESCAPE: // 退出游戏
							exit(0);
					}
				}
				
				if (event.type == SDL_QUIT) {
					exit(0);
				}
				
				if (event.type == SDL_MOUSEMOTION) /*鼠标移动*/ {
					m_playerX = event.motion.x;
					m_playerY = event.motion.y;
				}
			}
			
			// 控制帧率
			end = SDL_GetTicks();
			_time = end - begin;
			rate = 1000 / FRAMERATE;
			delay = rate - _time;
			
			if (delay > 0) {
				SDL_Delay(delay);
			}
		}
	}
	
	// 渲染字体
	void renderTTF(char *text) {
		m_fontSurface = TTF_RenderUTF8_Blended(m_font,text,m_colour);
		int w,h;
		w = m_fontSurface->w;
		h = m_fontSurface->h;
		
		m_fontTexture = SDL_CreateTextureFromSurface(m_renderer,m_fontSurface);
		if (!m_fontTexture) {
			fprintf(m_log,"Cannot create font texture!\n");
			exit(-1);
		}
		SDL_FreeSurface(m_fontSurface);
		m_fontSurface = NULL;
		
		SDL_Rect srcrect = {0,0,w,h};
		SDL_RenderCopy(m_renderer,m_fontTexture,NULL,&srcrect);
	}
	
	// 渲染
	void render() {
		SDL_SetRenderDrawColor(m_renderer,0xFF,0xFF,0xFF,0xFF);
		SDL_RenderClear(m_renderer);
		
		static int score = 0; // 分数
		
		switch (m_state) {
			case START:
				SDL_RenderCopy(m_renderer,m_backgroundTexture,NULL,NULL);
				break;
			case END:
				this->loadJPG("end.jpg");
				SDL_RenderCopy(m_renderer,m_backgroundTexture,NULL,NULL);
				
				// 重置游戏状态
				delete m_player;
				delete m_enemyPlane;
				delete m_bullet;
				m_bullet = new Bullet(m_renderer);
				m_player = new Player(m_renderer);
				m_enemyPlane = new EnemyPlane(m_renderer);
				break;
			case PLAYING:
				SDL_RenderCopy(m_renderer,m_backgroundTexture,NULL,NULL);
				
				m_player->render(m_playerX,m_playerY);
				m_enemyPlane->render(m_renderer,m_w);
				m_bullet->render(m_renderer,m_playerX,m_playerY,m_player,score);
				
				char text[1024];
				sprintf(text,"分数：%d",score);
				
				this->renderTTF(text);
				
				// 碰撞检测
				for (std::vector<SDL_Rect>::iterator i = m_enemyPlane->m_position.begin(); i != m_enemyPlane->m_position.end();) {
					// 出界检测
					if (i->y >= this->m_h) {
						if (i != m_enemyPlane->m_position.end()) {
							i = m_enemyPlane->m_position.erase(i);
						} else {
							m_enemyPlane->m_position.pop_back();
						}
					}
					
					// 检测子弹
					for (std::vector<SDL_Rect>::iterator j = m_bullet->m_position.begin(); j != m_bullet->m_position.end();) {
						// 出界检测
						if (j->y <= 0) {
							if (j != m_bullet->m_position.end()) {
								j = m_bullet->m_position.erase(j);
							} else {
								m_bullet->m_position.pop_back();
							}
						}
						
						if (j->x >= i->x && j->x <= i->x + m_enemyPlane->m_w && j->y >= i->y && j->y <= i->y + m_enemyPlane->m_h) {
							if (j != m_bullet->m_position.end()) {
								j = m_bullet->m_position.erase(j);
							} else {
								m_bullet->m_position.pop_back();
							}
							
							if (i != m_enemyPlane->m_position.end()) {
								i = m_enemyPlane->m_position.erase(i);
							} else {
								m_bullet->m_position.pop_back();
							}
							
							++score;
							
						} else {
							if (j != m_bullet->m_position.end()) {
								++j;
							}
						}
					}
					
					// 检测玩家
					if (i->x >= m_playerX && i->x <= m_playerX + m_player->m_w && i->y >= m_playerY && i->y <= m_playerY + m_player->m_h) {
						score = 0;
						m_state = END;
					}
					
					if (i != m_enemyPlane->m_position.end()) {
						++i;
					}
				}
				
				break;
		}
		
		SDL_RenderPresent(m_renderer);
	}
	
private:
	SDL_Window *m_window;
	SDL_Renderer *m_renderer;
	SDL_Surface *m_backgroundSurface;
	SDL_Texture *m_backgroundTexture;
	char *m_imagefile; // 正在使用的图片
	
	FILE *m_log; // 日志
	GameState m_state; // 游戏状态
	
	EnemyPlane *m_enemyPlane; // 敌机
	Player *m_player; // 玩家
	Bullet *m_bullet; // 子弹
	
	int m_w; // 窗口宽度
	int m_h;
	
	Sint32 m_playerX;
	Sint32 m_playerY;
	
	TTF_Font *m_font; // 字体
	SDL_Surface *m_fontSurface;
	SDL_Texture *m_fontTexture;
	
	SDL_Colour m_colour; // 字体颜色
};

#endif