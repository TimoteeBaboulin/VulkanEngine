#include "Camera.h"
#include "Inputs/InputManager.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"

Camera::Camera(glm::vec3 _pos, glm::vec3 _lookDir, glm::vec3 _worldUp) : Position(_pos), Forward(_lookDir), Up(_worldUp)
{
	ViewMatrix = glm::lookAt(Position, Position + Forward, Up);
}

void Camera::Rotate(glm::vec3 _axis, float _amount)
{
	Forward = glm::rotate(Forward, _amount, _axis);

	ViewMatrix = glm::lookAt(Position, Position + Forward, Up);
}

void Camera::Translate(glm::vec3 _vec)
{
	Position += _vec;

	ViewMatrix = glm::lookAt(Position, Position + Forward, Up);
}

glm::vec3 Camera::GetRightVector() const
{
	return glm::cross(Forward, Up);
}

glm::vec3 Camera::GetForwardVector() const
{
	return Forward;
}

glm::vec3 Camera::GetUpVector() const
{
	return Up;
}
