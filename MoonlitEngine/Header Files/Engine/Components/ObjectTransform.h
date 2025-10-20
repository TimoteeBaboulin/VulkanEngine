#pragma once

#include <glm/common.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <memory>

#include "ObjectBehaviour.h"



class ObjectTransform : public ObjectBehaviour
{
public:
	ObjectTransform();

	void SubscribeToFunctions() override;

	glm::vec3 GetPosition() { return m_position; }
	std::shared_ptr<glm::mat4> GetSharedPtrModelMat() { return std::make_shared<glm::mat4>(m_transformMatrix); };
protected:
	glm::vec3 m_position;
	glm::vec3 m_scale;
	glm::fquat m_rotation;

	glm::mat4 m_translationMatrix;
	glm::mat4 m_rotationMatrix;
	glm::mat4 m_scaleMatrix;

	glm::mat4 m_transformMatrix;
};