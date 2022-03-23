#include "Game.h"
#include "Components/KeyboardComponent.h"
// Game global variables
float Game::Width = 1280;
float Game::Height = 768;
float Game::TileSize = 64.f;

float Game::DeltaTime = 0.0f;

Entity* Game::Global = new Entity();

bool Game::Exit = false;

glm::vec2 Game::Removed(Game::Width * -2.0f - 1.0f, Game::Height * -2.0f - 1.0f);

// EC reference counting
long long IComponent::count = 0ll;
long long Entity::count = 0ll;

std::array<bool, MAX_KEYS> Game::keys{ 0 };


void Game::init(Entity* game)
{


	Game::Global->addIDComponent<ControllerComponent::Keyboard>("keyboard");
}