#include "Model.hpp"

std::vector<Mesh>& Model::getMeshes() {
    return meshes;
}

void Model::addMesh(Mesh& mesh) {
    meshes.push_back(mesh);
}

bool Model::empty() {
    return meshes.empty();
}
