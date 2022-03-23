#pragma once
#include "Components/ControllerComponent.h"
#include <GLFW/glfw3.h>
#include "Game.h"

namespace ControllerComponent
{
	/*
		Keyboard implementation for controller events
		keydown(): returns if the corresponding key is in the down position
		keyup(): returns if the corresponding key is in the up position
		keyPress(): returns if the corresponding key switched from the down position to the up position
		keyRelease(): returns if the corresponding key switched from the up position to the down position
	*/
	class Keyboard : public Component::IController
	{
		enum KeyID
		{
			Left = GLFW_KEY_LEFT,
			Right = GLFW_KEY_RIGHT,
			Down = GLFW_KEY_DOWN,
			Up = GLFW_KEY_UP,
			W = GLFW_KEY_W,
			A = GLFW_KEY_A,
			S = GLFW_KEY_S,
			D = GLFW_KEY_D,
		};

	public:
		Keyboard() = default;

		bool keyDownUp() override
		{
			return Game::keys[Up] || Game::keys[W];
		}

		bool keyDownDown() override
		{
			return Game::keys[Down] || Game::keys[S];
		}

		bool keyDownLeft() override
		{
			return Game::keys[Left] || Game::keys[A];
		}

		bool keyDownRight() override
		{
			return Game::keys[Right] || Game::keys[D];
		}
	};
}
