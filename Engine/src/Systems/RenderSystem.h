#pragma once
#include "Components/RenderComponent.h"
#include "Components/RendererComponent.h"
#include "Components/SystemComponent.h"
#include "Components/TransformationComponent.h"

namespace ComponentSystemRender
{
	class DynamicDraw : public Component::System
	{
	public:
		DynamicDraw(Component::Renderer& renderer, Component::Render& render, Component::Material& material,
					Component::Transform& transform, Component::Transform& cameraTransform)
			: m_renderer(renderer), m_render(render), m_material(material), m_transform(transform), m_camTransform(cameraTransform)
		{
			//Logger::message("Initializing Dynamic Draw System");

		}

		void execute() override
		{
			// update render dest by camera and local transforms
			m_render.dest.x = m_transform.rect.x - m_camTransform.rect.x;
			m_render.dest.y = m_transform.rect.y - m_camTransform.rect.y;
			m_render.dest.w = m_transform.rect.w * m_transform.scale;
			m_render.dest.h = m_transform.rect.h * m_transform.scale;

			m_renderer.draw(m_render, m_material);
		}

	private:
		Component::Renderer&  m_renderer;
		Component::Render&    m_render;
		Component::Material&  m_material;
		Component::Transform& m_transform;		// local transform
		Component::Transform& m_camTransform;
	};
}
