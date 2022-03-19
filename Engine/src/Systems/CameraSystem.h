#pragma once
#include "Game.h"

#include "Components/SystemComponent.h"
#include "Components/TransformationComponent.h"

/* Sets up camera position with respect to the target its following, such that it is within the center of the screen, however if object is out of boundary camera will not follow */
namespace ComponentSystemUpdate
{
	class Camera : public Component::System
	{
	public:
		Camera(Component::Transform& follow, Component::Transform& camera)
			: m_follow{follow},
			  m_camera{camera}
		{
			Logger::message("Initializing Camera System");
		}

		void execute() override
		{
			// Set camera position so the follower is always at the center
			m_camera.rect.x = m_follow.rect.x - Game::Width / 2.f + m_follow.rect.w * m_follow.scale / 2.f;
			m_camera.rect.y = m_follow.rect.y - Game::Height / 2.f + m_follow.rect.h * m_follow.scale / 2.f;
									 
			// Boundary check for camera.
			// If reaches one of the lower or upper bounds make it so the camera doesn't follow past
			if (m_camera.rect.x < 0)
				m_camera.rect.x = 0;
			if (m_camera.rect.y < 0)
				m_camera.rect.y = 0;
			if (m_camera.rect.x > m_camera.rect.w * m_camera.scale - Game::Width)		// account for game screen
				m_camera.rect.x = m_camera.rect.w * m_camera.scale - Game::Width;
			if (m_camera.rect.y > m_camera.rect.h * m_camera.scale - Game::Height)		// account for game screen
				m_camera.rect.y = m_camera.rect.h * m_camera.scale - Game::Height;
		}

	private:
		Component::Transform& m_follow;
		Component::Transform& m_camera;
	};
}
