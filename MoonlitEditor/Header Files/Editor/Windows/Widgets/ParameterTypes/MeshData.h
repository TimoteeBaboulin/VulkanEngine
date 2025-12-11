#pragma once

#include "ParameterEditor.h"
#include <qcombobox.h>
#include "ResourceManagement/ResourceManager.h"
#include "common.h"

namespace Editor
{
	namespace Widgets
	{
		class MeshDataParameterEditor : public ParameterEditor
		{
		public:
			MeshDataParameterEditor(const ParameterRepositoryEntry& _entry, QWidget* _parent = nullptr);
			~MeshDataParameterEditor();
			void AddToLayout(QLayout* _layout) override;
			void RemoveFromLayout(QLayout* _layout) override;

		private:
			QComboBox* m_meshDropdown;
			std::vector<ResourcePair<MeshData>> m_meshes;

			void OnMeshSelectionChanged(int _index);

			void ConnectSignals();
			void SetUpUI();
			void PopulateMeshDropdown();
		};
	}
}