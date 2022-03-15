#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h> // ReSharper disable once CppWrongIncludesOrder
#include <glm/ext/matrix_clip_space.hpp>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Material.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// GLOBAL VARIABLES
/////////////////////////////////////////////////////////////////////////////////////////////////////////

// Global variables
constexpr int SCREEN_WIDTH  = 1280;
constexpr int SCREEN_HEIGHT = 768;

// Global game variables
constexpr auto  SPEED       = 4.0f;
constexpr float TILE_SIZE   = 64.f;
constexpr int   MAX_SPRITES = 255;

// Shader filepath
constexpr auto vsFilepath = "Resources/Shaders/sprite.vs";
constexpr auto fsFilepath = "Resources/Shaders/sprite.fs";
// Texture filepath
constexpr auto fleshPath = "Resources/Images/flesh_child_full_1.png";
constexpr auto grassPath = "Resources/Images/grass.png";

Rectf fleshDest  = {5, 5, TILE_SIZE, TILE_SIZE};
Rectf fleshDest2 = {SCREEN_WIDTH - 69, SCREEN_HEIGHT - 69, TILE_SIZE, TILE_SIZE};
//Rect dest = { SCREEN_WIDTH/2 - 32, SCREEN_HEIGHT/2 - 32, 64, 64 };
Rectf src = {0, 0, 1, 1};

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// GLOBAL FUNCTION DEFINITIONS
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void processInput(GLFWwindow* window);		// process input for the player
void update(const float deltaTime);			// update logic

int main()
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// INITIALIZATION
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW!\n";
		return -1;
	}

	// OpenGL version = major.minor (4.6)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	// Use the deprecated functions? (CORE_PROFILE = yes, COMPATIBILITY = no)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	const auto window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL RPG", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
		std::cerr << "Failed to initialize Glad!" << std::endl;
		return -1;
	}

	// Normalize window to work on other devices
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// Set up alpha channel to display images beneath it.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Load in used shaders
	auto shader = Shader();
	shader.load(vsFilepath, fsFilepath);

	auto fleshTexture = Texture();
	fleshTexture.load(fleshPath);

	auto grassTexture = Texture();
	grassTexture.load(grassPath);

	// Use program for drawing
	shader.use();

	// Set up 2d camera
	const auto projection = glm::ortho(0.0f, static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT), 0.0f, -1.0f, 1.0f);
	shader.setMat4("projection", projection);

	// Setup materials
	auto fleshMat = Material(fleshTexture, shader, 0);		// flesh texture
	auto grassMat = Material(grassTexture, shader, 0);		// grass texture

	// How many columns of grass?
	const auto grassCols = static_cast<int>(std::ceilf(SCREEN_WIDTH / TILE_SIZE));
	// How many rows of grass
	const auto grassRows = static_cast<int>(std::ceilf(SCREEN_HEIGHT / TILE_SIZE));
	// How many total grass textures to draw?
	const auto totalTiles = grassCols * grassRows;

	// Create a renderer object and input appropriate attribute sizes (2 = pos, 2 = coords)
	auto renderer = Renderer({2, 2}, MAX_SPRITES);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Game loop
	while (!glfwWindowShouldClose(window)) {
		/////////////////////////////////////////////////////////////////////////////////////////////////////////
		// INPUT
		/////////////////////////////////////////////////////////////////////////////////////////////////////////
		processInput(window);

		/////////////////////////////////////////////////////////////////////////////////////////////////////////
		// UPDATE
		/////////////////////////////////////////////////////////////////////////////////////////////////////////
		update(0.016f);

		/////////////////////////////////////////////////////////////////////////////////////////////////////////
		// DRAW
		/////////////////////////////////////////////////////////////////////////////////////////////////////////

		// Clears screen to black
		renderer.clear();

		// Begin batch drawing
		renderer.beginDraw();

		// Draw Grass
		for (auto i = 0; i < totalTiles; ++i) {

			const float x = static_cast<float>((i % grassCols)) * TILE_SIZE; // finds place in column and multiplies by sprite width
			const float y = static_cast<float>((i / grassCols)) * TILE_SIZE; // finds place in row and multiplies by sprite height

			Rectf grassDest{x, y, TILE_SIZE, TILE_SIZE};
			renderer.draw(grassDest, src, grassMat);
		}

		// Draw flesh children
		renderer.draw(fleshDest, src, fleshMat);
		renderer.draw(fleshDest2, src, fleshMat);

		// End batch drawing
		renderer.endDraw();

		renderer.display();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// CLEAN-UP
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	glfwTerminate();

	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// GLOBAL FUNCTION IMPLEMENTATIONS
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS ||
		glfwGetKey(window,GLFW_KEY_W))
		fleshDest.y -= SPEED;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_S))
		fleshDest.y += SPEED;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_D))
		fleshDest.x += SPEED;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_A))
		fleshDest.x -= SPEED;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
}

void update(const float deltaTime)
{
	auto dt = deltaTime;
	/* Empty Function */
}
