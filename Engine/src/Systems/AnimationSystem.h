#pragma once
#include "Components/KeyboardComponent.h"
#include "Components/RectComponent.h"
#include "Components/SystemComponent.h"

#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

#include <string>
#include <unordered_map>
#include <vector>

using Anim = std::vector<Component::Src*>;

namespace System
{
	/* Update the next frame to be drawn of our sprite sheet to give the illusion of movement */
	class AnimationSystem : public Component::ISystem
	{
	public:
		AnimationSystem(const float speed, Component::Src& srcRect)
			: m_speed{speed},
			  m_src(srcRect) { }

		void execute() override
		{
			const auto time     = static_cast<float>(glfwGetTime());
			const auto anim_pos = static_cast<std::size_t>(time * m_speed) % m_current->size();
			const auto anim_src = m_current->at(anim_pos);

			m_src.x = anim_src->x;
			m_src.y = anim_src->y;
			m_src.w = anim_src->w;
			m_src.h = anim_src->h;
		}

		void add(const std::string& id, Anim anim)
		{
			m_animMap.emplace(id, anim);
			m_current = &anim;
		}

		// Change the textures source position
		void play(const std::string& id)
		{
			m_current = &m_animMap[id];
		}

	private:
		float								  m_speed;
		Component::Src&                       m_src;
		Anim*                                 m_current{nullptr};
		std::unordered_map<std::string, Anim> m_animMap{};
	};
}

namespace System
{
/* Animates object to play walk and idle animations on controller move events */
	class AnimateMoveSystem : public Component::ISystem
	{
		ControllerComponent::Keyboard&    m_controller;
		System::AnimationSystem& m_animation;
		glm::ivec2                        m_prev;
	public:
		AnimateMoveSystem(ControllerComponent::Keyboard& controller, System::AnimationSystem& animation)
			: m_controller(controller),
			  m_animation(animation),
			  m_prev{0, 1} {}

		void execute() override
		{
			const auto currDir = glm::ivec2{
				static_cast<int>(m_controller.keyDownRight()) - static_cast<int>(m_controller.keyDownLeft()),
				static_cast<int>(m_controller.keyDownDown()) - static_cast<int>(m_controller.keyDownUp())
			};

			if (currDir.x || currDir.y) {
				m_prev = currDir;
				if (currDir.x > 0) m_animation.play("walk right");
				else if (currDir.x < 0) m_animation.play("walk left");
				if (currDir.y > 0) m_animation.play("walk down");
				else if (currDir.y < 0) m_animation.play("walk up");
			}
			else {
				if (m_prev.x > 0) m_animation.play("idle right");
				else if (m_prev.x < 0) m_animation.play("idle left");
				if (m_prev.y > 0) m_animation.play("idle down");
				else if (m_prev.y < 0) m_animation.play("idle up");
			}
		}
	};
}
