#pragma once

#include "GLMExport.h"
#include "MoonlitExport.h"

// TODO: Add a dirty flag to avoid recalculating the view matrix on every call
namespace Moonlit
{
	struct MOONLIT_API Camera
	{
		Camera(glm::vec3 _pos, glm::vec3 _lookDir, glm::vec3 _worldUp);

		void Rotate(glm::vec3 _axis, float _amount);
		void Translate(glm::vec3 _vec);

		glm::vec3 GetPosition() const { return m_position; };

		glm::vec3 GetRightVector() const;
		glm::vec3 GetForwardVector() const;
		glm::vec3 GetUpVector() const;

		glm::highp_mat4 GetViewMatrix() const { return m_viewMatrix; };

	private:
		glm::highp_mat4 m_viewMatrix;

		glm::vec3 m_position;
		glm::vec3 m_forward;
		glm::vec3 m_up;
	};
}