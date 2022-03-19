// ReSharper disable CppClangTidyBugproneIntegerDivision
#include "Components/MaterialComponent.h"
#include "Components/RenderComponent.h"
#include "Components/RendererComponent.h"
#include "Components/ShaderComponent.h"
#include "Components/TextureComponent.h"
#include "Components/TransformationComponent.h"

#include "Systems/CameraSystem.h"
#include "Systems/RenderSystem.h"

#include "Entity.h"
#include "Game.h"
#include "Logger.h"

#include <GLFW/glfw3.h>
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
constexpr GLuint   MAX_SPRITES = 255;
constexpr GLint ROWS = 32;
constexpr GLint COLS = 32;

Rect SRC{ 0.0f,0.0f,64.0f,64.0f };

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// GLOBAL FUNCTION DEFINITIONS
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void processInput(GLFWwindow* window, Component::Transform& transform);		// process input for the player
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity,
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

	int flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);

	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}

	// Normalize window to work on other devices
	glViewport(0, 0, static_cast<int>(Game::Width), static_cast<int>(Game::Height));

	// Set up alpha channel to display images beneath it.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Set up engine, will be its own thing soon enough
	std::vector<Component::System*> renderSystems;
	std::vector<Component::System*> updateSystems;

	// Set up entities and their components

	// Shader filepath's
	constexpr auto vsFilepath = "Resources/Shaders/sprite.vs";
	constexpr auto fsFilepath = "Resources/Shaders/sprite.fs";

	// Shader Entity
	auto shader = Component::Shader();
	// Load in used shaders
	shader.load(vsFilepath, fsFilepath);

	// Set up camera
	// Use program for drawing
	shader.use();
	const auto projection = glm::ortho(0.0f, Game::Width, Game::Height, 0.0f, -1.0f, 1.0f);
	shader.setMat4("projection", projection);

	// Texture filepath's
	const auto fleshPath = "Resources/Images/flesh_child_full_1.png";
	const auto grassPath = "Resources/Images/grass.png";

	auto fleshTexture = Component::Texture();
	fleshTexture.load(fleshPath);

	auto grassTexture = Component::Texture();
	grassTexture.load(grassPath);

	// Create a renderer object and input appropriate attribute sizes (2 = pos, 2 = coords)
	// Renderer Entity
	auto  renderer = Component::Renderer({ 2, 2 }, MAX_SPRITES);

	// Setup camera entity
	const auto camera = new Entity();
	auto& cameraTransform = *camera->addComponent<Component::Transform>(0.f, 0.f, ROWS * Game::TileSize); // position = (0, 0) width/height = 32 tiles * 64 length of tile

	// Setup tile map
	const auto tileMap = new Entity();
	auto& tileMapMaterial = *tileMap->addComponent<Component::Material>(grassTexture, shader, 1); // grass texture

	// How many columns of grass?
	constexpr auto grassCols = COLS;
	// How many rows of grass
	constexpr auto grassRows = ROWS;
	// How many total grass textures to draw?
	constexpr auto totalTiles = grassCols * grassRows;

	const auto tiles = new Entity();
	tileMap->push_back_child(tiles);

	// Setup tiles for tile map
	for (auto i = 0; i < totalTiles; ++i) {

		const float x = static_cast<float>((i % grassCols)) * Game::TileSize; // finds place in column and multiplies by sprite width
		const float y = static_cast<float>((i / grassCols)) * Game::TileSize; // finds place in row and multiplies by sprite height

		Rect       grassDest{ x, y, Game::TileSize, Game::TileSize };
		auto&      tileTransform    = *tiles->push_back<Component::Transform>(grassDest);
		auto&      tileRender       = *tiles->push_back<Component::Render>(SRC);
		const auto tileDynamicDrawComp = tiles->push_back<ComponentSystemRender::DynamicDraw>(renderer, tileRender, tileMapMaterial, tileTransform, cameraTransform);
		renderSystems.push_back(tileDynamicDrawComp);
	}

	// Setup player and it's components
	const auto player = new Entity();


	auto&      playerTransform   = *player->addComponent<Component::Transform>((GLfloat)Game::Width, (GLfloat)Game::Height, 64.0f);
	auto&      playerRender      = *player->addComponent<Component::Render>(SRC); // src is full image, dest is set up during dynamic draw
	auto&      playerMaterial    = *player->addComponent<Component::Material>(fleshTexture, shader, 0);
	const auto playerDynamicDraw = player->addComponent<ComponentSystemRender::DynamicDraw>(renderer, playerRender, playerMaterial, playerTransform, cameraTransform);
	const auto playerCamera      = player->addComponent<ComponentSystemUpdate::Camera>(playerTransform, cameraTransform);


	renderSystems.push_back(playerDynamicDraw);
	updateSystems.push_back(playerCamera);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	
	// Game loop
	while (!glfwWindowShouldClose(window)) {
		/////////////////////////////////////////////////////////////////////////////////////////////////////////
		// INPUT
		/////////////////////////////////////////////////////////////////////////////////////////////////////////
		processInput(window,playerTransform);

		/////////////////////////////////////////////////////////////////////////////////////////////////////////
		// UPDATE
		/////////////////////////////////////////////////////////////////////////////////////////////////////////

		// Make updates to live entities
		for (const auto update : updateSystems) {
			update->execute();
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////////
		// DRAW
		/////////////////////////////////////////////////////////////////////////////////////////////////////////

		// Clears screen to black
		renderer.clear();

		// Begin batch drawing
		renderer.beginDraw();
	

		// Make draw calls to renderer
		for (const auto draw : renderSystems) {
			draw->execute();
		}
		
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
	delete player;
	delete camera;
	delete tileMap;
	std::cout << "Closing Application (CleanUp)\n";
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// GLOBAL FUNCTION IMPLEMENTATIONS
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void processInput(GLFWwindow* window, Component::Transform& transform)
{
	auto& rect = transform.rect;

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS ||
		glfwGetKey(window,GLFW_KEY_W))
		rect.y -= SPEED;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_S))
		rect.y += SPEED;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_D))
		rect.x += SPEED;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS ||
		glfwGetKey(window, GLFW_KEY_A))
		rect.x -= SPEED;
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
}

void framebufferSizeCallback(GLFWwindow* window, const int width, const int height)
{
	glViewport(0, 0, width, height);
}

void APIENTRY glDebugOutput(const GLenum       source,
							const GLenum       type,
							const unsigned int id,
							const GLenum       severity,
							GLsizei            length,
							const char*        message,
							const void*        userParam)
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
		default: break;
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
		default: break;
	} ss << std::endl;

	switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH:         ss << "GL SEVERITY: high"; break;
		case GL_DEBUG_SEVERITY_MEDIUM:       ss << "GL SEVERITY: medium"; break;
		case GL_DEBUG_SEVERITY_LOW:          ss << "GL SEVERITY: low"; break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: ss << "GL SEVERITY: notification"; break;
		default: break;
	}

	Logger::error(ss.str(), Logger::SEVERITY::HIGH);
}