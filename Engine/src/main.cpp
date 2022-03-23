// ReSharper disable CppClangTidyBugproneIntegerDivision
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <GLFW/glfw3.h>

#include <glad/glad.h>

#include <glm/ext/matrix_clip_space.hpp>

#include "Entity.h"
#include "Game.h"
#include "Logger.h"

#include "Components/KeyboardComponent.h"
#include "Components/MaterialComponent.h"
#include "Components/RectComponent.h"
#include "Components/RendererComponent.h"
#include "Components/ShaderComponent.h"
#include "Components/TextureComponent.h"
#include "Components/TransformComponent.h"

#include "Systems/AnimationSystem.h"
#include "Systems/CameraSystem.h"
#include "Systems/MoveSystem.h"
#include "Systems/RenderSystem.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// GLOBAL VARIABLES
/////////////////////////////////////////////////////////////////////////////////////////////////////////

// Global variables
constexpr int SCREEN_WIDTH  = 1280;
constexpr int SCREEN_HEIGHT = 768;

// Global game variables
constexpr auto   SPEED       = 4.0f;
constexpr GLuint MAX_SPRITES = 255;
constexpr GLint  ROWS        = 32;
constexpr GLint  COLS        = 32;

Rect SRC{0.0f, 0.0f, 64.0f, 64.0f};

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// GLOBAL FUNCTION DEFINITIONS
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void keyCallback(GLFWwindow* window, int key, int scanCode, int action, int mode);

void processInput(GLFWwindow* window, Component::Transform& transform);		// process input for the player
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

void APIENTRY glDebugOutput(GLenum       source,
							GLenum       type,
							unsigned int id,
							GLenum       severity,
							GLsizei      length,
							const char*  message,
							const void*  userParam);

