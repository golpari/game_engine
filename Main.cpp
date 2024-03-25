#include <iostream>
#include <unordered_map>
#include <filesystem>

#if __APPLE__
    #include "SDL2/SDL_events.h"
    #include "SDL2_image/SDL_image.h"
    #include "SDL2_ttf/SDL_ttf.h"
    #include "SDL2_mixer/SDL_mixer.h" 
#else
    #include "SDL_events.h"
    #include "SDL_image.h"
    #include "SDL_ttf.h"
    #include "SDL_mixer.h"
#endif

#include "Game.h"
#include "ActorTemplate.h"
#include "rapidjson/document.h"
#include "Renderer.h"

#include "Helper.h"
#include "AudioHelper.h"

#include "lua.hpp"
#include "LuaBridge.h"

int guuid = 0;
std::unordered_map<std::string, ActorTemplate*> templates; // keyed by template name

int main(int argc, char* argv[]) {
    Game gameManager;
    //std::cout << std::filesystem::current_path();
    gameManager.GameStart();
    gameManager.RunScene();
    return 0;
}
