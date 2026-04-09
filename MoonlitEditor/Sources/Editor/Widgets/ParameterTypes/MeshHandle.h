#pragma once

#include "Engine/ResourceManagement/ResourceManager.h"
#include "ParameterEditor.h"
#include "common.h"
#include "Editor/Widgets/ParameterEditor.h"
#include "Engine/Renderer/Renderer.h"

#include <qcombobox.h>

using MeshData = Moonlit::MeshData;

namespace Moonlit::Editor
{
	class MeshHandleParameterEditor : public ParameterEditor<Renderer::MeshHandle>
	{
	public:
		MeshHandleParameterEditor(Parameter* _parameter, ObjectBehaviour* _behaviour, QWidget* _parent = nullptr)
			: ParameterEditor(_parameter, _behaviour, _parent) {};
		~MeshHandleParameterEditor() override;

		void SetParameterUI() override;

	private:
		QComboBox* m_meshDropdown;
		std::vector<Renderer::MeshHandle> m_meshHandles;

		void RefreshDropdownOptions();
		void OnMeshSelected(int _index);
	};

	// namespace Widgets
	// {
	// 	class MeshDataParameterEditor : public ParameterEditor
	// 	{
	// 	public:
	// 		MeshDataParameterEditor(const Moonlit::ParameterRepositoryEntry& _entry, QWidget* _parent = nullptr);
	// 		~MeshDataParameterEditor();
	// 		void AddToLayout(QLayout* _layout) override;
	// 		void RemoveFromLayout(QLayout* _layout) override;
	//
	// 	private:
	// 		QComboBox* m_meshDropdown;
	// 		std::vector<Moonlit::ResourceManagement::ResourceHandle<MeshData>> m_meshes;
	//
	// 		void OnMeshSelectionChanged(int _index);
	//
	// 		void ConnectSignals();
	// 		void SetUpUI();
	// 		void PopulateMeshDropdown();
	// 	};
	// }
}