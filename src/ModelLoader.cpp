#include "ModelLoader.hpp"
#include <stdexcept>
#include <cstring>

Model ModelLoader::loadFromGltf(tinygltf::Model& gltfModel)
{
    Model model;
    // Loop through all meshes in the glTF file
    std::vector<tinygltf::Mesh>& gltfMeshes = gltfModel.meshes;
    int meshCount = gltfMeshes.size();
    for (size_t i = 0; i < meshCount; i++) {
        tinygltf::Mesh& gltfMesh = gltfMeshes[i];
        Mesh mesh = loadMesh(gltfModel, gltfMesh);
        model.addMesh(mesh);
    }
    return model;
}


Mesh ModelLoader::loadMesh(tinygltf::Model& gltfModel, tinygltf::Mesh& gltfMesh)
{
    Mesh mesh;

    int primitiveCount = (int)gltfMesh.primitives.size();

    for (int p = 0; p < primitiveCount; p++) {
        tinygltf::Primitive& primitive = gltfMesh.primitives[p];

        // POSITION
        std::map<std::string, int>& attributes = primitive.attributes;

        std::map<std::string, int>::iterator posIt = attributes.find("POSITION");

        if (posIt == attributes.end()) {
            throw std::runtime_error("Mesh missing POSITION attribute");
        }

        tinygltf::Accessor& posAccessor = gltfModel.accessors[posIt->second];
        uint8_t* posBase = getAttributePtr(gltfModel, posAccessor);
        int posStride = (int)posAccessor.ByteStride(gltfModel.bufferViews[posAccessor.bufferView]);

        // NORMAL
        std::map<std::string, int>::iterator normalIterator =
            attributes.find("NORMAL");

        bool hasNormals = (normalIterator != attributes.end());
        tinygltf::Accessor* normalAccessorPtr = 0;
        uint8_t* normalBase = 0;
        int normalStride = 0;

        if (hasNormals) {
            normalAccessorPtr = &gltfModel.accessors[normalIterator->second];
            normalBase = getAttributePtr(gltfModel, *normalAccessorPtr);
            normalStride = (int)normalAccessorPtr->ByteStride(gltfModel.bufferViews[normalAccessorPtr->bufferView]);
        }

        // TEXCOORD_0 
        std::map<std::string, int>::iterator uvIterator = attributes.find("TEXCOORD_0");

        bool hasUVs = (uvIterator != attributes.end());
        tinygltf::Accessor* uvAccessorPtr = 0;
        uint8_t* uvBase = 0;
        int uvStride = 0;

        if (hasUVs) {
            uvAccessorPtr = &gltfModel.accessors[uvIterator->second];
            uvBase = getAttributePtr(gltfModel, *uvAccessorPtr);
            uvStride = (int)uvAccessorPtr->ByteStride(gltfModel.bufferViews[uvAccessorPtr->bufferView]);
        }

        // Allocate vertices
        int vertexCount = (int)posAccessor.count;
        mesh.vertices.resize(vertexCount);

        for (int i = 0; i < vertexCount; i++) {
            // POSITION
            glm::vec3* posPtr = (glm::vec3*)(posBase + i * posStride);
            mesh.vertices[i].position = *posPtr;
            // NORMAL or default
            if (hasNormals) {
                glm::vec3* norPtr = (glm::vec3*)(normalBase + i * normalStride);
                mesh.vertices[i].normal = *norPtr;
            } else {
                mesh.vertices[i].normal = glm::vec3(0.0f, 0.0f, 1.0f);
            }
            // UV or default
            if (hasUVs) {
                glm::vec2* uvPtr = (glm::vec2*)(uvBase + i * uvStride);
                mesh.vertices[i].texcoord = *uvPtr;
            } else {
                mesh.vertices[i].texcoord = glm::vec2(0.0f, 0.0f);
            }
        }

        // Indices 
        tinygltf::Accessor& idxAccessor = gltfModel.accessors[primitive.indices];
        uint8_t* idxBase = getAttributePtr(gltfModel, idxAccessor);
        int idxCount = (int)idxAccessor.count;
        mesh.indices.resize(idxCount);
        if (idxAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
            uint16_t* src = (uint16_t*)idxBase;
            for (int i = 0; i < idxCount; i++) {
                mesh.indices[i] = (uint32_t)src[i];
            }
        }
        else if (idxAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT) {
            uint32_t* src = (uint32_t*)idxBase;
            for (int i = 0; i < idxCount; i++) {
                mesh.indices[i] = src[i];
            }
        }
        else {
            throw std::runtime_error("Unsupported index type");
        }
    }
    return mesh;
}



uint8_t* ModelLoader::getAttributePtr(tinygltf::Model& model, tinygltf::Accessor& accessor)
{
    tinygltf::BufferView& view = model.bufferViews[accessor.bufferView];
    tinygltf::Buffer& buffer = model.buffers[view.buffer];
    int offset = (int)view.byteOffset + (int)accessor.byteOffset;
    return buffer.data.data() + offset;
}
