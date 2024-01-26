#include <iostream>
#include "MapHelper.h"

// given some movement command, update the relevant actor
void UpdateActorPosition(Actor& actor, std::string movement) {

    //if not valid movement, do nothing
    if (movement != "n" &&
        movement != "e" &&
        movement != "s" &&
        movement != "w")
        return;

    //update position vec
    else if (movement == "n") {
        //DEBUG STATEMENT std::cout << "HERE: " << hardcoded_map[actor.position.y - 1][actor.position.x] << std::endl;
        if (hardcoded_map[actor.position.y - 1][actor.position.x] == 'b') //dont do anything if b (blocking) wall is there
            return;
        actor.position.y--;
    }
    else if (movement == "e") {
        if (hardcoded_map[actor.position.y][actor.position.x + 1] == 'b') //dont do anything if b (blocking) wall is there
            return;
        actor.position.x++;
    }
    else if (movement == "s") {
        if (hardcoded_map[actor.position.y + 1][actor.position.x] == 'b') //dont do anything if b (blocking) wall is there
            return;
        actor.position.y++;
    }
    else if (movement == "w") {
        if (hardcoded_map[actor.position.y][actor.position.x - 1] == 'b') //dont do anything if b (blocking) wall is there
            return;
        actor.position.x--;
    }

    //DEBUG STATEMENT std::cerr << actor.position.x << ", " << actor.position.y << std::endl;

    return;
}


void PrintCameraView(glm::ivec2 playerPosition) {
    //9x13

    // loop through map starting from playerPosition
    int startX = std::max(playerPosition.x - 6, 0);
    int endX = std::min(playerPosition.x + 6, HARDCODED_MAP_WIDTH);

    int startY = std::max(playerPosition.y - 4, 0);
    int endY = std::min(playerPosition.y + 4, HARDCODED_MAP_HEIGHT);

    bool actorPresent = false;
    for (int y = startY; y <= endY; ++y) {
        for (int x = startX; x <= endX; ++x) {
            // print each character of map, but only when an actor isnt already in that spot
            for (Actor actor : hardcoded_actors) {
                if (actor.position == glm::ivec2{ x, y }) {
                    actorPresent = true;
                    std::cout << actor.view;
                }
            }
            if (!actorPresent)
                std::cout << hardcoded_map[y][x];

            actorPresent = false;
        }
        std::cout << std::endl;
    }

    return;
}


int main() {

    //test suit 0 
    
    // on launch, print game_start_message
    std::cout << game_start_message << std::endl;

    std::string userInput;
    do {
        //update player position based on the movement
        UpdateActorPosition(hardcoded_actors.back(), userInput);

        // print map (pass in player position)
        PrintCameraView(hardcoded_actors.back().position);

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

