# ifndef MANAGERINPUT_HEADERS_H
# define MANAGERINPUT_HEADERS_H
# include<SDL2/SDL.h>
# include<unordered_map>

class ManagerInput{
public:
    ManagerInput();
    ~ManagerInput();

    int wheel_x;
    int wheel_y;

    void update(SDL_Event event);
    
    void wheelEvent(int x,int y){
        this->wheel_x = x;
        this->wheel_y = y;
    }

    void Destroy();
    void pressKey(unsigned int keyID);
    void releaseKey(unsigned int keyID);

    void setMouseCoords(int x, int y);

    bool isKeyDown(unsigned int keyID);

    bool isKeyPressed(unsigned int keyID);

    SDL_Point getMouseCoords() const {return this->mouseCoords;}

    bool exit;
private:
    bool wasKeyDown(unsigned int keyID);

    std::unordered_map<unsigned int,bool>keyMap;
    std::unordered_map<unsigned int,bool>previousMap;
    SDL_Point mouseCoords;
};

# endif
