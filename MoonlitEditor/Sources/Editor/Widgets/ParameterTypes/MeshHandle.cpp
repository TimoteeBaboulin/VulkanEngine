#include "MeshHandle.h"

#include <qlayout.h>

#include "Engine/ResourceManagement/Banks/MeshBank.h"

namespace Moonlit::Editor
{
    MeshHandleParameterEditor::~MeshHandleParameterEditor()
    {
        delete m_meshDropdown;
        m_meshHandles.clear();
    }

    void MeshHandleParameterEditor::SetParameterUI()
    {
        m_meshDropdown = new QComboBox(this);
        m_meshDropdown->setMinimumWidth(150);
        m_meshDropdown->setMinimumHeight(25);

        m_layout->addWidget(m_meshDropdown);

        connect(m_meshDropdown, &QComboBox::currentIndexChanged, this, &MeshHandleParameterEditor::OnMeshSelected);

        RefreshDropdownOptions();
    }

    void MeshHandleParameterEditor::RefreshDropdownOptions()
    {
        m_refreshingDropdown = true;

        m_meshDropdown->clear();
        m_meshHandles.clear();
        ResourceManagement::MeshBank& meshBank = ResourceManagement::MeshBank::Get();
        m_meshHandles = meshBank.GetAllResources();

        LOG_INFO("RefreshDropdownOptions: Found " + std::to_string(m_meshHandles.size()) + " meshes in MeshBank.");

        for (int i = 0; i < m_meshHandles.size(); i++)
        {
            m_meshDropdown->addItem(QString::fromStdString(m_meshHandles[i].Name()));
        }

        const Renderer::MeshHandle& currentHandle = m_parameter.Value();
        for (int i = 0; i < m_meshHandles.size(); i++)
        {
            if (m_meshHandles[i].Name() == currentHandle.Name())
            {
                m_meshDropdown->setCurrentIndex(i);
                break;
            }
        }

        m_refreshingDropdown = false;
    }

    void MeshHandleParameterEditor::OnMeshSelected(int _index)
    {
        if (_index < 0 || _index >= static_cast<int>(m_meshHandles.size()) || m_refreshingDropdown)
        {
        	return;
        }

        Renderer::MeshHandle selectedMesh = m_meshHandles[_index];
        ValueChanged(selectedMesh);
    }
}