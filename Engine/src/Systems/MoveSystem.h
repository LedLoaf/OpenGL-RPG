#pragma once
#include "Components/ControllerComponent.h"
#include "Components/SystemComponent.h"
#include "Components/TransformComponent.h"

namespace System
{
	constexpr auto ONE_OVER_SQRT_TWO = 0.70710678118f;
	constexpr GLfloat SPEED = 4.0f;

	/* Updates transform position on the game world */
	class ControllerSystem : public Component::ISystem
	{
		Component::Transform& m_transform;
		Component::IController& m_controller;
	public:

		ControllerSystem(Component::Transform& transform, Component::IController& controller)
			: m_transform(transform), m_controller(controller)
		{
		}

		void execute() override
		{
			GLfloat x = 0.0f, y = 0.0f;

			if (m_controller.keyDownRight()) x++;
			if (m_controller.keyDownLeft()) x--;
			if (m_controller.keyDownDown()) y++;
			if (m_controller.keyDownUp()) y--;

			if (!y && !x) {
				x *= ONE_OVER_SQRT_TWO;
				y *= ONE_OVER_SQRT_TWO;
			}

			m_transform.x += x * SPEED;
			m_transform.y += y * SPEED;
		}
	};
}