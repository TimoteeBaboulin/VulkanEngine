#pragma once
#include "Engine/Components/ObjectBehaviour.h"
#include "common.h"
#include "ResourceManagement/Mesh.h"
#include "Engine/Events/EventUtility.h"


class MeshRendererBehaviour : public ObjectBehaviour
{
public:
	MeshRendererBehaviour() = delete;
	MeshRendererBehaviour(GameObject* _owner);
	MeshRendererBehaviour(GameObject* _owner, std::shared_ptr<MeshData> _mesh);

	~MeshRendererBehaviour();

	std::vector<ParameterRepositoryEntry> GetParameterEntries() override;

private:
	void LookForTransformComponent();
	void UpdateMeshInstanceModel();

	class TransformBehaviour* m_transformComponent = nullptr;

	std::shared_ptr<MeshData> m_meshData = nullptr;

	ScopedEventSubscriber<>* m_transformChangedSubscriber = nullptr;
	uint32_t m_instanceId = 0;
};