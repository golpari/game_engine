#include <iostream>
#include <unordered_map>
#include <filesystem>

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <SDL2_mixer/SDL_mixer.h>

#include "Game.h"
#include "ActorTemplate.h"
#include "rapidjson/document.h"
#include "Renderer.h"

#include "Helper.h"
#include "AudioHelper.h"

int guuid = 0;
std::unordered_map<std::string, ActorTemplate*> templates; // keyed by template name

int main(int argc, char* argv[]) {
    Game gameManager;
    //std::cout << std::filesystem::current_path();
    gameManager.GameStart();
    gameManager.RunScene();
    return 0;
}
