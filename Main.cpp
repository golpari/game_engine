#include <iostream>
#include "MapHelper.h"
#include <sstream>

#include "EngineUtils.h"

//std::stringstream ss;
//bool firstRun = true;
//int health = 3;
//int score = 0;
//
//std::string CheckDialogue(std::string& dialogue, bool& scoredUpped) {
//    if (dialogue.find("game over") != std::string::npos)
//        return game_over_bad_message;
//    if (dialogue.find("health down") != std::string::npos)
//        health--;
//    if (dialogue.find("score up") != std::string::npos && !scoredUpped) {
//        score++;
//        scoredUpped = true;
//    }
//    if (dialogue.find("you win") != std::string::npos)
//        return game_over_good_message;
//    if (health <= 0)
//        return game_over_bad_message;
//    return "";
//}
//
//bool CheckBlocking(glm::ivec2 nextPosition) {
//    if (hardcoded_map[nextPosition.y][nextPosition.x] == 'b')
//        return true;
//    for (Actor& actor : hardcoded_actors) {
//        if (actor.position == nextPosition && actor.blocking)
//            return true;
//    }
//    return false;
//}
//
//// given some movement command, update the relevant actor
//void UpdateActorPosition(Actor& actor, std::string& movement) {
//
//    //if not valid movement, do nothing
//    if (movement != "n" &&
//        movement != "e" &&
//        movement != "s" &&
//        movement != "w")
//        return;
//
//    //update position vec
//    else if (movement == "n") {
//        //DEBUG STATEMENT std::cout << "HERE: " << hardcoded_map[actor.position.y - 1][actor.position.x] << std::endl;
//        if (CheckBlocking(glm::ivec2(actor.position.x, actor.position.y - 1))) //dont do anything if b (blocking) wall is there
//            return;
//        actor.position.y--;
//    }
//    else if (movement == "e") {
//        if (CheckBlocking(glm::ivec2(actor.position.x + 1, actor.position.y))) //dont do anything if b (blocking) wall is there
//            return;
//        actor.position.x++;
//    }
//    else if (movement == "s") {
//        if (CheckBlocking(glm::ivec2(actor.position.x, actor.position.y + 1))) //dont do anything if b (blocking) wall is there
//            return;
//        actor.position.y++;
//    }
//    else if (movement == "w") {
//        if (CheckBlocking(glm::ivec2(actor.position.x - 1, actor.position.y))) //dont do anything if b (blocking) wall is there
//            return;
//        actor.position.x--;
//    }
//
//    //DEBUG STATEMENT std::cerr << actor.position.x << ", " << actor.position.y << std::endl;
//
//    return;
//}
//
//void UpdateAllActorPositions() {
//
//    glm::ivec2 nextPosition;
//    //update all actors except for the player (which is the last actor)
//    for (int i = 0; i < hardcoded_actors.size() - 1; i++) {
//        nextPosition = hardcoded_actors[i].position + hardcoded_actors[i].velocity;
//        if (!CheckBlocking(nextPosition))
//            hardcoded_actors[i].position = nextPosition;
//        else
//            hardcoded_actors[i].velocity = -hardcoded_actors[i].velocity;
//    }
//}
//
//void PrintCameraView(glm::ivec2& playerPosition) {
//    // 9x13 view
//
//    // Calculate starting and ending coordinates for the 9x13 grid
//    int startX = playerPosition.x - 6;
//    int endX = playerPosition.x + 6;
//
//    int startY = playerPosition.y - 4;
//    int endY = playerPosition.y + 4;
//
//    bool actorPresent = false;
//    Actor actorToPrint;
//
//    for (int y = startY; y <= endY; ++y) {
//        for (int x = startX; x <= endX; ++x) {
//            actorPresent = false;
//
//            // Check if the current coordinates are within the map bounds
//            if (x >= 0 && x < HARDCODED_MAP_WIDTH && y >= 0 && y < HARDCODED_MAP_HEIGHT) {
//                // Check if there's an actor at the current position
//                for (Actor& actor : hardcoded_actors) {
//                    if (actor.position == glm::ivec2{ x, y }) {
//                        actorPresent = true;
//                        actorToPrint = actor;
//                    }
//                }
//
//                if (actorPresent)
//                    ss << actorToPrint.view;
//                else
//                    ss << hardcoded_map[y][x];
//            }
//            else {
//                // Print an empty space if outside map bounds
//                ss << ' ';
//            }
//        }
//        ss << '\n';
//    }
//
//    return;
//}
//
//
//std::string PrintDialogue(glm::ivec2& playerPosition) {
//    //access surrounding 8 slots
//    const int diffX[8] = { -1, -1, -1, 0, 1, 1, 1, 0 };
//    const int diffY[8] = { -1, 0, 1, 1, 1, 0, -1, -1 };
//
//    int adjacentX;
//    int adjacentY; 
//    std::string endgameString;
//    //loop through possible actors
//    for (Actor& actor : hardcoded_actors) {
//        //print contact dialogue if relevant
//        if (playerPosition == actor.position && actor.contact_dialogue != "") {
//            ss << actor.contact_dialogue << '\n';
//            endgameString = CheckDialogue(actor.contact_dialogue, actor.scoredUpped);
//        }
//
//        // loop through adjacent actors
//        for (int i = 0; i < 8; i++) {
//            glm::ivec2 adjacent{ playerPosition.x + diffX[i],playerPosition.y + diffY[i] };
//            
//            //print nearby dialogue if relevant
//            if (adjacent == actor.position && actor.nearby_dialogue != "") {
//                ss << actor.nearby_dialogue << '\n';
//                endgameString = CheckDialogue(actor.nearby_dialogue, actor.scoredUpped);
//            }
//        }  
//    }
//
//    return endgameString;
//}

int main() {    
    // on launch, check for resources directory
    EngineUtils::GameStart();

    std::string userInput;
    //do {
    //    //update player position based on the movement
    //    if (!firstRun) UpdateAllActorPositions();
    //    firstRun = false;
    //    UpdateActorPosition(hardcoded_actors.back(), userInput);
    //    

    //    // print map (pass in player position)
    //    PrintCameraView(hardcoded_actors.back().position);

    //    // check + print nearby & contact dialogue & eng game if needed
    //    std::string endgame = PrintDialogue(hardcoded_actors.back().position);

    //    // print player health and score
    //    ss << "health : " << health << ", "
    //        << "score : " << score << '\n';

    //    //if relevant, print relevant game end string  and return
    //    if (endgame != "") {
    //        ss << endgame;
    //        std::cout << ss.str();
    //        ss.clear();
    //        ss.str("");
    //        return 0;
    //    }

    //    //prompt user
    //    ss << "Please make a decision..." << '\n';

    //    //explain user options
    //    ss << "Your options are \"n\", \"e\", \"s\", \"w\", \"quit\"" << '\n';

    //    std::cout << ss.str();
    //    ss.clear();
    //    ss.str("");
    //} while (std::cin >> userInput && userInput != "quit");

    //std::cout << game_over_bad_message;
  
    return 0; 
}

