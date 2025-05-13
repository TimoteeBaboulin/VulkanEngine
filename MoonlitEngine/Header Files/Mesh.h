//#pragma once
//
//#include "Renderer/VulkanData.h"
//
//class Mesh
//{
//public:
//	void Load(MeshData _data);
//	void Load(vk::Device device, MeshData _data, vk::DescriptorSetLayout _layout, vk::DescriptorPool _pool);
//	void SetMaterials(MaterialInstance** _pInstances, size_t _instancesCount, vk::Device _device);
//	void CleanUp(vk::Device _device);
//	//void BindSets(vk::CommandBuffer _buffer, vk::PipelineLayout _layout);
//	void RecordCommandBuffer(vk::CommandBuffer _buffer, int _renderPass, vk::PipelineBindPoint _bindPoint);
//	void AddModelMatrix(glm::vec4 _mat);
//
//	std::vector<glm::mat4> m_modelMatrixes;
//
//	MeshCountType m_vertexCount;
//	MeshCountType m_triangleCount;
//
//	uint16_t m_textureCount;
//
//	vk::Buffer m_vertexBuffer;
//	vk::Buffer m_indexBuffer;
//
//	vk::DeviceMemory m_vertexMemory;
//	vk::DeviceMemory m_indexMemory;
//
//	std::vector<vk::Image> m_textureImages;
//	std::vector<vk::ImageView> m_textureImageViews;
//	std::vector<vk::Sampler> m_textureSamplers;
//
//	vk::DescriptorPool m_descriptorPool;
//
//	std::vector<MaterialInstance*> m_materials;
//};
//
//class MeshInstance
//{
//public:
//	//Create a mesh instance
//	friend bool operator==(const MeshInstance& _a, const MeshInstance& _b);
//
//
//private:
//	Mesh* m_mesh;
//	Material* m_material;
//	MaterialInstance* m_materialInstance;
//};