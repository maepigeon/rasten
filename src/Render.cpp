#include <glm/glm.hpp>
#include <iostream>

#include "Geom.hpp"
#include "Render.hpp"

void setSurfaceColor(SDL_Surface *surface, int width, int height, Color color) {
    SDL_LockSurface(surface);
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            setPixel(surface, x, y, color);
        }
    }
    SDL_UnlockSurface(surface);
}


// https://stackoverflow.com/questions/20070155/how-to-set-a-pixel-in-a-sdl-surface
void setPixel(SDL_Surface *surface, int x, int y, Color color)
{
    Uint32* const target_pixel = (Uint32 *) ((Uint8 *) surface->pixels
                + y * surface->pitch + x * SDL_BYTESPERPIXEL(surface->format));
    *target_pixel = color.bits;
}

void bresenhamLineH(std::vector<glm::ivec2>& points, int x0, int y0, int x1, int y1) {
    if (x0 > x1) { // Swap points 1 and 2
        int tmp = x0;
        x0 = x1;
        x1 = tmp;
        tmp = y0;
        y0 = y1;
        y1 = tmp;
    }
    int dx = x1 - x0;
    int dy = y1 - y0;

    int dir = (dy < 0) ? -1 : 1;
    dy *= dir;
    
    if (dx != 0) {
        int y = y0;
        int p = 2 * dy - dx;
        for (int i = 0; i <= dx; i++) {
            points.push_back({x0 + i, y});
            if (p >= 0) {
                y += dir;
                p = p - 2*dx;
            }
            p = p + 2*dy;
        }
    }
}
void bresenhamLineV(std::vector<glm::ivec2>& points, int x0, int y0, int x1, int y1) {
    if (y0 > y1) { // Swap points 1 and 2
        int tmp = x0;
        x0 = x1;
        x1 = tmp;
        tmp = y0;
        y0 = y1;
        y1 = tmp;
    }
    int dx = x1 - x0;
    int dy = y1 - y0;

    int dir = (dx < 0) ? -1 : 1;
    dx *= dir;
    
    if (dy != 0) {
        int x = x0;
        int p = 2 * dx - dy;
        for (int i = 0; i <= dy; i++) {
            points.push_back({x, y0 + i});
            if (p >= 0) {
                x += dir;
                p = p - 2*dy;
            }
            p = p + 2*dx;
        }
    }
}

// Used this video to learn Bresenham's line algorithm https://youtu.be/CceepU1vIKo?si=9oqYiaLDjKdF_4-t
void bresenhamLine(std::vector<glm::ivec2>& points, int x0, int y0, int x1, int y1) {
    if (abs(x1 - x0) > abs(y1-y0)) {
        return bresenhamLineH(points, x0, y0, x1, y1);
    } else {
        return bresenhamLineV(points, x0, y0, x1, y1);
    }
}

void updateLREdgesH(int* LPoints, int* RPoints, int minY, int x0, int y0, int x1, int y1) {
    if (x0 > x1) { // Swap points 1 and 2
        int tmp = x0;
        x0 = x1;
        x1 = tmp;
        tmp = y0;
        y0 = y1;
        y1 = tmp;
    }
    int dx = x1 - x0;
    int dy = y1 - y0;

    int dir = (dy < 0) ? -1 : 1;
    dy *= dir;
    
    if (dx != 0) {
        int y = y0;
        int p = 2 * dy - dx;
        for (int i = 0; i <= dx; i++) {
            int yIdx = y - minY;
            if (x0 + i < LPoints[yIdx]) {
                LPoints[yIdx] = x0 + i;
            }            
            if (x0 + i > RPoints[yIdx]) {
                RPoints[yIdx] = x0 + i;
            }
            if (p >= 0) {
                y += dir;
                p = p - 2*dx;
            }
            p = p + 2*dy;
        }
    }
}
void updateLREdgesV(int* LPoints, int* RPoints, int minY, int x0, int y0, int x1, int y1) {

    if (y0 > y1) { // Swap points 1 and 2
        int tmp = x0;
        x0 = x1;
        x1 = tmp;
        tmp = y0;
        y0 = y1;
        y1 = tmp;
    }
    int dx = x1 - x0;
    int dy = y1 - y0;

    int dir = (dx < 0) ? -1 : 1;
    dx *= dir;
    
    if (dy != 0) {
        int x = x0;
        int p = 2 * dx - dy;
        for (int i = 0; i <= dy; i++) {
            int yIdx = y0 + i - minY;
            if (x < LPoints[yIdx]) {
                LPoints[yIdx] = x;
            }            
            if (x > RPoints[yIdx]) {
                RPoints[yIdx] = x;
            }
            if (p >= 0) {
                x += dir;
                p = p - 2*dy;
            }
            p = p + 2*dx;
        }
    }
}

// Uses bresenham's line algorithm to generate an array of the leftmost and rightmost points in the triangle for each row
void getTriangleMaxMinArrays(int* LPointsArray, int* RPointsArray, int minY, int x0, int y0, int x1, int y1) {
    if (abs(x1 - x0) > abs(y1-y0)) {
        return updateLREdgesH(LPointsArray, RPointsArray, minY, x0, y0, x1, y1);
    } else {
        return updateLREdgesV(LPointsArray, RPointsArray, minY, x0, y0, x1, y1);
    }
}

// A is left-part of base, B is right-part of base, C is the point
void renderTriangle(std::vector<glm::ivec2>& points, glm::ivec2 A, glm::ivec2 B, glm::ivec2 C) {
    int maxY = std::max(std::max(A.y, B.y), C.y);
    int minY = std::min(std::min(A.y, B.y), C.y);
    int triHeight = maxY - minY;
    int leftPoints[triHeight];
    int rightPoints[triHeight]; 
    std::fill_n(leftPoints, triHeight, 1000000);
    std::fill_n(rightPoints, triHeight, -1000000);
    getTriangleMaxMinArrays(leftPoints, rightPoints, minY, A.x, A.y, B.x, B.y);
    getTriangleMaxMinArrays(leftPoints, rightPoints, minY, B.x, B.y, C.x, C.y);
    getTriangleMaxMinArrays(leftPoints, rightPoints, minY, C.x, C.y, A.x, A.y);
    for (int y = 0; y < triHeight; y++) {
        int lx = leftPoints[y];
        int rx = rightPoints[y];
        // Draw a line from lx to rx
        for (int x = lx; x <= rx; x++) {
            points.push_back({x, y + minY});
        }
    } 
}