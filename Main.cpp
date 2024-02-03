#include <iostream>
#include <sstream>

#include "Game.h"

std::stringstream ss;
bool firstRun = true;
int health = 3;
int score = 0;


std::string CheckDialogue(std::string& dialogue, bool& scoredUpped) {
	if (dialogue.find("game over") != std::string::npos)
		return Game::GameEnd(false);
	if (dialogue.find("health down") != std::string::npos)
		health--;
	if (dialogue.find("score up") != std::string::npos && !scoredUpped) {
		score++;
		scoredUpped = true;
	}
	if (dialogue.find("you win") != std::string::npos)
		return Game::GameEnd(true);
	if (health <= 0)
		return Game::GameEnd(false);
	return "";
}

std::string PrintDialogue(Scene scene) {
	//access surrounding 8 slots
	const int diffX[8] = { -1, -1, -1, 0, 1, 1, 1, 0 };
	const int diffY[8] = { -1, 0, 1, 1, 1, 0, -1, -1 };

	int adjacentX;
	int adjacentY;
	std::string endgameString;
	//loop through possible actors
	for (Actor* actor : scene.actors) {
		//print contact dialogue if relevant
		if (scene.player->position == actor->position && actor->contact_dialogue != "") {
			std::cout << actor->contact_dialogue << '\n';
			endgameString = CheckDialogue(actor->contact_dialogue, actor->scoredUpped);
		}

		// loop through adjacent actors
		for (int i = 0; i < 8; i++) {
			glm::ivec2 adjacent{ scene.player->position.x + diffX[i], scene.player->position.y + diffY[i] };

			//print nearby dialogue if relevant
			if (adjacent == actor->position && actor->nearby_dialogue != "") {
				std::cout << actor->nearby_dialogue << '\n';
				endgameString = CheckDialogue(actor->nearby_dialogue, actor->scoredUpped);
			}
		}
	}
	return endgameString;
}

int main() {    
    // on launch, check for resources directory
    Game gameManager;
    Game::GameStart();
    Scene initial = gameManager.LoadInitialScene();

    std::string userInput;
    do {
        //update player position based on the movement
        if (!firstRun) initial.MoveActors();
        firstRun = false;
        initial.MovePlayer(userInput);
        

        // print map
        initial.RenderScene();

        // check + print nearby & contact dialogue & eng game if needed
        std::string endgame = PrintDialogue(initial);

        // print player health and score
        ss << "health : " << health << ", "
            << "score : " << score << '\n';

        //if relevant, print relevant game end string  and return
        if (endgame != "") {
            ss << endgame;
            std::cout << ss.str();
            ss.clear();
            ss.str("");
            return 0;
        }

        //prompt user
        ss << "Please make a decision..." << '\n';

        //explain user options
        ss << "Your options are \"n\", \"e\", \"s\", \"w\", \"quit\"" << '\n';

        std::cout << ss.str();
        ss.clear();
        ss.str("");
    } while (std::cin >> userInput && userInput != "quit");

    std::cout << Game::GameEnd(false);
  
    return 0; 
}

