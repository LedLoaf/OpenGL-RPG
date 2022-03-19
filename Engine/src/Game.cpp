#include "Game.h"

// Game global variables
float Game::Width = 1280;
float Game::Height = 768;
float Game::TileSize = 64.f;

GLfloat Game::DeltaTime = 0.0f;

Entity* Game::Global = new Entity();

bool Game::Exit = false;

glm::vec2 Game::Removed(Game::Width * -2.0f - 1.0f, Game::Height * -2.0f - 1.0f);

// EC reference counting
long long IComponent::count = 0ll;
long long Entity::count = 0ll;

void Game::init(Entity* game)
{
	//auto& ctigs_overworld = *game->add_id_ct_input<Component::Trigger::Input::GameState::Overworld>("overworld");
	//ctigs_overworld.execute(game);

	//Game::global->add_id_component<Component::KeyboardArray>("keyboard");
}