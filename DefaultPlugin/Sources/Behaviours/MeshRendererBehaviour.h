#pragma once
#include "Engine/Component/ObjectBehaviour.h"
#include "common.h"
#include "Engine/ResourceManagement/Resources/Mesh.h"
#include "Engine/Events/EventUtility.h"


class MeshRendererBehaviour : public Moonlit::ObjectBehaviour
{
public:
	MeshRendererBehaviour() = delete;
	MeshRendererBehaviour(Moonlit::GameObject* _owner);
	MeshRendererBehaviour(Moonlit::GameObject* _owner, std::shared_ptr<Moonlit::MeshData> _mesh);

	virtual ~MeshRendererBehaviour();

	std::vector<Moonlit::ParameterRepositoryEntry> GetParameterEntries() override;
	void ParameterChanged(const Moonlit::ParameterRepositoryEntry& _parameter) override;

private:
	void LookForTransformComponent();
	void UpdateMeshInstanceModel();

	class TransformBehaviour* m_transformComponent = nullptr;

	std::shared_ptr<Moonlit::MeshData> m_meshData = nullptr;

	Moonlit::Events::ScopedEventSubscriber<>* m_transformChangedSubscriber = nullptr;
	uint32_t m_instanceId = 0;
};