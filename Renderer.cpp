#include "Renderer.h"

const int PIXEL_SCALE = 100;
SDL_RendererFlip GetFlipType(const Actor& actor);

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
    SetCameraResolution(processed);

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

void Renderer::StartFrame(std::vector<std::string> &introImages, int& index, std::vector<Actor*>& actors)
{
    // Check Events
    SDL_Event nextEvent;
    while (Helper::SDL_PollEvent498(&nextEvent)) {
        if (nextEvent.type == SDL_QUIT) {
            SDL_SetRenderDrawColor(renderer, r, g, b, 255);
            if (introImages.empty())
                SDL_RenderClear(renderer);
            // render all actors
            for (Actor* actor : actors) {
                RenderActor(*actor, { 0, 0 });
            }
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
}

void Renderer::EndFrame()
{
    // show render to user
    Helper::SDL_RenderPresent498(renderer);
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

void Renderer::RenderText(TTF_Font* font, const std::string& text, int font_size, SDL_Color font_color, int x, int y)
{
    
    //  All text will render at(25, height - 50) where  height is the window height.

    // Render the text to a surface
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), font_color);

    // Create a texture from the surface
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    // Set the rendering space and render to screen
    SDL_Rect renderQuad = { 25, winHeight - 50, surface->w, surface->h };
    SDL_Point pivot_point = { x, y };
    SDL_RenderCopyEx(
        renderer,
        texture,
        NULL,
        &renderQuad,
        0, // rotation angle
        &pivot_point,
        SDL_FLIP_NONE
    );
}

void Renderer::RenderActor(const Actor& actor, glm::vec2 playerPosition)
{
    if (!actor.view_image.empty()) {
        SDL_Texture* img = IMG_LoadTexture(renderer, ("resources/images/" + actor.view_image + ".png").c_str());

        /*if (img == nullptr) {
            // Output the SDL error to the console or handle it as needed
            std::cerr << "Unable to load texture. SDL_Error: " << SDL_GetError() << std::endl;
            return; // Exit the function if the texture couldn't be loaded
        }*/

        // Get img width (w) and height (h)
        int w, h;
        SDL_QueryTexture(img, NULL, NULL, &w, &h);

        // Assuming winWidth and winHeight are the dimensions of your window, and are defined elsewhere in your Renderer class
        // Calculate the center of the window
        int centerX = std::round(winWidth / 2);
        int centerY = std::round(winHeight / 2);

        double pivotX = std::round(w * 0.5);
        double pivotY = std::round(h * 0.5);
        if (actor.pivot_offsetX.has_value()) {
            pivotX = actor.pivot_offsetX.value();
        }
        if (actor.pivot_offsetY.has_value()) {
            pivotY = actor.pivot_offsetY.value();
        }

        // Calculate the actor's position relative to the playerPosition, such that the player is always centered
        float relativeXPos = std::round(actor.position.x - playerPosition.x) * PIXEL_SCALE;
        float relativeYPos = std::round(actor.position.y - playerPosition.y) * PIXEL_SCALE;

        // Convert this position so that an object at the playerPosition would be at the center of the screen
        float renderX = static_cast<int>(centerX + relativeXPos - pivotX);
        float renderY = static_cast<int>(centerY + relativeYPos - pivotY);

        // Calculate the scaling factors
        //float scaleX = std::abs(actor.scale.x);
        //float scaleY = std::abs(actor.scale.y);

        //// Calculate the new width and height after applying the scaling factors
        //int scaledW = std::round(w * scaleX);
        //int scaledH = std::round(h * scaleY);

        //// Calculate the change in width and height to adjust the actor's position accordingly
        //int deltaW = scaledW - w;
        //int deltaH = scaledH - h;

        // Calculate the center point of the texture for rotation
        SDL_Point pivot_point = { static_cast<int>(w * 0.5), static_cast<int>(h * 0.5)};

        // Create the destination rectangle at the position where the actor should be drawn
        SDL_Rect destination_rect = { renderX, renderY, std::round(w * std::abs(actor.scale.x)), std::round(h * std::abs(actor.scale.y)) };

        // Render the texture with the specified rotation and pivot point
        SDL_RenderCopyEx(
            renderer,
            img,
            NULL,
            &destination_rect,
            actor.rotation, // rotation angle
            &pivot_point,
            GetFlipType(actor)
        );
    }
}

uint64_t Renderer::SetCameraResolution(bool renderConfigProcessed) {
    //default values (diff for HW5, used to be 9x13)

    if (renderConfigProcessed) {
        if (out_renderingConfig.HasMember("x_resolution")) {
            winWidth = out_renderingConfig["x_resolution"].GetUint();
        }
        if (out_renderingConfig.HasMember("y_resolution")) {
            winHeight = out_renderingConfig["y_resolution"].GetUint();
        }
    }

    return EngineUtils::combine(winWidth, winHeight);
}

bool Renderer::ProcessRenderingConfig() {
    if (EngineUtils::CheckPathExists("resources/rendering.config", false)) {
        EngineUtils::ReadJsonFile("resources/rendering.config", out_renderingConfig);
        return true;
    }
    return false;
}

//helper I wrote with help from Chat
/* "This function starts with SDL_FLIP_NONE and uses bitwise OR (|) 
to add SDL_FLIP_HORIZONTAL or SDL_FLIP_VERTICAL to the flip variable 
based on the conditions of actor.scale.x and actor.scale.y. 
If both scales are negative, both flip flags will be set in the flip 
variable, which SDL's rendering functions will interpret as flipping 
both horizontally and vertically." -- ChatGPT explanation */
SDL_RendererFlip GetFlipType(const Actor& actor) {
    SDL_RendererFlip flip = SDL_FLIP_NONE;

    if (actor.scale.x < 0)
        flip = (SDL_RendererFlip)(flip | SDL_FLIP_HORIZONTAL);
    if (actor.scale.y < 0)
        flip = (SDL_RendererFlip)(flip | SDL_FLIP_VERTICAL);

    return flip;
}
