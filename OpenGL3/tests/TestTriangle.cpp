#include "TestTriangle.h"
#include "../ErrorCheck.h"
#include "imgui/imgui.h"

namespace test {
	TestTriangle::TestTriangle()
		: color{ 1.0f, 0.5f, 0.2f, 1.0f }
	{

		float vertices[] = {
		-0.5f, -0.5f, 0.0f, // left  
		 0.5f, -0.5f, 0.0f, // right 
		 0.0f,  0.5f, 0.0f  // top   
		};

		shader = std::make_unique<Shader>("res/shaders/UnlitColor.shader");
		shader->Bind();

		GLCall(glGenVertexArrays(1, &VAO));
		GLCall(glGenBuffers(1, &VBO));

		GLCall(glBindVertexArray(VAO));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
		GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
		GLCall(glEnableVertexAttribArray(0));
	}
	TestTriangle::~TestTriangle()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}
	void TestTriangle::OnUpdate(float deltaTime)
	{
		
	}
	void TestTriangle::OnRender()
	{
		GLCall(glClearColor(0.5f, 0.5f, 0.5f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		shader->Bind();
		shader->SetUniform4f("color", color[0], color[1], color[2], color[3]);
		GLCall(glBindVertexArray(VAO));
		GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));
	}
	void TestTriangle::OnImGuiRender()
	{
		ImGui::ColorEdit4("Color", color);
	}
}