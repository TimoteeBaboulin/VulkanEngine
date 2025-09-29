#include "ResourceManagement/MeshBank.h"

#include <filesystem>

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/mesh.h"
#include "assimp/scene.h"

//ResourceBank<MeshData>* MeshBank::Instance = new MeshBank();

MeshData GetMesh(aiMesh* _mesh)
{
    MeshData mesh;
    mesh.vertexCount = _mesh->mNumVertices;
    mesh.vertices = new Vertex[mesh.vertexCount];

    for (int i = 0; i < _mesh->mNumVertices; i++)
    {
        mesh.vertices[i] = Vertex(_mesh->mVertices[i].x, _mesh->mVertices[i].y, _mesh->mVertices[i].z);

        mesh.vertices[i].uvX = _mesh->mTextureCoords[0][i].x;
        mesh.vertices[i].uvY = _mesh->mTextureCoords[0][i].y;
    }
    mesh.triangleCount = _mesh->mNumFaces;
    mesh.indices = new uint16_t[mesh.triangleCount * 3];

    for (int i = 0; i < mesh.triangleCount; i++)
    {
        mesh.indices[i * 3] = _mesh->mFaces[i].mIndices[0];
        mesh.indices[i * 3 + 1] = _mesh->mFaces[i].mIndices[1];
        mesh.indices[i * 3 + 2] = _mesh->mFaces[i].mIndices[2];
    }

    return mesh;
}

bool MeshBank::TryLoad(std::string _filepath)
{
    namespace fs = std::filesystem;

    fs::path path(_filepath);
    assert(fs::is_regular_file(path));
    std::string name = path.stem().string();

    if (Exist(name))
    {
        std::cout << "[MeshBank] Mesh with name " << name << " already exist in MeshBank." << std::endl;
        return false;
    }

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(_filepath, aiPostProcessSteps::aiProcess_OptimizeMeshes | aiPostProcessSteps::aiProcess_Triangulate | aiPostProcessSteps::aiProcess_FlipUVs);
    if (scene == nullptr)
    {
        std::string errorMessage = importer.GetErrorString();
        throw new std::runtime_error(errorMessage);
    }

    std::shared_ptr<MeshData> meshPtr = std::make_shared<MeshData>();
    *meshPtr = GetMesh(scene->mMeshes[0]);

    m_resources.push_back(ResourcePair<MeshData>{
        name,
        meshPtr
    });

    return false;
}
