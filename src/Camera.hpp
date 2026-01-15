#ifndef CAMERA
#define CAMERA

#include <SDL3/SDL_events.h>
#include <glm/glm.hpp>

class Camera {
public:
    glm::vec3 velocity;
    glm::vec3 position;
    // vertical rotation
    float pitch { 0.f };
    // horizontal rotation
    float yaw { 0.f };

    glm::mat4 getViewMatrix();
    glm::mat4 getRotationMatrix();

    void processSDLInputEvent(SDL_Event& e);
    void update();
};
#endif