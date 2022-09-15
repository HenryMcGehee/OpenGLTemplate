#pragma once

#include "Test.h"
#include "../Shader.h"
#include <glm/\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace test {
	class TestColors : public Test
	{
	public:
		TestColors();
		~TestColors();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		unsigned int VBO, VAO, EBO, texture;
		unsigned int lightVAO;
		float xPosition, yPosition, zPosition;
		float xRotation, yRotation, zRotation;
		glm::vec3 modelPosition, modelRotation, modelScale;
		glm::vec3 lightPosition;
		glm::vec3 cameraPosition, cameraRotation, cameraTarget, cameraDirection;
		float fov;
		float shininess = 32.0f;
		float specular = 0.5f;
		glm::mat4 model;
		glm::mat4 model2;
		glm::mat4 view;
		glm::mat4 projection;
		float lightColor[3];
		float modelColor[3];
		std::unique_ptr<Shader> shader;
		std::unique_ptr<Shader> lightShader;
	};
}