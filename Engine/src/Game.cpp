#include "Game.h"

// Game global variables
float Game::width = 1280;
float Game::height = 768;
GLfloat Game::deltaTime = 0.0f;

Entity* Game::global = new Entity();

bool Game::exit = false;

glm::vec2 Game::removed(Game::width * -2.0f - 1.0f, Game::height * -2.0f - 1.0f);

// EC reference counting
long long IComponent::count = 0ll;
long long Entity::count = 0ll;

void Game::init(Entity* game)
{
	//auto& ctigs_overworld = *game->add_id_ct_input<Component::Trigger::Input::GameState::Overworld>("overworld");
	//ctigs_overworld.execute(game);

	//Game::global->add_id_component<Component::KeyboardArray>("keyboard");
}