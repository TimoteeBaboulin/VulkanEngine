#include "ResourceManagement/MeshBank.h"

#include <filesystem>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/mesh.h>
#include <assimp/scene.h>

#include "Debug/Logger.h"

//ResourceBank<MeshData>* MeshBank::Instance = new MeshBank();

MeshData GetMesh(aiMesh* _mesh)
{
    MeshData mesh;
    mesh.vertexCount = _mesh->mNumVertices;
    mesh.vertices = new Vertex[mesh.vertexCount];

	bool hasUVs = _mesh->HasTextureCoords(0);
	bool hasNormals = _mesh->HasNormals();
	bool hasTangentsAndBitangents = _mesh->HasTangentsAndBitangents();

    for (int i = 0; i < _mesh->mNumVertices; i++)
    {
		Vertex& vertex = mesh.vertices[i];
        vertex.position.x = _mesh->mVertices[i].x;
		vertex.position.y = _mesh->mVertices[i].y;
		vertex.position.z = _mesh->mVertices[i].z;

        if (hasUVs)
        {
            vertex.uv.x = _mesh->mTextureCoords[0][i].x;
            vertex.uv.y = _mesh->mTextureCoords[0][i].y;
        }

        if (hasNormals)
        {
            vertex.normal.x = _mesh->mNormals[i].x;
            vertex.normal.y = _mesh->mNormals[i].y;
            vertex.normal.z = _mesh->mNormals[i].z;
        }

        if (hasTangentsAndBitangents)
        {
            vertex.tangeant.x = _mesh->mTangents[i].x;
            vertex.tangeant.y = _mesh->mTangents[i].y;
            vertex.tangeant.z = _mesh->mTangents[i].z;

            vertex.bitangeant.x = _mesh->mBitangents[i].x;
            vertex.bitangeant.y = _mesh->mBitangents[i].y;
            vertex.bitangeant.z = _mesh->mBitangents[i].z;
        }
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
    if (!fs::is_regular_file(path))
    {
        LOG_ERROR("Couldn't find the file at " + _filepath);
		return false;
    }

    std::string name = path.stem().string();

    if (Exist(name))
    {
		Logger::LogWarning("Mesh with name " + name + " already exist in MeshBank.");
        return false;
    }

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(_filepath, 
        aiPostProcessSteps::aiProcess_Triangulate |
        aiPostProcessSteps::aiProcess_GenNormals |
        aiPostProcessSteps::aiProcess_CalcTangentSpace);
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
