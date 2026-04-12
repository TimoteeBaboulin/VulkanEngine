#include "Engine/Renderer/Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "Engine/Inputs/InputManager.h"

Moonlit::Camera::Camera(glm::vec3 _pos, glm::vec3 _lookDir, glm::vec3 _worldUp) : m_position(_pos), m_forward(_lookDir), m_up(_worldUp)
{
	m_viewMatrix = glm::lookAt(m_position, m_position + m_forward, m_up);
}

void Moonlit::Camera::Rotate(glm::vec3 _axis, float _amount)
{
	m_forward = glm::rotate(m_forward, _amount, _axis);

	m_viewMatrix = glm::lookAt(m_position, m_position + m_forward, m_up);
}

void Moonlit::Camera::Translate(glm::vec3 _vec)
{
	m_position += _vec;

	m_viewMatrix = glm::lookAt(m_position, m_position + m_forward, m_up);
}

glm::vec3 Moonlit::Camera::GetRightVector() const
{
	return glm::cross(m_forward, m_up);
}

glm::vec3 Moonlit::Camera::GetForwardVector() const
{
	return m_forward;
}

glm::vec3 Moonlit::Camera::GetUpVector() const
{
	return m_up;
}
