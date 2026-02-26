#pragma once

#include <qcombobox.h>
#include "Engine/ResourceManagement/ResourceManager.h"
#include "ParameterEditor.h"
#include "common.h"

using MeshData = Moonlit::MeshData;

namespace Editor
{
	namespace Widgets
	{
		class MeshDataParameterEditor : public ParameterEditor
		{
		public:
			MeshDataParameterEditor(const Moonlit::ParameterRepositoryEntry& _entry, QWidget* _parent = nullptr);
			~MeshDataParameterEditor();
			void AddToLayout(QLayout* _layout) override;
			void RemoveFromLayout(QLayout* _layout) override;

		private:
			QComboBox* m_meshDropdown;
			std::vector<Moonlit::ResourceManagement::ResourcePair<MeshData>> m_meshes;

			void OnMeshSelectionChanged(int _index);

			void ConnectSignals();
			void SetUpUI();
			void PopulateMeshDropdown();
		};
	}
}