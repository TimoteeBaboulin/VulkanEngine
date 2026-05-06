#pragma once

#include "Engine/ResourceManagement/ResourceManager.h"
#include "common.h"
#include "../ParameterEditor.h"
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
		bool m_refreshingDropdown = false;

		void RefreshDropdownOptions();
		void OnMeshSelected(int _index);
	};
}