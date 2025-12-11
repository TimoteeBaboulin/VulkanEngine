#include "MeshData.h"
#include "ResourceManagement/ResourceManager.h"
#include "common.h"
#include <qlayout.h>

Editor::Widgets::MeshDataParameterEditor::MeshDataParameterEditor(const ParameterRepositoryEntry& _entry, QWidget* _parent)
	: ParameterEditor(_entry, _parent)
{
	SetUpUI();
}

Editor::Widgets::MeshDataParameterEditor::~MeshDataParameterEditor()
{
}

void Editor::Widgets::MeshDataParameterEditor::AddToLayout(QLayout* _layout)
{
	_layout->addWidget(m_meshDropdown);
}

void Editor::Widgets::MeshDataParameterEditor::RemoveFromLayout(QLayout* _layout)
{
	_layout->removeWidget(m_meshDropdown);
}

void Editor::Widgets::MeshDataParameterEditor::OnMeshSelectionChanged(int _index)
{
	if (_index < 0 || _index >= static_cast<int>(m_meshes.size()))
	{
		return;
	}

	std::shared_ptr<MeshData> selectedMesh = m_meshes[_index].ResourcePtr;
	*reinterpret_cast<std::shared_ptr<MeshData>*>(m_entry.Data) = selectedMesh;
	OnParameterChanged.Invoke(this);
}

void Editor::Widgets::MeshDataParameterEditor::ConnectSignals()
{
	m_meshDropdown->connect(m_meshDropdown, &QComboBox::currentIndexChanged, this, &MeshDataParameterEditor::OnMeshSelectionChanged);
}

void Editor::Widgets::MeshDataParameterEditor::SetUpUI()
{
	m_meshDropdown = new QComboBox(this);
	m_meshDropdown->setMinimumWidth(150);

	PopulateMeshDropdown();
}

void Editor::Widgets::MeshDataParameterEditor::PopulateMeshDropdown()
{
	m_meshes = ResourceManager::Instance()->GetAllResourcesOfType<MeshData>();
	for (const auto& mesh : m_meshes)
	{
		m_meshDropdown->addItem(QString::fromStdString(mesh.Name));
	}

	std::shared_ptr<MeshData> currentMesh = *reinterpret_cast<std::shared_ptr<MeshData>*>(m_entry.Data);
	for (size_t i = 0; i < m_meshes.size(); ++i)
	{
		if (m_meshes[i].ResourcePtr == currentMesh)
		{
			m_meshDropdown->setCurrentIndex(static_cast<int>(i));
			break;
		}
	}

	ConnectSignals();
}
