#pragma once

#undef min
#undef max

#include "Engine/Component/ObjectBehaviour.h"
#include "Engine/Events/Event.h"

#include <glm/common.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <vector>

class TransformBehaviour : public Moonlit::ObjectBehaviour
{
public:
	Moonlit::Events::Event<void> OnTransformChanged;

	TransformBehaviour(Moonlit::GameObject* _owner,
	                   glm::vec3 _position = glm::vec3(0.0f, 0.0f, 0.0f),
	                   glm::vec3 _scale = glm::vec3(1.0, 1.0f, 1.0f),
	                   glm::fquat _rotation = glm::fquat(glm::vec3(0.0f, 0.0f, 0.0f)));
	virtual ~TransformBehaviour();

	void SubscribeToFunctions() override;
	std::vector<ParameterBase*> GetParameters() override;
	void ParameterChanged(const Moonlit::ParameterRepositoryEntry& _parameter) override;

	//Setters
	void SetPosition(glm::vec3 _position);
	
	// Getters
	glm::vec3 GetPosition() const { return m_position.Value(); }
	glm::mat4 GetModelMat();
protected:
	Parameter<glm::vec3> m_position = Parameter<glm::vec3>("Position", 0,0,0);
	Parameter<glm::vec3> m_scale = Parameter<glm::vec3>("Scale", 0,0,0);
	Parameter<glm::fquat> m_rotation = Parameter<glm::fquat>("Rotation");

	glm::mat4 m_translationMatrix;
	glm::mat4 m_rotationMatrix;
	glm::mat4 m_scaleMatrix;

	glm::mat4 m_transformMatrix;

	bool m_isDirty = false;

	void RecalculateMatrices();
};