#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <iostream>
#include <inttypes.h>

#include "CPURender/RasterizerGeometry.hpp"
#include "CPURender/Rasterizer.hpp"
#include "Camera.hpp"
#include "ResourceManager.hpp"
#include "tiny_gltf.h"
#include "Model.hpp"
#include "ModelLoader.hpp"



int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    int width = 640;
    int height = 480;
    int64_t windowFlags = SDL_WINDOW_INPUT_FOCUS;
    SDL_Window* window = SDL_CreateWindow("Rasten", width, height, windowFlags);
    
    if (window == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    ResourceManager rm;
    ResourceManager::ModelID id = rm.loadModel("/Users/mae/workspace/graphics-projects/rasten/demo-scene/assets/Triangle.gltf");
    tinygltf::Model& gltfModel = rm.getModel(id);
    ModelLoader modelLoader;
    Model model = modelLoader.loadFromGltf(gltfModel);

    Rasterizer cpuRenderer;
    Color colorYellow = {255, 255, 0, 255};
    SDL_Renderer* sdlRenderer = SDL_CreateRenderer(window, nullptr);
    bool success = cpuRenderer.createCanvas(sdlRenderer, window, colorYellow, width, height);
    if (!success) {
        return 1;
    }

    cpuRenderer.initGeometry();

    // Main loop
    SDL_Event e;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_EVENT_QUIT:
                    quit = true;
                    break;
            }
        }
        Camera camera = Camera();
        camera.processSDLInputEvent(e);
        camera.update();

        cpuRenderer.update();
    }
    rm.forgetModel(id);
    cpuRenderer.destroy();
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}    