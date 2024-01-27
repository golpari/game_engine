#include <iostream>
#include "MapHelper.h"

bool CheckBlocking(glm::ivec2 nextPosition) {
    if (hardcoded_map[nextPosition.x][nextPosition.y] == 'b')
        return true;
    for (Actor actor : hardcoded_actors) {
        if (actor.position == nextPosition && actor.blocking)
            return true;
    }
    return false;
}

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
        if (CheckBlocking(glm::ivec2(actor.position.y - 1, actor.position.x))) //dont do anything if b (blocking) wall is there
            return;
        actor.position.y--;
    }
    else if (movement == "e") {
        if (CheckBlocking(glm::ivec2(actor.position.y, actor.position.x + 1))) //dont do anything if b (blocking) wall is there
            return;
        actor.position.x++;
    }
    else if (movement == "s") {
        if (CheckBlocking(glm::ivec2(actor.position.y + 1, actor.position.x))) //dont do anything if b (blocking) wall is there
            return;
        actor.position.y++;
    }
    else if (movement == "w") {
        if (CheckBlocking(glm::ivec2(actor.position.y, actor.position.x - 1))) //dont do anything if b (blocking) wall is there
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
    Actor actorToPrint;
    for (int y = startY; y <= endY; ++y) {
        for (int x = startX; x <= endX; ++x) {
            // print each character of map, but only when an actor isnt already in that spot
            for (Actor actor : hardcoded_actors) {
                if (actor.position == glm::ivec2{ x, y }) {
                    actorPresent = true;
                    actorToPrint = actor;
                }
            }
            if (actorPresent)
                std::cout << actorToPrint.view;
            else if (!actorPresent)
                std::cout << hardcoded_map[y][x];

            actorPresent = false;
        }
        std::cout << std::endl;
    }

    return;
}

void PrintDialogue(glm::ivec2 playerPosition) {
    //access surrounding 8 slots
    const int diffX[8] = { -1, -1, -1, 0, 1, 1, 1, 0 };
    const int diffY[8] = { -1, 0, 1, 1, 1, 0, -1, -1 };

    int adjacentX;
    int adjacentY;

    //loop through possible actors
    for (Actor actor : hardcoded_actors) {

        //print contact dialogue if relevant
        if (hardcoded_map[playerPosition.y][playerPosition.x] == actor.view) {
            std::cout << actor.contact_dialogue << std::endl;
        }

        // loop through adjacent actors
        for (int i = 0; i < 8; i++) {
            glm::ivec2 adjacent{ playerPosition.x + diffX[i],playerPosition.y + diffY[i] };
            
            //print nearby dialogue if relevant
            if (adjacent == actor.position) {
                std::cout << actor.nearby_dialogue << std::endl;
            }
        }

        
    }

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

        // check + print nearby & contact dialogue
        PrintDialogue(hardcoded_actors.back().position);

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

