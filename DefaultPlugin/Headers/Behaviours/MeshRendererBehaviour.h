#pragma once
#include "Engine/Components/ObjectBehaviour.h"
#include "common.h"
#include "ResourceManagement/Mesh.h"


class MeshRendererBehaviour : public ObjectBehaviour
{
public:
	MeshRendererBehaviour() = delete;
	MeshRendererBehaviour(GameObject* _owner);
	MeshRendererBehaviour(GameObject* _owner, std::shared_ptr<MeshData> _mesh);

private:
	void LookForTransformComponent();

	MeshInstance m_meshInstance;
};