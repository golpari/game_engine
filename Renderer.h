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

public:
    void Initialize(const std::string& title);

    void StartFrame();
    void EndFrame();
    void Cleanup();
    void RenderImage(const std::string& imageName);
    void RenderText(TTF_Font* font, const std::string& text, int x, int y);
    void RenderActor(const Actor& actor, glm::vec2 playerPosition);
    void RenderHUD(const std::string& hp_image, TTF_Font* font, int health, int score);

private:
    uint64_t GetCameraResolution();
    void ProcessRenderingConfig();
};

#endif
