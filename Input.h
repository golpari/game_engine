#ifndef INPUT_H
#define INPUT_H

#if __APPLE__
	#include "SDL2/SDL.h"
#else
	#include "SDL.h"
#endif

#include <unordered_map>
#include <vector>

enum INPUT_STATE { INPUT_STATE_UP, INPUT_STATE_JUST_BECAME_DOWN, INPUT_STATE_DOWN, INPUT_STATE_JUST_BECAME_UP };

class Input
{
public:
	static void Init() // Call before main loop begins.
	{
		// TODO add all relevant keys to the map
		//sdl scancodes go 0, then 4-?? but you can loop through them like this
		for (int keyNum = SDL_SCANCODE_UNKNOWN; keyNum < SDL_NUM_SCANCODES; keyNum++) {
			keyboard_states[static_cast<SDL_Scancode>(keyNum)] = INPUT_STATE_UP; //all keys start up/unpressed
		}
	}

	static void ProcessEvent(const SDL_Event& e) // Call every frame at start of event loop.
	{
		// check if the current event is a key up or key down event
		if (e.type == SDL_KEYDOWN) {
			just_became_down_scancodes.push_back(e.key.keysym.scancode);
		}
		else if (e.type == SDL_KEYUP) {
			just_became_up_scancodes.push_back(e.key.keysym.scancode);
		}

		//update the keyboard states once per frame
		Update();
	}

	static void Update() {

		// update all the values for continued keyPresses
		for (auto& it : keyboard_states) {

			// if last frame had the keyval pair as just-down, then make it still-down
			if (it.second == INPUT_STATE_JUST_BECAME_DOWN) {
				it.second = INPUT_STATE_DOWN;
			}
			// if last frame had the keyval pair as just-up, then make it still-up
			else if (it.second == INPUT_STATE_JUST_BECAME_UP) {
				it.second = INPUT_STATE_UP;
			}
		}

		// now update the values in the map for just-downs and just-ups
		for (SDL_Scancode key : just_became_down_scancodes) {
			// add key to the map with a just-down scancode
			//if (keyboard_states.find(key) == keyboard_states.end()) { //dont think you need this since [] will add the item or update it
				keyboard_states[key] = INPUT_STATE_JUST_BECAME_DOWN;
			//}
		}
		for (SDL_Scancode key : just_became_up_scancodes) {
			// add key to the map with a just-up scancode
			//if (keyboard_states.find(key) == keyboard_states.end()) { //dont think you need this since [] will add the item or update it
			keyboard_states[key] = INPUT_STATE_JUST_BECAME_UP;
			//}
		}

		// empty these lists for use on the next frame
		just_became_down_scancodes.clear();
		just_became_up_scancodes.clear();
	}

	static bool GetKey(SDL_Scancode keycode) {
		if (keyboard_states[keycode] == INPUT_STATE_DOWN || keyboard_states[keycode] == INPUT_STATE_JUST_BECAME_DOWN) return true;
		return false;
	}
	static bool GetKeyDown(SDL_Scancode keycode) {
		if (keyboard_states[keycode] == INPUT_STATE_JUST_BECAME_DOWN) return true;
		return false;
	}
	static bool GetKeyUp(SDL_Scancode keycode) {
		if (keyboard_states[keycode] == INPUT_STATE_JUST_BECAME_UP) return true;
		return false;
	}

private:
	static inline std::unordered_map<SDL_Scancode, INPUT_STATE> keyboard_states;
	static inline std::vector<SDL_Scancode> just_became_down_scancodes;
	static inline std::vector<SDL_Scancode> just_became_up_scancodes;
};

#endif
