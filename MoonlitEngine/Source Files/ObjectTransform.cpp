#include "ObjectTransform.h"

ObjectTransform::ObjectTransform()
{
	m_position = glm::vec3(0, 0, 0);
	m_rotation = glm::fquat(0, 0, 0, 1);
	m_scale = glm::vec3(1, 1, 1);

	m_translationMatrix = glm::translate(glm::mat4(), m_position);
	m_rotationMatrix = glm::toMat4(m_rotation);
	m_scaleMatrix = glm::scale(glm::mat4(1), m_scale);

	m_transformMatrix = m_translationMatrix * m_rotationMatrix * m_scaleMatrix;
}

void ObjectTransform::SubscribeToFunctions()
{
}