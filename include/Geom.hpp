#ifndef GEOM
#define GEOM
#include <cstdint>

#include <glm/glm.hpp>

struct Triangle {
    glm::ivec2 v0;
    glm::ivec2 v1;
    glm::ivec2 v2;
};
typedef union {
    struct {
        std::uint8_t r, g, b, a;
    } channels;
    std::uint32_t bits;
} Color;
struct ColoredTriangle {
    Triangle tri;
    Color color;
};

#endif