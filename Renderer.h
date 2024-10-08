#ifndef RENDERER_H
#define RENDERER_H

#if __APPLE__
    #include "SDL2_ttf/SDL_ttf.h"
#else
    #include "SDL_ttf.h"
#endif

#include <string>
#include <unordered_map>

#include "Actor.h"
#include "EngineUtils.h"
#include "glm/glm.hpp"
#include "Helper.h"

struct Camera {
    
    glm::vec2 position = { 0, 0 };

};

class Renderer
{
    std::unordered_map<std::string, SDL_Texture*> textures;
    rapidjson::Document out_renderingConfig;

    int winWidth = 640;
    int winHeight = 360;
    double zoomFactor = 1;
    float camEasefactor = 1;

public:
    int r = 255;
    int g = 255;
    int b = 255;
    bool animateActorsOnMovement;

    SDL_Renderer* renderer = nullptr;
    Camera cam;

    Renderer() {}

    void Initialize(const std::string& title); // second param added so that cam position can be initially set to player pos if player exists

    // returns false if end event triggered
   // bool StartFrame(std::vector<std::string> &introImages, int& index);
   // void EndFrame();
    void Cleanup();
    void RenderImage(const std::string& imageName);
    void RenderText(TTF_Font* font, const std::string& text, int font_size, SDL_Color font_color, int numTextsShown, int index);
    void RenderActors(std::vector<Actor*> actors);
    void RenderActor(const Actor& actor, glm::vec2 playerPosition);
    void RenderHUD(const std::string& hp_image, TTF_Font* font, int health, int score);

private:
    uint64_t SetCameraResolution(bool renderConfigProcessed);
    bool ProcessRenderingConfig();
};

#endif
