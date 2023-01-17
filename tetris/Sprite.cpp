# include"Sprite.h"
Sprite::Sprite(){

}

void Sprite::init(){
    this->origin = {0.0,0.0};
    this->scale = {1.0,1.0};
    this->position = {0,0};
    this->color = {255,255,255,255}; // R,G,B,A
    this->rectTxt.x = 0;
    this->rectTxt.y = 0;
    this->angel = 0;
}

Sprite::~Sprite(){

}

void Sprite::setColor(int r,int g,int b,int a){
    this->color = {
        (Uint8)r,
        (Uint8)g,
        (Uint8)b,
        (Uint8)a        
    };
}

void Sprite::setColor(SDL_Color color){
    this->color = color;
}

void Sprite::setPosition(SDL_FPoint position){
    this->position = position;
}

void Sprite::setPosition(const float&x,const float&y){
    this->position = {x,y};
}

void Sprite::movePosition(SDL_FPoint move){
    this->position.x += move.x;
    this->position.y += move.y;
}

void Sprite::movePosition(float x,float y){
    this->position.x += x;
    this->position.y += y;
}
void Sprite::movePosition(int x,int y){
    this->position.x += x;
    this->position.y += y;
}
    
void Sprite::setScale(SDL_FPoint scale){
    this->scale = scale;
}

void Sprite::setScale(const float&x,const float& y){
    this->scale = {x,y};
}

void Sprite::setTexture(SDL_Texture* texture){
    this->texture = texture;
    SDL_QueryTexture(this->texture,NULL,NULL,&this->rectTxt.w,&this->rectTxt.h);
    this->init();
}

void Sprite::setOrigin(SDL_FPoint origin){
    this->origin = origin;
}

void Sprite::setOrigin(const float &x,const float &y){
    this->origin = {x,y};
}

void Sprite::setRotation(float angel){
    this->angel = angel;
}

void Sprite::drawSprite(SDL_Renderer* renderer){
    SDL_FPoint Centr = this->origin;
    Centr.x *= this->scale.x;
    Centr.y *= this->scale.y;

    SDL_FRect rectWin{
        this->position.x - Centr.x,
        this->position.y - Centr.y,
        this->rectTxt.w * this->scale.x,
        this->rectTxt.h * this->scale.y
    };

    SDL_SetTextureColorMod(this->texture,this->color.r,this->color.g,this->color.b);
    SDL_SetTextureAlphaMod(this->texture,this->color.a);
    SDL_RenderCopyExF(renderer,this->texture,&this->rectTxt,
                &rectWin,this->angel,&Centr,SDL_FLIP_NONE);
    
}

SDL_FRect Sprite::getRect(){
    SDL_FPoint Centr = this->origin;
    Centr.x *= this->scale.x;
    Centr.y *= this->scale.y;

    SDL_Rect rectWin{
        this->position.x - Centr.x,
        this->position.y - Centr.y,
        this->rectTxt.w * this->scale.x,
        this->rectTxt.h * this->scale.y
    };

    return rectWin;
}

void Sprite::setTextureRect(SDL_Rect rectTxt){
    this->rectTxt = rectTxt;
}

void Sprite::setTextureRect(const int&x,const int&y,const int&w,const int&h){
    this->rectTxt = {x,y,w,h};
}

void Sprite::setTextureRect(const float&x,const float&y,const float&w,const float&h){
    this->rectTxt = {(int)x,(int)y,(int)w,(int)h,};
}

void Sprite::movePosition(float x,float y){
    this->position.x += x;
    this->position.y += y;
}

void Sprite::movePosition(int x, int y){
    this->position.x += x;
    this->position.y += y;
}

