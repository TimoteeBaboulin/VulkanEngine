#include "../pch.h"
#include "TransformBehaviour.h"

using GameObject = Moonlit::GameObject;
using ObjectBehaviour = Moonlit::ObjectBehaviour;

TransformBehaviour::TransformBehaviour(GameObject* _owner, glm::vec3 _position, glm::vec3 _scale, glm::fquat _rotation)
	: ObjectBehaviour(_owner), OnTransformChanged(this)
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

std::vector<Moonlit::ParameterRepositoryEntry> TransformBehaviour::GetParameterEntries()
{
	std::vector<Moonlit::ParameterRepositoryEntry> entries = ObjectBehaviour::GetParameterEntries();

	entries.push_back(Moonlit::ParameterRepositoryEntry{ "Position", typeid(m_position).name(), sizeof(m_position), &m_position});
	entries.push_back(Moonlit::ParameterRepositoryEntry{ "Scale", typeid(m_scale).name(), sizeof(m_scale), &m_scale });
	entries.push_back(Moonlit::ParameterRepositoryEntry{ "Rotation", typeid(m_rotation).name(), sizeof(m_rotation), &m_rotation });

	return entries;
}

void TransformBehaviour::ParameterChanged(const Moonlit::ParameterRepositoryEntry& _parameter)
{
	LOG_INFO("TransformBehaviour::ParameterChanged called on object " + std::to_string(reinterpret_cast<uintptr_t>(this)) + " for parameter " + std::string(_parameter.Name));

	m_translationMatrix = glm::translate(glm::mat4(1), m_position);

	m_isDirty = true;
	OnTransformChanged.Invoke(this);
}

void TransformBehaviour::SetPosition(glm::vec3 _position)
{
	m_position = _position;

	m_isDirty = true;
	OnTransformChanged.Invoke(this);
}

glm::mat4 TransformBehaviour::GetModelMat()
{
	if (m_isDirty)
		RecalculateMatrices();

	return m_transformMatrix;
}

void TransformBehaviour::RecalculateMatrices()
{
	m_translationMatrix = glm::translate(glm::mat4(1), m_position);
	m_rotationMatrix = glm::toMat4(m_rotation);
	m_scaleMatrix = glm::scale(glm::mat4(1), m_scale);
	m_transformMatrix = m_translationMatrix * m_rotationMatrix * m_scaleMatrix;

	m_isDirty = false;
}
