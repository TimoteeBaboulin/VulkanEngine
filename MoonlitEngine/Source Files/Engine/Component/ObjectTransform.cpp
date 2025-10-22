#include "Engine/Components/ObjectTransform.h"

ObjectTransform::ObjectTransform(GameObject* _owner, glm::vec3 _position, glm::vec3 _scale, glm::fquat _rotation) : ObjectBehaviour(_owner)
{
	m_position = _position;
	m_rotation = _rotation;
	m_scale = _scale;
	m_translationMatrix = glm::translate(glm::mat4(1), m_position);
	m_rotationMatrix = glm::toMat4(m_rotation);
	m_scaleMatrix = glm::scale(glm::mat4(1), m_scale);
	m_transformMatrix = m_translationMatrix * m_rotationMatrix * m_scaleMatrix;
}

void ObjectTransform::SubscribeToFunctions()
{
}

void ObjectTransform::SetPosition(glm::vec3 _position)
{
	m_position = _position;

	// TODO: Use a dirty flag to recalculate the matrices in late update
	m_translationMatrix = glm::translate(glm::mat4(1), m_position);
	m_transformMatrix = m_translationMatrix * m_rotationMatrix * m_scaleMatrix;
}
