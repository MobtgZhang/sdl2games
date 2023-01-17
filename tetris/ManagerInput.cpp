# include"ManagerInput.h"
ManagerInput::ManagerInput():mouseCoords({0,0}){
    this->exit = false;
}

void ManagerInput::Destroy(){
    this->keyMap.clear();
    this->previousMap.clear();
}

ManagerInput::~ManagerInput(){
    this->Destroy();
}

void ManagerInput::update(SDL_Event event){
    for(auto& it: keyMap){
        this->keyMap[it.first] = it.second;
    }
    this->wheel_x = false;
    this->wheel_y = false;

    while(SDL_PollEvent(&event)){
        switch (event.type){
        case SDL_QUIT:
            this->exit = true;
            break;
        case SDL_KEYDOWN:
            this->pressKey(event.key.keysym.sym);
            break;
        case SDL_KEYUP:
            this->releaseKey(event.key.keysym.sym);
            break;
        case SDL_MOUSEMOTION:
            this->setMouseCoords(event.motion.x,event.motion.y);
            break;
        case SDL_MOUSEWHEEL:
            this->wheelEvent(event.wheel.x,event.wheel.y);
            break;
        case SDL_MOUSEBUTTONDOWN:
            this->pressKey(event.button.button);
            break;
        case SDL_MOUSEBUTTONUP:
            this->releaseKey(event.button.button);
            break;
        case SDL_WINDOWEVENT:
            if(event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED){}
            if(event.window.event == SDL_WINDOWEVENT_LEAVE){}
            break;
        default:
            break;
        }
    }
}

void ManagerInput::pressKey(unsigned int keyID){
    this->keyMap[keyID] = true;
}

void ManagerInput::releaseKey(unsigned int keyID){
    this->keyMap[keyID] = false;
}

void ManagerInput::setMouseCoords(int x,int y){
    this->mouseCoords.x = x;
    this->mouseCoords.y = y;
}

bool ManagerInput::isKeyDown(unsigned int keyID){
    auto it = this->keyMap.find(keyID);
    // 这里有点问题
    return it != keyMap.end() ? it->second :false;
}

bool ManagerInput::isKeyPressed(unsigned int keyID){
    if(this->isKeyPressed(keyID) && (!this->wasKeyDown(keyID))){return true;}
    else return false;
}

bool ManagerInput::wasKeyDown(unsigned int keyID){
    
}
