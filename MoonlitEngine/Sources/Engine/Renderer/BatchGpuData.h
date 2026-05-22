#pragma once

#include "../../common.h"
#include "CustomVulkanStructs.h"
#include "../ResourceManagement/Resources/TextureData.h"
#include "Material/Material.h"

class RendererDeviceManager;
class MaterialInstance;

struct MeshEntry;
struct DeviceData;

namespace Moonlit::Renderer { class RenderDevice; }

namespace Moonlit::Renderer
{

	/// <summary>
	/// Buffers and memory addresses for vertex/index/texture data
	/// </summary>
	struct DrawBufferResources
	{
		vk::Buffer vertexBuffer;
		vk::Buffer indexBuffer;
		vk::Buffer modelMatrixBuffer;

		vk::DeviceMemory vertexBufferMemory;
		vk::DeviceMemory indexBufferMemory;
		vk::DeviceMemory modelMatrixBufferMemory;

		std::vector<vk::DescriptorSet> textureDescriptorSets;
		std::vector<TextureData> textures;
	};

	/// <summary>
	/// This class owns the GPU resources for a DrawBuffer
	/// It handles the creation, updating, and destruction of these resources
	/// </summary>
	class MOONLIT_API BatchGpuData
	{
	public:
		BatchGpuData(RenderDevice& _renderDevice, Material& _baseMaterial,
			class DrawBuffer* _buffer);

		// Forbid copy, explicitly implement move
		// (Don't forget rule of five)

		BatchGpuData(BatchGpuData const&) = delete;
		BatchGpuData& operator=(BatchGpuData const&) = delete;
		BatchGpuData(BatchGpuData&& _src);
		BatchGpuData& operator=(BatchGpuData&& _rhs) noexcept;

		~BatchGpuData();

		DrawBufferResources const GetBufferResources() { return m_drawResources; };
		vk::Device const GetDevice() { return m_deviceData.Device; }

		void Initialise();
		void Render(vk::CommandBuffer& _cmd, std::string _renderPass,
			vk::DescriptorSet* _uboSet);

	private:
		uint32_t m_syncedVersion = UINT32_MAX;

		MaterialInstance* m_materialInstance = nullptr;

		DrawBuffer* m_parentBuffer;

		DrawBufferResources m_drawResources;
		DeviceData m_deviceData;
		RenderDevice* m_renderDevice;
		vk::CommandBuffer m_commandBuffer;

		std::vector<Image*> m_loadedImages;

		bool m_resourcesGenerated = false;

		// Helper function to create a TextureData from an Image
		TextureData GetTextureData(Image& _image);

		// This is used if the buffer is dirty to manually update resources
		void UpdateData();
		void UpdateBuffers();
		void UpdateTextures();
		void UpdateTextureSets();

		// Memory management
		void AllocateCommandBuffer();
		void AllocateTextureSets();

		void GenerateBuffers();
		void GenerateTextures(std::vector<Image*>& _textures);

		void ClearBuffers();
		void ClearTextures();
	};
}