#include "Renderer.h"


// draw window
void Renderer::Initialize(const std::string& title)
{
    // FIRST THING, read in rendering json 
    bool processed = ProcessRenderingConfig();

    // tell SDL what you want to do 
    SDL_Init(SDL_INIT_VIDEO);

    // default values
    int winWidth = 640;
    int winHeight = 360;
    EngineUtils::split(GetCameraResolution(processed), winWidth, winHeight);

    // Create the window
    SDL_Window* window = SDL_CreateWindow(
        title.c_str(),                  //window title
        SDL_WINDOWPOS_CENTERED,         // initial x position
        SDL_WINDOWPOS_CENTERED,         // initial y position
        winWidth,                       // width, in pixels
        winHeight,                      // height, in pixels
        SDL_WINDOW_SHOWN                // flags
    );

    if (processed) {
        if (out_renderingConfig.HasMember("clear_color_r") && out_renderingConfig["clear_color_r"] != ""
            && out_renderingConfig.HasMember("clear_color_g") && out_renderingConfig["clear_color_g"] != ""
            && out_renderingConfig.HasMember("clear_color_b") && out_renderingConfig["clear_color_b"] != "") {
            r = out_renderingConfig["clear_color_r"].GetInt();
            g = out_renderingConfig["clear_color_g"].GetInt();
            b = out_renderingConfig["clear_color_b"].GetInt();
        }
    }
    
    
    renderer = Helper::SDL_CreateRenderer498(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);    
}

void Renderer::StartFrame()
{
    // process events aka keep going until there are no more events or a close event is triggered!
    SDL_Event nextEvent;
    while (Helper::SDL_PollEvent498(&nextEvent)) {
        if (nextEvent.type == SDL_QUIT) {
            EndFrame();
            exit(0);
        }
    }   
}

void Renderer::EndFrame()
{
    // always clear framebuffer at beginning of frame
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_RenderClear(renderer);

    // show render to user
    Helper::SDL_RenderPresent498(renderer);

    SDL_Delay(1);
}

void Renderer::RenderImage(const std::string& imageName)
{
}

uint64_t Renderer::GetCameraResolution(bool renderConfigProcessed) {
    //default values (diff for HW5, used to be 9x13)
    int x_res = 640;
    int y_res = 360;

    if (renderConfigProcessed) {
        if (out_renderingConfig.HasMember("x_resolution")) {
            x_res = out_renderingConfig["x_resolution"].GetUint();
        }
        if (out_renderingConfig.HasMember("y_resolution")) {
            y_res = out_renderingConfig["y_resolution"].GetUint();
        }
    }

    return EngineUtils::combine(x_res, y_res);
}

bool Renderer::ProcessRenderingConfig() {
    if (EngineUtils::CheckPathExists("resources/rendering.config", false)) {
        EngineUtils::ReadJsonFile("resources/rendering.config", out_renderingConfig);
        return true;
    }
    return false;
}
