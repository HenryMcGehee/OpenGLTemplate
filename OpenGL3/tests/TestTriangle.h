#pragma once

#include "Test.h"
#include "../Shader.h"

namespace test {
	class TestTriangle : public Test
	{
	public:
		TestTriangle();
		~TestTriangle();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		unsigned int VBO, VAO;
		float color[4];
		std::unique_ptr<Shader> shader;
	};
}