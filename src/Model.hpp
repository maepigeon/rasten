#ifndef MODEL
#define MODEL

#include <vector>
#include "Mesh.hpp"
class Model {
public:
    std::vector<Mesh>& getMeshes();
    void addMesh(Mesh& mesh);
    bool empty();
private:
    std::vector<Mesh> meshes;
};
#endif