
#ifndef PIXEL_PROCESSOR
#define PIXEL_PROCESSOR

#include <glm/glm.hpp>
#include "RasterizerGeometry.hpp"
#include <SDL3/SDL.h>
void bresenhamLine(std::vector<glm::ivec2>& points, int x0, int y0, int x1, int y1);
void renderTriangle(std::vector<glm::ivec2>& points, glm::ivec2 A, glm::ivec2 B, glm::ivec2 C);

class Rasterizer {
private:
    SDL_Renderer* sdlRenderer;
    SDL_Texture* texture;
    SDL_Surface* surface;
    int numTriangles;
    int numLines;
    std::vector<Triangle> triangles;
    std::vector<Line> lines;
    Triangle t1, t2, t3;
    Line line1;
    std::vector<glm::ivec2> lineABPoints;
    int width, height;
    Color color;

public:
    struct RasterBuffer
    {
        Color* pixels = nullptr;
        std::uint32_t width = 0;
        std::uint32_t height = 0;
    };
    Rasterizer();
    void initGeometry();
    void destroy();
    void update();
    bool createCanvas(SDL_Renderer* renderer, SDL_Window* window, Color color, int width, int height);
    void setSurfaceColor(SDL_Surface *surface, int width, int height, Color color);
    void setPixel(SDL_Surface *surface, int x, int y, Color color);
};
#endif