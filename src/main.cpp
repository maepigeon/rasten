#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <iostream>
#include <inttypes.h>

#include "Geom.hpp"
#include "Render.hpp"

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    int width = 640;
    int height = 480;
    int64_t windowFlags = SDL_WINDOW_INPUT_FOCUS;
    SDL_Window* window = SDL_CreateWindow("Rasten", width, height, windowFlags);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);    
    if (window == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }
    if (renderer == nullptr) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    // Define geometry
    int numTriangles = 3;
    Triangle t1 = {{0.0, 0.0}, {0.0, 100.0}, {100.0, 0.0}};
    Triangle t2 = {{100.0, 200.0}, {100.0, 0.0}, {200.0, 50.0}};
    Triangle t3 = {{300.0, 200.0}, {350.0, 400.0}, {350.0, 250.0}};
    Triangle tList[] = {t1, t2, t3};
    glm::ivec2 A, B;
    A = {200.0, 60.0};
    B = {100 , 260};
    std::vector<glm::ivec2> lineABPoints;

    // Fill the window with a solid color
    SDL_Surface* surface = SDL_CreateSurface(width, height, SDL_PIXELFORMAT_RGBA32);
    Color colorYellow = {255, 255, 0, 255};
    setSurfaceColor(surface, width, height, colorYellow);

    // Create a texture that can be sent to the gpu from the (cpu-owned) surface 
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == NULL) {
        SDL_Log("CreateTextureFromSurface failed: %s", SDL_GetError());
        exit(1);
    }

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
        lineABPoints.clear();
        SDL_RenderClear(renderer);
        Color colorRed= {255, 0, 0, 255};
        Color colorBlue= {0, 0, 255, 255};
        //Clear the renderer
        setSurfaceColor(surface, width, height, colorYellow);
        //Render Lines
        bresenhamLine(lineABPoints, A.x, A.y, B.x, B.y); 
        SDL_FPoint sdlBresenhamPoints[lineABPoints.size()];
        for (int i = 0; i < lineABPoints.size(); i++) {
            setPixel(surface, (float)lineABPoints[i].x, (float)lineABPoints[i].y, colorRed);
        }
        std::vector<glm::ivec2> points; // The points to render
        // Render all triangles, in order defined in the list
        for (int i = 0; i < numTriangles; i++) {
            renderTriangle(points, tList[i].v0, tList[i].v1, tList[i].v2);
        }
        for (int i = 0; i < points.size(); i++) {
            setPixel(surface, points[i].x, points[i].y, colorBlue);
        }
        SDL_DestroyTexture(texture);
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_RenderTexture(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    }
    SDL_DestroySurface(surface);
    SDL_DestroyTexture(texture);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}    



    //Note:  Alternatively, could load an image like this:
    /*const char *file = "/Users/mae/workspace/graphics-projects/rasten/image.png";
    SDL_Surface* pngSurface = SDL_LoadPNG(file);
    if (!pngSurface) { SDL_Log("SDL_LoadPNG failed: %s", SDL_GetError()); } */
    