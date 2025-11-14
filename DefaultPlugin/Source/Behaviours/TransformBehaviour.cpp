#include "pch.h"
#include "Behaviours/TransformBehaviour.h"

TransformBehaviour::TransformBehaviour(GameObject* _owner, glm::vec3 _position, glm::vec3 _scale, glm::fquat _rotation) : ObjectBehaviour(_owner)
{
	m_position = _position;
	m_rotation = _rotation;
	m_scale = _scale;
	m_translationMatrix = glm::translate(glm::mat4(1), m_position);
	m_rotationMatrix = glm::toMat4(m_rotation);
	m_scaleMatrix = glm::scale(glm::mat4(1), m_scale);
	m_transformMatrix = m_translationMatrix * m_rotationMatrix * m_scaleMatrix;
}

void TransformBehaviour::SubscribeToFunctions()
{
}

std::vector<ParameterRepositoryEntry> TransformBehaviour::GetParameterEntries()
{
	std::vector<ParameterRepositoryEntry> entries = ObjectBehaviour::GetParameterEntries();

	entries.push_back(ParameterRepositoryEntry{ "Position", typeid(m_position).name(), sizeof(m_position), &m_position});
	entries.push_back(ParameterRepositoryEntry{ "Scale", typeid(m_scale).name(), sizeof(m_scale), &m_scale });
	entries.push_back(ParameterRepositoryEntry{ "Rotation", typeid(m_rotation).name(), sizeof(m_rotation), &m_rotation });

	return entries;
}

void TransformBehaviour::ParameterChanged()
{
	m_transformMatrix = m_translationMatrix * m_rotationMatrix * m_scaleMatrix;
}

void TransformBehaviour::SetPosition(glm::vec3 _position)
{
	m_position = _position;

	// TODO: Use a dirty flag to recalculate the matrices in late update
	m_translationMatrix = glm::translate(glm::mat4(1), m_position);
	m_transformMatrix = m_translationMatrix * m_rotationMatrix * m_scaleMatrix;
}
