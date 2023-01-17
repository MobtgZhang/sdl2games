# ifndef SPRITE_HEADERS_H
# define SPRITE_HEADERS_H
# include<SDL2/SDL.h>
# include<SDL2/SDL_image.h>
# include<string>

class Sprite{
public:
    Sprite();
    ~Sprite();

    void drawSprite(SDL_Renderer* renderer);
    void setColor(int r,int g, int b,int a);
    void setColor(SDL_Color color);

    void setPosition(SDL_FPoint position);
    void setPosition(const float &x,const float &y);

    void setScale(const SDL_FPoint scale);
    void setScale(const float & x,const float & y);

    void setTexture(SDL_Texture* texture);
    void setTextureRect(SDL_Rect rectTxt);
    void setTextureRect(const int & x,const int & y,const int & w,const int & h);
    void setTextureRect(const float & x,const float & y,const float & w,const float & h);

    void setOrigin(const SDL_FPoint origin);
    void setOrigin(const float &x,const float &y);

    void setRotation(float angel);

    SDL_FPoint getPosition(){return this->position;}
    SDL_FPoint getOrigin(){return this->origin;}
    SDL_FPoint getScale(){return this->scale;}
    SDL_Color getColor(){return this->color;}

    SDL_Texture* getTexture(){return this->texture;}
    SDL_Rect getTextureRect(){return this->rectTxt;}
    SDL_FRect getRect();

    void movePosition(SDL_FPoint move);
    void movePosition(float x,float y);
    void movePosition(int x,int y);
    
private:
    void init();
    float angel;
    SDL_FPoint position;
    SDL_FPoint scale;
    SDL_FPoint origin;
    SDL_Texture* texture;
    SDL_Rect rectTxt;
    SDL_Color color;
};

# endif
