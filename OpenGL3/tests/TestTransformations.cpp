#include "TestTransformations.h"
#include "../ErrorCheck.h"
#include "stb_image.h"
#include "imgui/imgui.h"
#include <GLFW/glfw3.h>

namespace test {
	TestTransformations::TestTransformations()
	{
		float vertices[] = {
			// positions       
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};
		unsigned int indices[] = {
			0, 1, 3, // first triangle
			1, 2, 3  // second triangle
		};

		shader = std::make_unique<Shader>("res/shaders/Translation.shader");
		shader->Bind();

		// Handle Rectangle
		{
			GLCall(glGenVertexArrays(1, &VAO));
			GLCall(glGenBuffers(1, &VBO));
			GLCall(glGenBuffers(1, &EBO));

			GLCall(glBindVertexArray(VAO));
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
			GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

			GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
			GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

			GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));
			GLCall(glEnableVertexAttribArray(0));

			GLCall(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))));
			GLCall(glEnableVertexAttribArray(1));
		}

		// Texture Setup
		{
			GLCall(glGenTextures(1, &texture));
			GLCall(glBindTexture(GL_TEXTURE_2D, texture)); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
			// set the texture wrapping parameters
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));	// set texture wrapping to GL_REPEAT (default wrapping method)
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
			// set texture filtering parameters
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
			// load image, create texture and generate mipmaps
			int width, height, nrChannels;
			stbi_set_flip_vertically_on_load(true);
			// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
			unsigned char* data = stbi_load("res/images/wall.jpg", &width, &height, &nrChannels, 0);
			if (data)
			{
				GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data));
				GLCall(glGenerateMipmap(GL_TEXTURE_2D));
			}
			else
			{
				std::cout << "Failed to load texture" << std::endl;
			}
			stbi_image_free(data);
		}

			
	}
	TestTransformations::~TestTransformations()
	{
		GLCall(glDeleteVertexArrays(1, &VAO));
		GLCall(glDeleteBuffers(1, &VBO));
	}
	void TestTransformations::OnUpdate(float deltaTime)
	{
	}
	void TestTransformations::OnRender()
	{
		GLCall(glClearColor(0.5f, 0.5f, 0.5f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
		shader->Bind();

		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
		GLCall(glBindVertexArray(VAO));

		trans = glm::mat4(1.0f);
		
		// Position
		trans = glm::translate(trans, glm::vec3(xPosition, yPosition, zPosition));

		// Rotation
		trans = glm::rotate(trans, (float)glfwGetTime() * xRotation, glm::vec3(1.0, 0.0, 0.0));
		trans = glm::rotate(trans, (float)glfwGetTime() * yRotation, glm::vec3(0.0, 1.0, 0.0));
		trans = glm::rotate(trans, (float)glfwGetTime() * zRotation, glm::vec3(0.0, 0.0, 1.0));
		
		// Scale
		trans = glm::scale(trans, glm::vec3(xScale, yScale, zScale));

		unsigned int transformLoc = shader->GetUniformLocation("transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
	}
	void TestTransformations::OnImGuiRender()
	{
		ImGui::SliderFloat("X Position", &xPosition, -1.0f, 1.0f);
		ImGui::SliderFloat("Y Position", &yPosition, -1.0f, 1.0f);
		// ImGui::SliderFloat("Z Position", &zPosition, -1.0f, 1.0f);

		ImGui::SliderFloat("X Rotation", &xRotation, -5.0f, 5.0f);
		ImGui::SliderFloat("Y Rotation", &yRotation, -5.0f, 5.0f);
		ImGui::SliderFloat("Z Rotation", &zRotation, -5.0f, 5.0f);

		ImGui::SliderFloat("X Scale", &xScale, 0.1f, 5.0f);
		ImGui::SliderFloat("Y Scale", &yScale, 0.1f, 5.0f);
		// ImGui::SliderFloat("Z Scale", &zScale, 0.1f, 5.0f);
	}
}