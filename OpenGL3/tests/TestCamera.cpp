#include "TestCamera.h"
#include "../ErrorCheck.h"
#include "stb_image.h"
#include <GLFW/glfw3.h>
#include "imgui/imgui.h"

namespace test {
	TestCamera::TestCamera()
		: modelScale(10.0f, 0.2f, 10.0f), fov(45.0f), cameraPosition(0.0f, -2.0f, -15.0f), cameraRotation(8.0f, 0.0f, 0.0f)
	{
		float vertices[] = {
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};

		shader = std::make_unique<Shader>("res/shaders/ModelViewMatrix.shader");
		shader->Bind();

		// Handle Cube
		{
			GLCall(glGenVertexArrays(1, &VAO));
			GLCall(glGenBuffers(1, &VBO));

			GLCall(glBindVertexArray(VAO));
			GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
			GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

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

		cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);

		glEnable(GL_DEPTH_TEST);
	}
	TestCamera::~TestCamera()
	{
		GLCall(glDeleteVertexArrays(1, &VAO));
		GLCall(glDeleteBuffers(1, &VBO));
	}
	void TestCamera::OnUpdate(float deltaTime)
	{
	}
	void TestCamera::OnRender()
	{
		GLCall(glClearColor(0.5f, 0.5f, 0.5f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		shader->Bind();

		GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
		GLCall(glBindVertexArray(VAO));

		// Model
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(modelPosition.x, modelPosition.y, modelPosition.z));

			model = glm::rotate(model, glm::radians(modelRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
			model = glm::rotate(model, glm::radians(modelRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::rotate(model, glm::radians(modelRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

			model = glm::scale(model, modelScale);

			unsigned int modelLoc = shader->GetUniformLocation("model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		}

		// View
		{
			view = glm::mat4(1.0f);
			view = glm::translate(view, cameraPosition);
			
			view = glm::rotate(view, glm::radians(cameraRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
			view = glm::rotate(view, glm::radians(cameraRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
			view = glm::rotate(view, glm::radians(cameraRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

			unsigned int viewlLoc = shader->GetUniformLocation("view");
			glUniformMatrix4fv(viewlLoc, 1, GL_FALSE, glm::value_ptr(view));
		}

		// Projection
		{
			projection = glm::perspective(glm::radians(fov), 960.0f / 540.0f, 0.1f, 100.0f);
			unsigned int projectionLoc = shader->GetUniformLocation("projection");
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		}
	}
	void TestCamera::OnImGuiRender()
	{
		ImGui::Text("Model Transform");
		ImGui::DragFloat3("Position", (float*)&modelPosition, 0.1f);
		ImGui::DragFloat3("Rotation", (float*)&modelRotation);
		ImGui::DragFloat3("Scale", (float*)&modelScale, 0.1f);

		ImGui::Text("Camera FOV");
		ImGui::DragFloat("FOV", &fov);

		ImGui::Text("Camera Transform");
		ImGui::DragFloat3("CamPosition", (float*)&cameraPosition, 0.01f);
		ImGui::DragFloat3("CamRotation", (float*)&cameraRotation);
	}
}