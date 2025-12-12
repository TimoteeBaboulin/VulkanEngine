#pragma once

#undef min
#undef max
#include <glm/common.hpp>

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <memory>
#include <vector>

#include "Engine/Components/ObjectBehaviour.h"
#include "Engine/Events/Event.h"

class TransformBehaviour : public ObjectBehaviour
{
public:
	Event<void> OnTransformChanged;

	TransformBehaviour(GameObject* _owner,
		glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 _scale = glm::vec3(1.0, 1.0f, 1.0f),
		glm::fquat _rotation = glm::fquat(glm::vec3(0.0f, 0.0f, 0.0f)));

	void SubscribeToFunctions() override;
	virtual std::vector<ParameterRepositoryEntry> GetParameterEntries() override;
	virtual void ParameterChanged(const ParameterRepositoryEntry& _parameter) override;

	//Setters
	void SetPosition(glm::vec3 _position);
	
	// Getters
	glm::vec3 GetPosition() const { return m_position; }
	glm::mat4 GetModelMat();
protected:
	glm::vec3 m_position;
	glm::vec3 m_scale;
	glm::fquat m_rotation;

	glm::mat4 m_translationMatrix;
	glm::mat4 m_rotationMatrix;
	glm::mat4 m_scaleMatrix;

	glm::mat4 m_transformMatrix;

	bool m_isDirty = false;

	void RecalculateMatrices();
};