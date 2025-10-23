#pragma once
#include "Engine/Components/ObjectBehaviour.h"

#include "ResourceManagement/Mesh.h"


class ObjectMeshRenderer : public ObjectBehaviour
{
public:
	ObjectMeshRenderer() = delete;
	ObjectMeshRenderer(GameObject* _owner);
	ObjectMeshRenderer(GameObject* _owner, std::shared_ptr<MeshData> _mesh);

private:
	void LookForTransformComponent();

	MeshInstance m_meshInstance;
};