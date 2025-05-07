#pragma once

#include <glm/glm.hpp>

struct Camera
{
	Camera(glm::vec3 _pos, glm::vec3 _lookDir, glm::vec3 _worldUp);

	void Rotate(glm::vec3 _axis, float _amount);
	void Translate(glm::vec3 _vec);

	glm::vec3 GetRightVector() const;
	glm::vec3 GetForwardVector() const;
	glm::vec3 GetUpVector() const;

	glm::highp_mat4 GetViewMatrix() const { return ViewMatrix; };

private:
	glm::highp_mat4 ViewMatrix;

	glm::vec3 Position;
	glm::vec3 Forward;
	glm::vec3 Up;
};