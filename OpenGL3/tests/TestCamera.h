#pragma once

#include "Test.h"
#include "../Shader.h"
#include <glm/\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace test {
	class TestCamera : public Test
	{
	public:
		TestCamera();
		~TestCamera();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		unsigned int VBO, VAO, EBO, texture;
		float xPosition, yPosition, zPosition;
		float xRotation, yRotation, zRotation;
		glm::vec3 modelPosition, modelRotation, modelScale;
		glm::vec3 cameraPosition, cameraRotation, cameraTarget, cameraDirection;
		float fov;
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;
		std::unique_ptr<Shader> shader;
	};
}