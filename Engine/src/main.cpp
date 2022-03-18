#include "Components/MaterialComponent.h"
#include "Components/RenderComponent.h"
#include "Components/ShaderComponent.h"
#include "Components/TextureComponent.h"

#include "Entity.h"
#include "Game.h"
#include "Logger.h"

#include <GLFW/glfw3.h> // ReSharper disable once CppWrongIncludesOrder
#include <glad/glad.h>
#include <glm/ext/matrix_clip_space.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

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
Rect  src = { 0.0f, 0.0f, TILE_SIZE, TILE_SIZE };

// Shader filepath
constexpr auto vsFilepath = "Resources/Shaders/sprite.vs";
constexpr auto fsFilepath = "Resources/Shaders/sprite.fs";
// Texture filepath
constexpr auto fleshPath = "Resources/Images/flesh_child_full_1.png";
constexpr auto grassPath = "Resources/Images/grass.png";

Rect fleshDest  = {5, 5, TILE_SIZE, TILE_SIZE};
Rect fleshDest2 = {SCREEN_WIDTH - 69, SCREEN_HEIGHT - 69, TILE_SIZE, TILE_SIZE};

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// GLOBAL FUNCTION DEFINITIONS
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void processInput(GLFWwindow* window);		// process input for the player
void update(const float deltaTime);			// update logic
void framebufferSizeCallback(GLFWwindow* window, const int width, const int height);
void APIENTRY gl_debug_output(GLenum source, GLenum type, unsigned int id, GLenum severity,
							  GLsizei length, const char* message, const void* userParam);
int main()
{
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// INITIALIZATION
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::cout << "Starting Application (Initializing)\n";

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

	// Make the window resizable and scale the renderer
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
		std::cerr << "Failed to initialize Glad!" << std::endl;
		return -1;
	}

	// Normalize window to work on other devices
	glViewport(0, 0, static_cast<int>(Game::width), static_cast<int>(Game::height));

	// Set up alpha channel to display images beneath it.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Load in used shaders
	auto shader = Component::Shader();
	shader.load(vsFilepath, fsFilepath);

	auto fleshTexture = Component::Texture();
	fleshTexture.load(fleshPath);

	auto grassTexture = Component::Texture();
	grassTexture.load(grassPath);

	// Use program for drawing
	shader.use();

	// Set up 2d camera
	const auto projection = glm::ortho(0.0f, Game::width, Game::height, 0.0f, -1.0f, 1.0f);
	shader.setMat4("projection", projection);

	// Setup materials
	auto fleshMat = Component::Material(fleshTexture, shader, 0);		// flesh texture
	auto grassMat = Component::Material(grassTexture, shader, 0);		// grass texture

	// How many columns of grass?
	const auto grassCols = static_cast<int>(std::ceilf(SCREEN_WIDTH / TILE_SIZE));
	// How many rows of grass
	const auto grassRows = static_cast<int>(std::ceilf(SCREEN_HEIGHT / TILE_SIZE));
	// How many total grass textures to draw?
	const auto totalTiles = grassCols * grassRows;

	// Create a renderer object and input appropriate attribute sizes (2 = pos, 2 = coords)
	auto renderer = Component::Renderer({2, 2}, MAX_SPRITES);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	//todo testing ec
	const auto player  = new Entity();
	auto&      matComp = *player->addComponent<Component::Material>(fleshTexture, shader, 0);

	const auto tileMap    = new Entity();
	auto&      mapMatComp = *tileMap->addComponent<Component::Material>(grassTexture, shader, 1);

	const auto tiles = new Entity();
	tileMap->push_back(tiles);

	assert(tileMap->childrenSize() == 1);
	assert(tileMap->componentsSize() == 1);
	assert(player->componentsSize() == 1);

	std::cout << "Starting Game Loop (Update)\n";

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

			Rect grassDest{x, y, TILE_SIZE, TILE_SIZE};
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
	std::cout << "Closing Application (CleanUp)\n";
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

void framebufferSizeCallback(GLFWwindow* window, const int width, const int height)
{
	glViewport(0, 0, width, height);
}

void APIENTRY gl_debug_output(GLenum source,
							  GLenum type,
							  unsigned int id,
							  GLenum severity,
							  GLsizei length,
							  const char* message,
							  const void* userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	std::stringstream ss;
	ss << "OpenGL error (" << id << "): " << message << std::endl;

	switch (source) {
		case GL_DEBUG_SOURCE_API:             ss << "SOURCE: API"; break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   ss << "SOURCE: Window System"; break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: ss << "SOURCE: Shader Compiler"; break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:     ss << "SOURCE: Third Party"; break;
		case GL_DEBUG_SOURCE_APPLICATION:     ss << "SOURCE: Application"; break;
		case GL_DEBUG_SOURCE_OTHER:           ss << "SOURCE: Other"; break;
	} ss << std::endl;

	switch (type) {
		case GL_DEBUG_TYPE_ERROR:               ss << "TYPE: Error"; break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: ss << "TYPE: Deprecated Behaviour"; break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  ss << "TYPE: Undefined Behaviour"; break;
		case GL_DEBUG_TYPE_PORTABILITY:         ss << "TYPE: Portability"; break;
		case GL_DEBUG_TYPE_PERFORMANCE:         ss << "TYPE: Performance"; break;
		case GL_DEBUG_TYPE_MARKER:              ss << "TYPE: Marker"; break;
		case GL_DEBUG_TYPE_PUSH_GROUP:          ss << "TYPE: Push Group"; break;
		case GL_DEBUG_TYPE_POP_GROUP:           ss << "TYPE: Pop Group"; break;
		case GL_DEBUG_TYPE_OTHER:               ss << "TYPE: Other"; break;
	} ss << std::endl;

	switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH:         ss << "GL SEVERITY: high"; break;
		case GL_DEBUG_SEVERITY_MEDIUM:       ss << "GL SEVERITY: medium"; break;
		case GL_DEBUG_SEVERITY_LOW:          ss << "GL SEVERITY: low"; break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: ss << "GL SEVERITY: notification"; break;
	}

	Logger::error(ss.str(), Logger::SEVERITY::HIGH);
}