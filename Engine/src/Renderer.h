#pragma once
#include <glad/glad.h>

#include <vector>

#include <glm/vec4.hpp>

#include "Material.h"

#define Rectf glm::vec4 // { x, y, z = width, w = height }

// Simple batch renderer for drawing sprites from the same image and shader
class Renderer
{
public:
	Renderer(const std::vector<GLint>& attributes, GLuint maxSprites);

	Renderer(const Renderer&) = delete;

	Renderer(Renderer&& other) noexcept;

	Renderer& operator=(const Renderer&) = delete;

	Renderer& operator=(Renderer&& other) noexcept;

	~Renderer() { release(); }

	void draw(const Rectf& dest, const Rectf& src, Material& mat);

	void display();

	void clear(float r = 0, float g = 0, float b = 0, float a = 255);

	void release();

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
