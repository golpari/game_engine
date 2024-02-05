#include <iostream>
#include <unordered_map>
#include <filesystem>

#include "Game.h"
#include "ActorTemplate.h"
#include "rapidjson/document.h"

int guuid = 0;
std::unordered_map<std::string, ActorTemplate*> templates; // keyed by template name

int main() {   
    Game gameManager;
    gameManager.GameStart();

    gameManager.RunScene(*gameManager.currentScene);

    std::cout << gameManager.GameEnd(false);
  
    return 0; 
}

