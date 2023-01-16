# ifndef MANAGERTEXTURE_HEADER_H
# define MANAGERTEXTURE_HEADER_H
# include<SDL2/SDL.h>
# include<SDL2/SDL_image.h>
# include<map>
# include<string>

class ManagerTexture{
public:
    ManagerTexture();
    ~ManagerTexture();

    void Destroy();
    
    void setRender(SDL_Renderer* renderer);

    SDL_Texture* getTexture(std::string name);

private:
    SDL_Renderer* renderer;

    SDL_Texture* loadTexture(std::string name);

    std::map<std::string,SDL_Texture*> texture_map;
};

# endif
