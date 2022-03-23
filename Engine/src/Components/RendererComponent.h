#pragma once
#include "Components/BaseComponent.h"
#include "Components/MaterialComponent.h"
#include "Components/RectComponent.h"

#include <glad/glad.h>

#include <vector>

namespace Component
{
	/* Simple batch renderer for drawing sprites from the same image and shader */
	class Renderer final : public IComponent
	{
	public:

		Renderer(const std::vector<unsigned int>& attributes, unsigned int maxSprites);

		Renderer(const Renderer&) = delete;

		Renderer(Renderer&& other) noexcept;

		Renderer& operator=(const Renderer&) = delete;

		Renderer& operator=(Renderer&& other) noexcept;

		~Renderer() override { release(); }

		void release();

		void clear(float r = 0, float g = 0, float b = 0, float a = 255);

		void draw(const Rect& src, const Rect& dest, Component::Material& mat);

		void display();

		// For batch renderer
		void beginDraw();

		void endDraw();

	private:
		void flush();

	private:
		unsigned int               m_vbo{0};
		unsigned int               m_vao{0};
		unsigned int               m_attribSize{};
		unsigned int               m_maxSprites{};
		std::vector<float> m_buffer{};
		Component::Material*            m_currentMaterial{nullptr};
	};
}
