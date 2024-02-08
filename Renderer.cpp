#include "Renderer.h"


// draw window
void Renderer::Initialize(const std::string& title)
{
    // FIRST THING, read in rendering json 
    bool processed = ProcessRenderingConfig();

    // tell SDL what you want to do 
    SDL_Init(SDL_INIT_VIDEO);

    // default values
    winWidth = 640;
    winHeight = 360;
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

void Renderer::StartFrame(std::vector<std::string> &introImages, int& index)
{
    SDL_Event nextEvent;
    while (Helper::SDL_PollEvent498(&nextEvent)) {
        if (nextEvent.type == SDL_QUIT) {
            SDL_RenderClear(renderer);
            EndFrame();
            exit(0);
        }

        // Mouse event: SDL_MOUSEBUTTONDOWN is for mouse button press
        else if (nextEvent.type == SDL_MOUSEBUTTONDOWN && nextEvent.button.button == SDL_BUTTON_LEFT) {
            // Handle left mouse click
            index++;
        }

        // Keyboard event: SDL_KEYDOWN is for key press
        else if (nextEvent.type == SDL_KEYDOWN) {
            // Check for spacebar or enter key using scancode
            if (nextEvent.key.keysym.scancode == SDL_SCANCODE_SPACE || nextEvent.key.keysym.scancode == SDL_SCANCODE_RETURN) {
                // Handle spacebar or enter key press
                index++;
            }
        }
    }

    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    SDL_RenderClear(renderer);

    // show intro image as directed
    if (introImages.size() > index)
        RenderImage(introImages[index]);
    else
        SDL_RenderClear(renderer);
}

void Renderer::EndFrame()
{
    // show render to user
    Helper::SDL_RenderPresent498(renderer);

    SDL_Delay(1);
}

void Renderer::RenderImage(const std::string& imageName)
{
    SDL_Texture* img = IMG_LoadTexture(renderer, ("resources/images/" + imageName + ".png").c_str());

    /*if (img == nullptr) {
        // Output the SDL error to the console or handle it as needed
        std::cerr << "Unable to load texture. SDL_Error: " << SDL_GetError() << std::endl;
        return; // Exit the function if the texture couldn't be loaded
    }*/

    // get img w and h
    int w, h;
    SDL_QueryTexture(img, NULL, NULL, &w, &h);
    SDL_Rect destination_rect = { 0, 0, winWidth, winHeight };
    SDL_Point pivot_point = { w * 0.5, h };
    SDL_RenderCopyEx(
        renderer,
        img,
        NULL,
        &destination_rect,
        0, // rotation angle
        &pivot_point,
        SDL_FLIP_NONE
    );
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
