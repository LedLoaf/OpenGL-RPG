#pragma once
#include "Entity.h"

#include <glad/glad.h>

#include <glm/vec2.hpp>
constexpr auto MAX_KEYS = 1024u;


class Game
{
public:
	Game() = delete;

	static float Width;
	static float Height;

	static float TileSize;

	static float DeltaTime;
	static Entity* Global;
	static bool Exit;
	static glm::vec2 Removed;

	static void init(Entity* game);

	static std::array<bool, MAX_KEYS> keys;

};

