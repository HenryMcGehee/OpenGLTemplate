#pragma once

#include "Test.h"
#include "../Shader.h"
#include <glm/\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace test {
	class TestTransformations : public Test
	{
	public:
		TestTransformations();
		~TestTransformations();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		unsigned int VBO, VAO, EBO, texture;
		float xPosition, yPosition, zPosition;
		float xRotation, yRotation, zRotation;
		float xScale = 1, yScale = 1, zScale = 1;
		glm::mat4 trans;
		std::unique_ptr<Shader> shader;
	};
}