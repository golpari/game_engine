#include <iostream>

#include "Game.h"

int guuid = 0;

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

