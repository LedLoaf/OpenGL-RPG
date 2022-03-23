#pragma once
#include "Components/SystemComponent.h"
#include "Components/TransformComponent.h"

#include "Game.h"

namespace System
{
/* Sets up camera position with respect to the target its following, such that it is within the center of the screen, however if object is out of boundary camera will not follow */
	class CameraSystem : public Component::ISystem
	{
	public:
		CameraSystem(Component::Transform& follow, Component::Transform& camera)
			: m_follow{follow},
			  m_camera{camera}
		{
			Logger::message("Initializing Camera System");
		}

		void execute() override
		{
			// Set camera position so the follower is always at the center
			m_camera.x = m_follow.x - Game::Width / 2.f + m_follow.w * m_follow.scale / 2.f;
			m_camera.y = m_follow.y - Game::Height / 2.f + m_follow.h * m_follow.scale / 2.f;
									 
			// Boundary check for camera.
			// If reaches one of the lower or upper bounds make it so the camera doesn't follow past
			if (m_camera.x < 0)
				m_camera.x = 0;
			if (m_camera.y < 0)
				m_camera.y = 0;
			if (m_camera.x > m_camera.w * m_camera.scale - Game::Width)			// account for game screen
				m_camera.x = m_camera.w * m_camera.scale - Game::Width;
			if (m_camera.y > m_camera.h * m_camera.scale - Game::Height)		// account for game screen
				m_camera.y = m_camera.h * m_camera.scale - Game::Height;
		}

	private:
		Component::Transform& m_follow;
		Component::Transform& m_camera;
	};
}
