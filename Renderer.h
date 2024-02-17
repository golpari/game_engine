#ifndef RENDERER_H
#define RENDERER_H

#include <string>
#include <unordered_map>

#include <SDL_ttf.h>

#include "Actor.h"
#include "glm/glm.hpp"
#include "Helper.h"
#include "EngineUtils.h"
#include "Game.h"

// A class to store all our rendering
class Renderer
{
    std::unordered_map<std::string, SDL_Texture*> textures;
    rapidjson::Document out_renderingConfig;
    
    int r = 255;
    int g = 255;
    int b = 255;

    int winWidth = 640;
    int winHeight = 360;

public:
    SDL_Renderer* renderer = nullptr;

    void Initialize(const std::string& title);

    void StartFrame(std::vector<std::string> &introImages, int& index, std::vector<Actor*>& actors);
    void EndFrame();
    void Cleanup();
    void RenderImage(const std::string& imageName);
    void RenderText(TTF_Font* font, const std::string& text, int font_size, SDL_Color font_color, int x, int y);
    void RenderActor(const Actor& actor, glm::vec2 playerPosition);
    void RenderHUD(const std::string& hp_image, TTF_Font* font, int health, int score);

private:
    uint64_t SetCameraResolution(bool renderConfigProcessed);
    bool ProcessRenderingConfig();
};

#endif
