
#ifndef RENDER
#define RENDER

#include <glm/glm.hpp>
#include "Geom.hpp"
#include <SDL3/SDL.h>


struct RasterBuffer
{
    Color* pixels = nullptr;
    std::uint32_t width = 0;
    std::uint32_t height = 0;
};

void setSurfaceColor(SDL_Surface *surface, int width, int height, Color color);
void setPixel(SDL_Surface *surface, int x, int y, Color color);
void bresenhamLine(std::vector<glm::ivec2>& points, int x0, int y0, int x1, int y1);
void renderTriangle(std::vector<glm::ivec2>& points, glm::ivec2 A, glm::ivec2 B, glm::ivec2 C);


#endif