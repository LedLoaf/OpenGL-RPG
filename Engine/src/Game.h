#pragma once
#include <glad/glad.h>

#include <glm/vec2.hpp>

#include "Entity.h"

class Game
{
public:
	Game() = delete;

	static float width;
	static float height;
	static GLfloat deltaTime;
	static Entity* global;
	static bool exit;
	static glm::vec2 removed;

	static void init(Entity* game);

};

