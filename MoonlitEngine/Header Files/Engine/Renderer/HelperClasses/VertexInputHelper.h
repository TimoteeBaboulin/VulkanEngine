#pragma once

#include <vector>

#include "Engine/Renderer/CustomVulkanStructs.h"

struct VertexInputDescription
{
	std::vector<vk::VertexInputBindingDescription>& Bindings;
	std::vector<vk::VertexInputAttributeDescription>& Attributes;
};

class VertexInputBuilder
{
public:

	VertexInputBuilder& AddBinding(vk::VertexInputRate _inputRate)
	{
		m_binding++;

		vk::VertexInputBindingDescription bindingDesc;
		bindingDesc.binding = m_binding;
		bindingDesc.inputRate = _inputRate;
		bindingDesc.stride = 0; // Will be calculated based on attributes added
		m_bindings.emplace_back(bindingDesc);

		m_offset = 0;

		return *this;
	}

	VertexInputBuilder& AddAttribute(vk::Format format, uint32_t size)
	{
		vk::VertexInputAttributeDescription attributeDesc;
		attributeDesc.binding = m_binding;
		attributeDesc.location = m_location;
		attributeDesc.format = format;
		attributeDesc.offset = m_offset;
		m_attributes.emplace_back(attributeDesc);
		m_location++;
		m_offset += size;

		m_bindings.back().stride += size;

		return *this;
	}

	VertexInputDescription Build()
	{
		VertexInputDescription description(m_bindings, m_attributes);
		return description;
	}

	vk::PipelineVertexInputStateCreateInfo BuildPipelineInfo()
	{
		vk::PipelineVertexInputStateCreateInfo vertexInputInfo;
		vertexInputInfo.sType = vk::StructureType::ePipelineVertexInputStateCreateInfo;
		vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(m_bindings.size());
		vertexInputInfo.pVertexBindingDescriptions = m_bindings.data();
		vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(m_attributes.size());
		vertexInputInfo.pVertexAttributeDescriptions = m_attributes.data();
		return vertexInputInfo;
	}

private:
	std::vector<vk::VertexInputBindingDescription> m_bindings;
	std::vector<vk::VertexInputAttributeDescription> m_attributes;

	uint32_t m_location = 0;
	uint32_t m_binding = -1;
	uint32_t m_offset = 0;
};