int main()
{
	// Logger::toFile(); // save logger to file

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// INITIALIZATION
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	Logger::message("Starting Application (Initializing)");

	if (!glfwInit()) {
		Logger::error("Failed to initialize GLFW", Logger::SEVERITY::HIGH);
		std::cerr << "Failed to initialize GLFW!\n";
		return -1;
	}

	// OpenGL version = major.minor (4.6)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	// Allow debug message early
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

	// Use the deprecated functions? (CORE_PROFILE = yes, COMPATIBILITY = no)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	const auto window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "OpenGL RPG", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Make the window resizable and scale the renderer
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	// set up so keyboard can interact with window
	glfwSetKeyCallback(window, keyCallback);

	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
		Logger::error("Failed to initialize Glad...", Logger::SEVERITY::HIGH);
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
	std::vector<Component::ISystem*> renderSystems;
	std::vector<Component::ISystem*> updateSystems;

	// Set up entities and their components

	// Shader filepath's
	constexpr auto vsFilepath = "Resources/Shaders/sprite.vs";
	constexpr auto fsFilepath = "Resources/Shaders/sprite.fs";

	// Shader Entity
	const auto shaders          = new Entity();
	auto&      shaderComponent = *shaders->addComponent<Component::Shader>();
	// Load in used shaders
	shaderComponent.load(vsFilepath, fsFilepath);

	// Set up camera
	// Use program for drawing
	shaderComponent.use();
	const auto projection = glm::ortho(0.0f, Game::Width, Game::Height, 0.0f, -1.0f, 1.0f);
	shaderComponent.setMat4("projection", projection);

	// Texture filepath's
	const auto fleshPath = "Resources/Images/flesh_full.png";
	const auto grassPath = "Resources/Images/grass.png";

	auto* textures = new Entity();

	auto& fleshTexture = *textures->push_back<Component::Texture>();
	fleshTexture.load(fleshPath);

	auto& grassTexture = *textures->push_back<Component::Texture>();
	grassTexture.load(grassPath);

	// Create a renderer object and input appropriate attribute sizes (2 = pos, 2 = coords)
	// Renderer Entity
	const auto renderer        = new Entity();
	auto&      renderComponent = *renderer->addComponent<Component::Renderer>(std::vector<GLuint>{2, 2}, MAX_SPRITES); // grass texture

	// Setup controller
	const auto  controller          = new Entity();
	auto& controllerComponent = *controller->addComponent<ControllerComponent::Keyboard>();

	// Setup camera entity
	const auto camera          = new Entity();
	auto&      cameraTransform = *camera->addComponent<Component::Transform>(0.f, 0.f, ROWS * Game::TileSize); // position = (0, 0) width/height = 32 tiles * 64 length of tile

	// Setup tile map
	const auto tileMap         = new Entity();
	auto&      tileMapMaterial = *tileMap->addComponent<Component::Material>(grassTexture, shaderComponent, 1); // grass texture

	// How many total grass textures to draw?
	constexpr auto totalTiles = COLS * ROWS;

	const auto tiles = new Entity();
	tileMap->push_back_child(tiles);

	// Setup tiles for tile map
	for (auto i = 0; i < totalTiles; ++i) {

		const float x = static_cast<float>((i % COLS)) * Game::TileSize; // finds place in column and multiplies by sprite width
		const float y = static_cast<float>((i / COLS)) * Game::TileSize; // finds place in row and multiplies by sprite height

		auto& grassDest = *tiles->push_back<Component::Dest>( x, y, Game::TileSize, Game::TileSize );
		auto& tileTransform = *tiles->push_back<Component::Transform>(grassDest);
		auto& src           = *tiles->push_back<Component::Src>(SRC);
		auto& dest          = *tiles->push_back<Component::Dest>();

		const auto tileDynamicDrawComp = tiles->push_back<ComponentSystemRender::DynamicDraw>(renderComponent, src,dest, tileMapMaterial, tileTransform, cameraTransform);
		renderSystems.push_back(tileDynamicDrawComp);
	}
	

	// Setup player and it's components
	const auto player = new Entity();

	auto&      playerTransform   = *player->addComponent<Component::Transform>(Game::Width, Game::Height, Game::TileSize);
	auto&      playerSrc         = *player->addComponent<Component::Src>(SRC); // src is full image, dest is set up during dynamic draw
	auto&      playerDest        = *player->addComponent<Component::Dest>();
	auto&      playerMaterial    = *player->addComponent<Component::Material>(fleshTexture, shaderComponent, 0);

	const auto playerDynamicDraw = player->addComponent<ComponentSystemRender::DynamicDraw>(renderComponent, playerSrc, playerDest, playerMaterial, playerTransform, cameraTransform);
	const auto playerCamera      = player->addComponent<System::CameraSystem>(playerTransform, cameraTransform);
	const auto playerMove        = player->addComponent<System::ControllerSystem>(playerTransform, controllerComponent);

	const auto playerAnimation   = player->addComponent<System::AnimationSystem>(4.f, playerSrc);
	const auto playerAnimateMove = player->addComponent<System::AnimateMoveSystem>(controllerComponent, *playerAnimation);

	// set up flesh animations
	const std::string anims[] = {
		"idle down",
		"idle up",
		"idle right",
		"idle left",
		"walk down",
		"walk up",
		"walk right",
		"walk left"
	};

	auto       animIdx    = 0u;
	const auto animation = new Entity();

	const auto fleshTexCols = fleshTexture.width / 64u;

	for (auto i = 0; i < 4; ++i) {
		Rect rect{
			static_cast<float>(animIdx % fleshTexCols) * Game::TileSize,
			static_cast<float>(animIdx / fleshTexCols) * Game::TileSize,
			Game::TileSize,
			Game::TileSize
		};

		const auto idle = animation->push_back<Component::Src>(rect);
		playerAnimation->add(anims[animIdx++], Anim{ idle });
	}

	for (auto i = 0; i < 4; ++i) {
		Rect rect{
			static_cast<float>((animIdx + i) % fleshTexCols) * Game::TileSize,
			static_cast<float>((animIdx + i) / fleshTexCols) * Game::TileSize,
			Game::TileSize,
			Game::TileSize
		};

		Rect rect2{
			static_cast<float>((animIdx + i + 1) % fleshTexCols) * Game::TileSize,
			static_cast<float>((animIdx + i + 1) / fleshTexCols) * Game::TileSize,
			Game::TileSize,
			Game::TileSize
		};

		const auto walk1 = animation->push_back<Component::Src>(rect);
		const auto walk2 = animation->push_back<Component::Src>(rect2);
		playerAnimation->add(anims[animIdx++], Anim{ walk1, walk2 });
	}

	renderSystems.push_back(playerDynamicDraw);
	updateSystems.push_back(playerMove);
	updateSystems.push_back(playerCamera);
	updateSystems.push_back(playerAnimateMove);
	updateSystems.push_back(playerAnimation);


	Logger::message("Entities Created: " + std::to_string(Entity::count));
	Logger::message("Components Created: " + std::to_string(IComponent::count));

	GLfloat lastFrame = 0.0f;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Game loop
	while (!glfwWindowShouldClose(window)) {
		// calculate delta time
		const auto currentFrame = static_cast<GLfloat>(glfwGetTime());
		Game::DeltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		/////////////////////////////////////////////////////////////////////////////////////////////////////////
		// INPUT
		/////////////////////////////////////////////////////////////////////////////////////////////////////////
		//processInput(window, playerTransform);

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
		renderComponent.clear();

		// Begin batch drawing
		renderComponent.beginDraw();

		// Make draw calls to renderer
		for (const auto draw : renderSystems) {
			draw->execute();
		}

		// End batch drawing
		renderComponent.endDraw();

		renderComponent.display();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	// CLEAN-UP
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	glfwTerminate();
	// delete entities and their components
	delete player;
	delete camera;
	delete tileMap;
	delete shaders;
	delete textures;
	delete renderer;
	delete controller;
	delete animation;

	if (Entity::count) {
		std::cerr << "Entity Memory Leak: " << Entity::count << std::endl;
		return -1;
	}

	if (IComponent::count) {
		std::cerr << "Component Memory Leak: " << IComponent::count << std::endl;
		return -1;
	}

	Logger::message("Closing application...");

	return 0;
}

void keyCallback(GLFWwindow* window, int key, int scanCode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < MAX_KEYS) {
		// update our global keyboard object
		if (action == GLFW_PRESS)
			Game::keys[key] = GL_TRUE;
		else if (action == GLFW_RELEASE)
			Game::keys[key] = GL_FALSE;
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
// GLOBAL FUNCTION IMPLEMENTATIONS
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void processInput(GLFWwindow* window, Component::Transform& transform)
{
	auto& rect = transform;

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
	ss << "OpenGL error (" << id << "): " << message << "\n";
	Logger::error(ss.str(), Logger::SEVERITY::HIGH);

	switch (source) {
		case GL_DEBUG_SOURCE_API: ss << "SOURCE: API";
			break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM: ss << "SOURCE: Window System";
			break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER: ss << "SOURCE: Shader Compiler";
			break;
		case GL_DEBUG_SOURCE_THIRD_PARTY: ss << "SOURCE: Third Party";
			break;
		case GL_DEBUG_SOURCE_APPLICATION: ss << "SOURCE: Application";
			break;
		case GL_DEBUG_SOURCE_OTHER: ss << "SOURCE: Other";
			break;
		default: break;
	}
	ss << "\n";

	switch (type) {
		case GL_DEBUG_TYPE_ERROR: ss << "TYPE: Error";
			break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: ss << "TYPE: Deprecated Behaviour";
			break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: ss << "TYPE: Undefined Behaviour";
			break;
		case GL_DEBUG_TYPE_PORTABILITY: ss << "TYPE: Portability";
			break;
		case GL_DEBUG_TYPE_PERFORMANCE: ss << "TYPE: Performance";
			break;
		case GL_DEBUG_TYPE_MARKER: ss << "TYPE: Marker";
			break;
		case GL_DEBUG_TYPE_PUSH_GROUP: ss << "TYPE: Push Group";
			break;
		case GL_DEBUG_TYPE_POP_GROUP: ss << "TYPE: Pop Group";
			break;
		case GL_DEBUG_TYPE_OTHER: ss << "TYPE: Other";
			break;
		default: break;
	}
	ss << "\n";

	switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH: ss << "GL SEVERITY: high";
			break;
		case GL_DEBUG_SEVERITY_MEDIUM: ss << "GL SEVERITY: medium";
			break;
		case GL_DEBUG_SEVERITY_LOW: ss << "GL SEVERITY: low";
			break;
		case GL_DEBUG_SEVERITY_NOTIFICATION: ss << "GL SEVERITY: notification";
			break;
		default: break;
	}

	Logger::error(ss.str(), Logger::SEVERITY::HIGH);
}
