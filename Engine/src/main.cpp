#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"

#include <glad/glad.h> 
#include <GLFW/glfw3.h> // ReSharper disable once CppWrongIncludesOrder
#include <glm/ext/matrix_clip_space.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 768;

constexpr auto vsFilepath = "Resources/Shaders/sprite.vs";
constexpr auto fsFilepath = "Resources/Shaders/sprite.fs";
constexpr auto fleshFilepath = "Resources/Images/flesh_child_full_1.png";

void processInput(GLFWwindow* window);
Rect dest = { SCREEN_WIDTH/2 - 32, SCREEN_HEIGHT/2 - 32, 64, 64 };
Rect src = { 0,0,1,1 };
constexpr auto SPEED = 4.0f;

int main()
{
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW!\n";
		return -1;
	}

	// OpenGL version = major.minor (4.6)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	// Use the deprecated functions? (CORE_PROFILE = yes, COMPATIBILITY = no)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	const auto window = glfwCreateWindow(SCREEN_WIDTH,SCREEN_HEIGHT, "OpenGL RPG", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize Glad!" << std::endl;
		return -1;
	}
	auto shader = Shader();
	shader.load(vsFilepath, fsFilepath);

	auto texture = Texture();
	texture.load(fleshFilepath);

	// Use program for drawing
	shader.use();

	// Set up the tex unit to the correct value
	shader.setInt("image", 0);

	// Set up 2d camera
	const auto projection = glm::ortho(0.0f, static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT), 0.0f, -1.0f, 1.0f);
	shader.setMat4("projection", projection);

	// Bind texture for drawing
	texture.bind();

	// Create a renderer object and input appropriate attribute sizes (2 = pos, 2 = coords)
	auto renderer = Renderer({ 2, 2 });

	// game loop
	while (!glfwWindowShouldClose(window)) {

		processInput(window);

		renderer.clear();

		//renderer.draw(dest,src);
		renderer.draw(dest, src);
		renderer.display();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window,GLFW_KEY_W))
		dest.y -= SPEED;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S))
		dest.y += SPEED;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D))
		dest.x += SPEED;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A))
		dest.x -= SPEED;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
