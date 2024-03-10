#include "Renderer.h"

const int PIXEL_SCALE = 100;

SDL_RendererFlip GetFlipType(const Actor& actor);

// draw window
void Renderer::Initialize(const std::string& title)
{
    // FIRST THING, read in rendering json 
    bool processed = ProcessRenderingConfig();

    //set up the camera offset info only if renderConfig processed
    if (processed) {
        if (out_renderingConfig.HasMember("cam_offset_x")) cam.cam_offset_x = out_renderingConfig["cam_offset_x"].GetFloat();
        if (out_renderingConfig.HasMember("cam_offset_y")) cam.cam_offset_y = out_renderingConfig["cam_offset_y"].GetFloat();
        if (out_renderingConfig.HasMember("zoom_factor")) zoomFactor = out_renderingConfig["zoom_factor"].GetDouble();
    }

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

/*bool Renderer::StartFrame(std::vector<std::string>& introImages, int& index)
{
    // Check Events
    SDL_Event nextEvent;
    while (Helper::SDL_PollEvent498(&nextEvent)) {
        if (nextEvent.type == SDL_QUIT) {
            SDL_SetRenderDrawColor(renderer, r, g, b, 255);
            if (introImages.empty())
                SDL_RenderClear(renderer);
            // render all actors
            //for (Actor* actor : actors) {
            //    RenderActor(*actor, { 0, 0 });
            //}
            //EndFrame();
            //exit(0);
            return false;
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
    return true;
}*/

//void Renderer::EndFrame()
//{
//    // show render to user
//    Helper::SDL_RenderPresent498(renderer);
//}

void Renderer::RenderImage(const std::string& imageName)
{
    // TODO dont load texture on every frame
    // if texture not alrdy loaded
    if (textures.find(imageName) == textures.end()) {
        //load it
        SDL_Texture* img = IMG_LoadTexture(renderer, ("resources/images/" + imageName + ".png").c_str());
        textures[imageName] = img;
    }

    /*if (img == nullptr) {
        // Output the SDL error to the console or handle it as needed
        std::cerr << "Unable to load texture. SDL_Error: " << SDL_GetError() << std::endl;
        return; // Exit the function if the texture couldn't be loaded
    }*/

    // get img w and h
    int w, h;
    SDL_QueryTexture(textures[imageName], NULL, NULL, &w, &h);
    SDL_Rect destination_rect = { 0, 0, winWidth, winHeight };
    //SDL_Point pivot_point = { static_cast<int>((w * 0.5)), h };
    SDL_RenderCopy(
        renderer,
        textures[imageName],
        NULL,
        &destination_rect
    );
}

void Renderer::RenderText(TTF_Font* font, const std::string& text, int font_size, SDL_Color font_color, int numTextsShown, int index)
{
    
    //  All text will render at(25, height - 50) where  height is the window height.

    // Render the text to a surface
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), font_color);

    // Create a texture from the surface
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    int x = 0;
    int y = 0;
    TTF_SizeText(font, text.c_str(), &x, &y);

    // Set the rendering space and render to screen
    SDL_Rect renderQuad = { 25, winHeight - 50 - (numTextsShown - 1 - index) * 50, surface->w, surface->h };
    //SDL_Point pivot_point = { x, y };
    SDL_RenderCopy(
        renderer,
        texture,
        NULL,
        &renderQuad
    );
}

void Renderer::RenderActors(std::vector<Actor*> actors, Actor* player) {
    SDL_RenderSetScale(renderer, zoomFactor, zoomFactor);
    if (player == nullptr) {
        for (Actor* actor : actors) {
            RenderActor(*actor, { 0, 0 }); // if no player present, camera set at 0,0
        }
    }
    else {
        for (Actor* actor : actors) {
            RenderActor(*actor, player->position);
        }
    }
}

void Renderer::RenderActor(const Actor& actor, glm::vec2 playerPosition)
{
    // first, texture must exist
    if (!actor.view_image.empty()) {
        // if texture not alrdy loaded
        if (textures.find(actor.view_image) == textures.end()) {
            //load it
            SDL_Texture* img = IMG_LoadTexture(renderer, ("resources/images/" + actor.view_image + ".png").c_str());
            textures[actor.view_image] = img;
        }

        //now that the texture is loaded. render it. 

        /*if (img == nullptr) {
            // Output the SDL error to the console or handle it as needed
            std::cerr << "Unable to load texture. SDL_Error: " << SDL_GetError() << std::endl;
            return; // Exit the function if the texture couldn't be loaded
        }*/

        // Get img width (w) and height (h)
        int w, h;
        SDL_QueryTexture(textures[actor.view_image], NULL, NULL, &w, &h);

        // get pivot offset
        double pivotX = std::round(w * 0.5);
        double pivotY = std::round(h * 0.5);
        if (actor.pivot_offsetX.has_value()) {
            pivotX = actor.pivot_offsetX.value();
        }
        if (actor.pivot_offsetY.has_value()) {
            pivotY = actor.pivot_offsetY.value();
        }

        // Calculate the actor's position relative to the playerPosition, such that the player is always centered
        float relativeXPos = std::round(actor.position.x - playerPosition.x);
        float relativeYPos = std::round(actor.position.y - playerPosition.y);

        SDL_Point pivotSDLPoint;
        pivotSDLPoint.x = std::round(pivotX * std::abs(actor.scale.x));
        pivotSDLPoint.y = std::round(pivotY * std::abs(actor.scale.y));

        SDL_Rect dstRect;
        dstRect.x = std::round(relativeXPos * PIXEL_SCALE + winWidth * 0.5f - pivotSDLPoint.x - cam.cam_offset_x * PIXEL_SCALE) / zoomFactor;
        dstRect.y = std::round(relativeYPos * PIXEL_SCALE + winHeight * 0.5f - pivotSDLPoint.y - cam.cam_offset_y * PIXEL_SCALE) / zoomFactor;
        dstRect.w = w * std::abs(actor.scale.x);
        dstRect.h = h * std::abs(actor.scale.y);

        // Render the texture with the specified rotation and pivot point, use rendercopyex498
        Helper::SDL_RenderCopyEx498(
            actor.actorID,
            actor.actor_name,
            renderer,
            textures[actor.view_image],
            NULL,
            &dstRect,
            actor.rotation, // rotation angle
            &pivotSDLPoint,
            GetFlipType(actor)
        );
    }
}

void Renderer::RenderHUD(const std::string& hp_image, TTF_Font* font, int health, int score)
{
    SDL_RenderSetScale(renderer, 1, 1);

    // RENDER HP IMAGES
    SDL_Texture* img = IMG_LoadTexture(renderer, ("resources/images/" + hp_image + ".png").c_str());

    // get img w and h
    int w, h;
    SDL_QueryTexture(img, NULL, NULL, &w, &h);
    

    // RENDER SCORE

    // Render the text to a surface
    SDL_Surface* surface = TTF_RenderText_Solid(font, ("score : " + std::to_string(score)).c_str(), SDL_Color{255, 255, 255, 255});

    // Create a texture from the surface
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    for (int i = 0; i < health; i++) {
        SDL_Rect destination_rect = { 5 + i * (w + 5), 25, w, h };
        SDL_RenderCopy(
            renderer,
            img,
            NULL,
            &destination_rect
        );
    }

    // Set the rendering space and render to screen
    SDL_Rect renderQuad = { 5, 5, surface->w, surface->h };
    SDL_RenderCopy(
        renderer,
        texture,
        NULL,
        &renderQuad
    );
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
