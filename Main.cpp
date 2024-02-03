#include <iostream>

#include "Game.h"

int main() {   
    Game gameManager;
    gameManager.GameStart();

    std::string userInput;
    do {

        gameManager.RunScene(*gameManager.currentScene, userInput);

    } while (std::cin >> userInput && userInput != "quit");

    std::cout << Game::GameEnd(false);
  
    return 0; 
}

