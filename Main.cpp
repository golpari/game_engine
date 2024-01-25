#include <iostream>
#include "MapHelper.h"

int main() {

    //test suit 0 
    
    // on launch, print game_start_message
    std::cout << game_start_message << std::endl;

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

    std::string userInput;
    while (std::cin >> userInput && userInput != "quit") {
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
    }

    std::cout << game_over_bad_message;
  
    return 0; 
}
