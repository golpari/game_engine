#include <iostream>
#include "MapHelper.h"

int main() {

    //test suit 0 
    
    // on launch, print game_start_message
    std::cout << game_start_message << std::endl;

    std::string userInput;
    do {
        // print ma
        // print initial render
        std::cout << initial_render << std::endl;

        int health = 3;
        int score = 0;
        // print player health and score
        std::cout << "health : " << health << ", "
            << "score : " << score << std::endl;

        //prompt user
        std::cout << "Please make a decision..." << std::endl;

        //explain user options
        std::cout << "Your options are \"n\", \"e\", \"s\", \"w\", \"quit\"" << std::endl;
    } while (std::cin >> userInput && userInput != "quit");

    std::cout << game_over_bad_message;
  
    return 0; 
}

// given some movement command, update the relevant actor
void UpdateActorPosition(Actor actor, char movement) {
    return;
}


void PrintCameraView(glm::ivec2 playerPosition) {

    return;
}
