#include <iostream>
#include <unordered_map>
#include <filesystem>

#include <SDL.h>
#include <SDL_events.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "Game.h"
#include "ActorTemplate.h"
#include "rapidjson/document.h"

#include "Helper.h"
#include "AudioHelper.h"

int guuid = 0;
std::unordered_map<std::string, ActorTemplate*> templates; // keyed by template name

int main(int argc, char* argv[]) {
    Game gameManager;
    gameManager.GameStart();

    gameManager.RunScene();

    std::cout << gameManager.GameEnd(false);
    return 0;
}

int main() {   
    Game gameManager;
    gameManager.GameStart();

    gameManager.RunScene();

    std::cout << gameManager.GameEnd(false);
  
    return 0; 
}

