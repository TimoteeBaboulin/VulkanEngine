#pragma once
#include "Engine/Component/ObjectBehaviour.h"
#include "common.h"
#include "../../../MoonlitEditor/Sources/Editor/Widgets/ParameterTypes/MeshData.h"
#include "Engine/ResourceManagement/Resources/Mesh.h"
#include "Engine/Events/EventUtility.h"
#include "Engine/ResourceManagement/ResourceHandle.h"


class MeshRendererBehaviour : public Moonlit::ObjectBehaviour
{
public:
	MeshRendererBehaviour() = delete;
	MeshRendererBehaviour(Moonlit::GameObject* _owner);
	MeshRendererBehaviour(Moonlit::GameObject* _owner, std::shared_ptr<Moonlit::MeshData> _mesh);

	virtual ~MeshRendererBehaviour();

	std::vector<ParameterBase*> GetParameters() override;
	void ParameterChanged(const Moonlit::ParameterRepositoryEntry& _parameter) override;

private:
	void LookForTransformComponent();
	void UpdateMeshInstanceModel();

	class TransformBehaviour* m_transformComponent = nullptr;

	Parameter<Moonlit::ResourceManagement::ResourceHandle<Moonlit::MeshData>> m_meshData = Parameter<Moonlit::ResourceManagement::ResourceHandle<Moonlit::MeshData>>("MeshData", nullptr);

	Moonlit::Events::ScopedEventSubscriber<>* m_transformChangedSubscriber = nullptr;
	uint32_t m_instanceId = 0;
};
