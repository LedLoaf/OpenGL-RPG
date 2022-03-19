#pragma once
#include "Entity.h"

#include <glad/glad.h>

#include <glm/vec2.hpp>


class Game
{
public:
	Game() = delete;

	static float Width;
	static float Height;

	static float TileSize;

	static GLfloat DeltaTime;
	static Entity* Global;
	static bool Exit;
	static glm::vec2 Removed;

	static void init(Entity* game);

};

