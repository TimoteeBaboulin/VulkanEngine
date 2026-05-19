#pragma once

#include "common.h"
#include "../ParameterEditor.h"
#include "Engine/ResourceManagement/ResourceManager.h"
#include "Engine/ResourceManagement/ResourceHandle.h"

#include <qcombobox.h>
#include <qlayout.h>

namespace Moonlit::Editor
{
    template<typename HANDLE_TYPE> struct ResourceHandleTraits;

    template<typename RESOURCE_TYPE>
    struct ResourceHandleTraits<ResourceManagement::ResourceHandle<RESOURCE_TYPE>>
    {
        using resource_type = RESOURCE_TYPE;
    };

    template<typename HANDLE_TYPE>
    class ResourceHandleParameterEditor : public ParameterEditor<HANDLE_TYPE>
    {
        using ResourceType = typename ResourceHandleTraits<HANDLE_TYPE>::resource_type;
        using Base         = ParameterEditor<HANDLE_TYPE>;
        using Parameter    = typename Base::Parameter;

    public:
        ResourceHandleParameterEditor(Parameter* _parameter, ObjectBehaviour* _behaviour, QWidget* _parent = nullptr)
            : Base(_parameter, _behaviour, _parent) {}

        ~ResourceHandleParameterEditor() override { delete m_dropdown; }

        void SetParameterUI() override
        {
            m_dropdown = new QComboBox(this);
            m_dropdown->setMinimumWidth(150);
            m_dropdown->setMinimumHeight(25);
            this->m_layout->addWidget(m_dropdown);

            QObject::connect(m_dropdown, &QComboBox::currentIndexChanged,
                    this, &ResourceHandleParameterEditor::OnSelected);

            RefreshDropdownOptions();
        }

    private:
        QComboBox*               m_dropdown  = nullptr;
        std::vector<HANDLE_TYPE> m_handles;
        bool                     m_refreshing = false;

        void RefreshDropdownOptions()
        {
            m_refreshing = true;
            m_dropdown->clear();
            m_handles = ResourceManagement::ResourceManager::Get().GetAllResourcesOfType<ResourceType>();

            for (const auto& handle : m_handles)
                m_dropdown->addItem(QString::fromStdString(handle.Name()));

            const HANDLE_TYPE& current = this->m_parameter.Value();
            for (int i = 0; i < static_cast<int>(m_handles.size()); i++)
            {
                if (m_handles[i].Name() == current.Name())
                {
                    m_dropdown->setCurrentIndex(i);
                    break;
                }
            }

            m_refreshing = false;
        }

        void OnSelected(int _index)
        {
            if (_index < 0 || _index >= static_cast<int>(m_handles.size()) || m_refreshing)
                return;

            this->ValueChanged(m_handles[_index]);
        }
    };
}
