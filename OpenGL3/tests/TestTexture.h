#pragma once

#include "Test.h"
#include "../Shader.h"

namespace test {
	class TestTexture : public Test
	{
	public:
		TestTexture();
		~TestTexture();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		unsigned int VBO, VAO, EBO, texture;
		std::unique_ptr<Shader> shader;
	};
}