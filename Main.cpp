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

    std::string userInput;
    do {

        gameManager.RunScene(*gameManager.currentScene, userInput);

    } while (std::cin >> userInput && userInput != "quit");

    std::cout << gameManager.GameEnd(false);
  
    return 0; 
}

