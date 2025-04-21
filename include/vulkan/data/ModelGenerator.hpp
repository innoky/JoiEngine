#pragma once

#include "vulkan/render/Vertex.hpp"
#include "extern/tiny_gltf.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>
#include <iostream>

inline std::pair<std::vector<Vertex>, std::vector<uint32_t>> LoadGLTFMesh_All(const std::string &path)
{
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    tinygltf::TinyGLTF loader;
    tinygltf::Model model;
    std::string err, warn;

    bool loaded = loader.LoadBinaryFromFile(&model, &err, &warn, path);
    if (!loaded)
        loaded = loader.LoadASCIIFromFile(&model, &err, &warn, path);

    if (!loaded)
    {
        std::cerr << "Failed to load GLTF: " << err << "\n";
        return {{}, {}};
    }

    size_t baseVertex = 0;

    for (const auto &node : model.nodes)
    {
        if (node.mesh < 0)
            continue;

        std::cout << "[GLTF Node] mesh: " << node.mesh;
        if (node.name.find("Plane001") != std::string::npos)
            continue;

        if (!node.translation.empty())
        {
            std::cout << ", translation: "
                      << node.translation[0] << ", "
                      << node.translation[1] << ", "
                      << node.translation[2];
        }

        if (!node.name.empty())
        {
            std::cout << ", name: " << node.name;
        }

        std::cout << std::endl;

        glm::mat4 nodeTransform(1.0f);
        if (!node.matrix.empty())
        {
            nodeTransform = glm::make_mat4(node.matrix.data());
        }
        else
        {
            if (!node.scale.empty())
            {
                nodeTransform = glm::scale(nodeTransform, glm::vec3(
                                                              node.scale[0], node.scale[1], node.scale[2]));
            }
            if (!node.rotation.empty())
            {
                glm::quat q(
                    node.rotation[3], node.rotation[0],
                    node.rotation[1], node.rotation[2]);
                nodeTransform *= glm::mat4_cast(q);
            }
            if (!node.translation.empty())
            {
                nodeTransform = glm::translate(nodeTransform, glm::vec3(
                                                                  node.translation[0], node.translation[1], node.translation[2]));
            }
        }

        const auto &mesh = model.meshes[node.mesh];
        for (const auto &primitive : mesh.primitives)
        {
            const auto &posAccessor = model.accessors[primitive.attributes.at("POSITION")];
            const auto &posView = model.bufferViews[posAccessor.bufferView];
            const auto &posBuffer = model.buffers[posView.buffer];
            const float *posData = reinterpret_cast<const float *>(
                posBuffer.data.data() + posView.byteOffset + posAccessor.byteOffset);

            size_t startVertex = vertices.size();
            size_t count = posAccessor.count;
            vertices.resize(startVertex + count);

            // --- COLOR ---
            bool hasColor = primitive.attributes.count("COLOR_0") > 0;
            const float *colorData = nullptr;
            if (hasColor)
            {
                const auto &colAccessor = model.accessors[primitive.attributes.at("COLOR_0")];
                const auto &colView = model.bufferViews[colAccessor.bufferView];
                const auto &colBuffer = model.buffers[colView.buffer];
                colorData = reinterpret_cast<const float *>(
                    colBuffer.data.data() + colView.byteOffset + colAccessor.byteOffset);
            }

            // --- UV ---
            bool hasUV = primitive.attributes.count("TEXCOORD_0") > 0;
            const float *uvData = nullptr;
            if (hasUV)
            {
                const auto &uvAccessor = model.accessors[primitive.attributes.at("TEXCOORD_0")];
                const auto &uvView = model.bufferViews[uvAccessor.bufferView];
                const auto &uvBuffer = model.buffers[uvView.buffer];
                uvData = reinterpret_cast<const float *>(
                    uvBuffer.data.data() + uvView.byteOffset + uvAccessor.byteOffset);
            }

            // --- FILL VERTICES ---
            for (size_t i = 0; i < count; ++i)
            {
                glm::vec3 pos = glm::make_vec3(&posData[i * 3]);
                glm::vec3 transformed = glm::vec3(nodeTransform * glm::vec4(pos, 1.0f));
                vertices[startVertex + i].pos = transformed;

                if (hasColor)
                    vertices[startVertex + i].color = glm::make_vec3(&colorData[i * 3]);
                else
                    vertices[startVertex + i].color = (i % 10 == 0) ? glm::vec3(0.25f) : glm::vec3(0.45f);

                if (hasUV)
                    vertices[startVertex + i].uv = glm::make_vec2(&uvData[i * 2]);
                else
                    vertices[startVertex + i].uv = glm::vec2(0.0f);
            }

            // --- INDICES ---
            const auto &idxAccessor = model.accessors[primitive.indices];
            const auto &idxView = model.bufferViews[idxAccessor.bufferView];
            const auto &idxBuffer = model.buffers[idxView.buffer];

            const void *indexData = idxBuffer.data.data() + idxView.byteOffset + idxAccessor.byteOffset;
            size_t indexCount = idxAccessor.count;

            size_t startIndex = indices.size();
            indices.resize(startIndex + indexCount);

            if (idxAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT)
            {
                const uint16_t *buf = static_cast<const uint16_t *>(indexData);
                for (size_t i = 0; i < indexCount; ++i)
                    indices[startIndex + i] = static_cast<uint32_t>(buf[i]) + baseVertex;
            }
            else if (idxAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT)
            {
                const uint32_t *buf = static_cast<const uint32_t *>(indexData);
                for (size_t i = 0; i < indexCount; ++i)
                    indices[startIndex + i] = buf[i] + baseVertex;
            }
            else
            {
                std::cerr << "Unsupported index type\n";
                continue;
            }

            baseVertex += count;
        }
    }

    std::cout << "[GLTF::ALL] Loaded " << vertices.size() << " vertices, "
              << indices.size() << " indices from " << path << "\n";

    return {vertices, indices};
}
