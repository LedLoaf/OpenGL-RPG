#pragma once
#include "Components/BaseComponent.h"
#include "Components/MaterialComponent.h"

#include <glad/glad.h>

#include <vector>

#include "Rect.h"

namespace Component
{
	/* Simple batch renderer for drawing sprites from the same image and shader */
	class Renderer final : public IComponent
	{
	public:
		Renderer(const std::vector<GLint>& attributes, GLuint maxSprites);

		Renderer(const Renderer&) = delete;

		Renderer(Renderer&& other) noexcept;

		Renderer& operator=(const Renderer&) = delete;

		Renderer& operator=(Renderer&& other) noexcept;

		~Renderer() override { release(); }

		void release();

		void clear(float r = 0, float g = 0, float b = 0, float a = 255);

		void draw(const Rect& dest, const Rect& src, Material& mat);

		void display();

		// For batch renderer
		void beginDraw();

		void endDraw();

	private:
		void flush();

	private:
		GLuint               m_vbo{0};
		GLuint               m_vao{0};
		GLuint               m_attribSize{};
		GLuint               m_maxSprites{};
		std::vector<GLfloat> m_buffer{};
		Material*            m_currentMaterial{nullptr};
	};
}
