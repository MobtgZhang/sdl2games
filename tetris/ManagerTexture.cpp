# include "ManagerTexture.h"

ManagerTexture::ManagerTexture(){

}

void ManagerTexture::setRender(SDL_Renderer* renderer){
    this->renderer = renderer;
}

void ManagerTexture::Destroy(){
    for(std::map<std::string,SDL_Texture*>::iterator ptr=this->texture_map.begin();
        ptr!=this->texture_map.end();ptr++){
            SDL_DestroyTexture(ptr->second);
    }
    this->texture_map.clear();
}

ManagerTexture::~ManagerTexture(){
    this->Destroy();
}

SDL_Texture* ManagerTexture::getTexture(std::string name){
    std::map<std::string,SDL_Texture*>::iterator mit = texture_map.find(name);
    if(mit == texture_map.end()){
        SDL_Texture* newTexture = this->loadTexture(name);

        texture_map.insert(std::make_pair(name,newTexture));

        return newTexture;
    }
    return mit->second;
}

SDL_Texture* ManagerTexture::loadTexture(std::string name){
    SDL_Surface* loadedImage = IMG_Load(name.c_str());

    if(loadedImage==NULL){
        printf("eror load Image.\n");
    }

    SDL_Texture* newTexture = SDL_CreateTextureFromSurface(this->renderer,loadedImage);

    if(newTexture==NULL){
        printf("eror create newTexture.\n");
    }

    SDL_FreeSurface(loadedImage);

    return newTexture;
}

