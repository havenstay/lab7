#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

using namespace std;

class Model
{
public:

    vector<Mesh> meshes;
    string directory;

    Model(const string& path)
    {
        loadModel(path);
    }

    void Draw(Shader shader, glm::mat4 OX1_transform, glm::mat4 OX2_transform, glm::mat4 OX3_transform)
    {

        glm::mat4 flatCarriage = OX2_transform;
        flatCarriage[3][1] = 0.0f; 

        for (unsigned int i = 0; i < meshes.size(); ++i)
        {
            glm::mat4 currentModelMatrix = glm::mat4(1.0f);

            if (i == 5 || i == 10)
            {
                currentModelMatrix = flatCarriage;
            }
            else if (i == 6 || i == 7 || i == 8 || i == 9)
            {
                currentModelMatrix = OX2_transform;
            }

            else if (i == 11) 
            {
                glm::vec3 centerLeft = glm::vec3(-0.71f, 0.59f, 0.039f);
                glm::mat4 plus_center = glm::translate(glm::mat4(1.0f), centerLeft);
                glm::mat4 minus_center = glm::translate(glm::mat4(1.0f), -centerLeft);


                currentModelMatrix = OX2_transform * (plus_center * OX3_transform * minus_center);
            }
            else if (i == 12) 
            {
                glm::vec3 centerRight = glm::vec3(-0.91f, 0.64f, 0.039f); 
                glm::mat4 plus_center = glm::translate(glm::mat4(1.0f), centerRight);
                glm::mat4 minus_center = glm::translate(glm::mat4(1.0f), -centerRight);

                glm::mat4 mirrorRotate = glm::inverse(OX3_transform);

                currentModelMatrix = OX2_transform * (plus_center * mirrorRotate * minus_center);
            }
            else
            {
                currentModelMatrix = glm::mat4(1.0f);
            }
            shader.Use();
            shader.setMat4("model", currentModelMatrix);

            glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(currentModelMatrix)));
            shader.setMat3("normalMatrix", normalMatrix);

            shader.setVec3("mat.ambient", 1.0f, 1.0f, 1.0f);
            shader.setVec3("mat.diffuse", 1.0f, 1.0f, 1.0f);

            meshes[i].Draw(shader);
        }
    }

private:

    void loadModel(const string& path)
    {
        Assimp::Importer importer;

        const aiScene* scene = importer.ReadFile(
            path,
            aiProcess_Triangulate |
            aiProcess_FlipUVs |
            aiProcess_GenNormals
        );

        if (!scene ||
            scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
            !scene->mRootNode)
        {
            cerr << "ASSIMP ERROR: "
                << importer.GetErrorString()
                << endl;

            return;
        }

        directory =
            path.substr(0, path.find_last_of('/'));

        processNode(scene->mRootNode, scene);
    }

    void processNode(aiNode* node,
        const aiScene* scene)
    {
        for (unsigned int i = 0;
            i < node->mNumMeshes;
            ++i)
        {
            aiMesh* mesh =
                scene->mMeshes[node->mMeshes[i]];

            meshes.push_back(
                processMesh(mesh, scene)
            );
        }

        for (unsigned int i = 0;
            i < node->mNumChildren;
            ++i)
        {
            processNode(
                node->mChildren[i],
                scene
            );
        }
    }

    Mesh processMesh(aiMesh* mesh,
        const aiScene* scene)
    {
        vector<Vertex> vertices;
        vector<unsigned int> indices;

        for (unsigned int i = 0;
            i < mesh->mNumVertices;
            ++i)
        {
            Vertex vertex;

            vertex.Position.x =
                mesh->mVertices[i].x;

            vertex.Position.y =
                mesh->mVertices[i].y;

            vertex.Position.z =
                mesh->mVertices[i].z;

            if (mesh->HasNormals())
            {
                vertex.Normal.x =
                    mesh->mNormals[i].x;

                vertex.Normal.y =
                    mesh->mNormals[i].y;

                vertex.Normal.z =
                    mesh->mNormals[i].z;
            }

            vertices.push_back(vertex);
        }

        for (unsigned int i = 0;
            i < mesh->mNumFaces;
            ++i)
        {
            aiFace face = mesh->mFaces[i];

            for (unsigned int j = 0;
                j < face.mNumIndices;
                ++j)
            {
                indices.push_back(
                    face.mIndices[j]
                );
            }
        }

        return Mesh(vertices, indices);
    }
};

#endif