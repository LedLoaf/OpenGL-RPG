#pragma once
#include "Components/RectComponent.h"
#include "Components/RendererComponent.h"
#include "Components/SystemComponent.h"
#include "Components/TransformComponent.h"

namespace ComponentSystemRender
{
	/* Draw sprites on screen with respect to where the camera is located */
	class DynamicDraw : public Component::ISystem
	{
	public:
		DynamicDraw(Component::Renderer&  renderer,
					Component::Rectangle& src,
					Component::Rectangle& dest,
					Component::Material&  material,
					Component::Transform& transform,
					Component::Transform& cameraTransform)
			: m_renderer(renderer),
				m_dest(dest),
				m_src(src),
			  m_material(material),
			  m_transform(transform),
			  m_camTransform(cameraTransform)
		{
			//Logger::message("Initializing Dynamic Draw System");
		}

		void execute() override
		{
			Component::Dest destination;
			destination.x = m_dest.x;
			destination.y = m_dest.y;
			destination.w = m_dest.w;
			destination.h = m_dest.h;

			// Update render dest by camera and local transforms
			destination.x = m_transform.x - m_camTransform.x;
			destination.y = m_transform.y - m_camTransform.y;
			destination.w = m_transform.w * m_transform.scale;
			destination.h = m_transform.h * m_transform.scale;

			m_renderer.draw(m_src, destination, m_material);
		}

	private:
		Component::Renderer&  m_renderer;
		Component::Dest&      m_dest;
		Component::Src&       m_src;
		Component::Material&  m_material;
		Component::Transform& m_transform;		// local transform
		Component::Transform& m_camTransform;
	};
}
