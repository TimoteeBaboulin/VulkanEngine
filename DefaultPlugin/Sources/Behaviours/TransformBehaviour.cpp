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
	m_translationMatrix = glm::translate(glm::mat4(1), *m_position);
	m_rotationMatrix = glm::toMat4(*m_rotation);
	m_scaleMatrix = glm::scale(glm::mat4(1), *m_scale);
	m_transformMatrix = m_translationMatrix * m_rotationMatrix * m_scaleMatrix;
}

TransformBehaviour::~TransformBehaviour()
{
	LOG_INFO("TransformBehaviour::~TransformBehaviour called");
}

void TransformBehaviour::SubscribeToFunctions()
{
}

std::vector<ParameterBase*> TransformBehaviour::GetParameters()
{
	std::vector<ParameterBase*> entries = ObjectBehaviour::GetParameters();

	entries.push_back(&m_position);
	entries.push_back(&m_scale);
	entries.push_back(&m_rotation);

	return entries;
}

void TransformBehaviour::ParameterChanged(const ParameterBase* _parameter)
{
	m_isDirty = true;
	OnTransformChanged(this);
}

void TransformBehaviour::SetPosition(glm::vec3 _position)
{
	m_position = _position;

	m_isDirty = true;
	OnTransformChanged(this);
}

void TransformBehaviour::Translate(glm::vec3 _translation)
{
	*m_position += _translation;
	m_isDirty;

	LOG_INFO("New position is {" + std::to_string((*m_position).x) + "," + std::to_string((*m_position).y) + ","
		+ std::to_string((*m_position).z) + "}");

	OnTransformChanged(this);
}

void TransformBehaviour::Rotate(glm::vec3 _rotation)
{
	rotate((*m_rotation), _rotation);

	m_isDirty = true;
	OnTransformChanged(this);
}

glm::mat4 TransformBehaviour::GetModelMat()
{
	if (m_isDirty)
		RecalculateMatrices();

	return m_transformMatrix;
}

void TransformBehaviour::RecalculateMatrices()
{
	m_translationMatrix = glm::translate(glm::mat4(1), *m_position);
	m_rotationMatrix = glm::toMat4(*m_rotation);
	m_scaleMatrix = glm::scale(glm::mat4(1), *m_scale);
	m_transformMatrix = m_translationMatrix * m_rotationMatrix * m_scaleMatrix;

	m_isDirty = false;
}
