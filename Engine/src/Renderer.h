#pragma once
#include <glad/glad.h>

#include <vector>

#include <glm/vec4.hpp>

#define Rect glm::vec4 // { x, y, z = width, w = height }

class Renderer
{
public:
	Renderer(const std::vector<GLint>& attributes);
	Renderer(const Renderer&) = delete;
	Renderer(Renderer&& other);

	Renderer& operator=(const Renderer&) = delete;
	Renderer& operator=(Renderer&& other);

	~Renderer() { release(); }

	void draw(const Rect& dest, const Rect& src);

	void display();

	void clear(float r = 0, float g = 0, float b = 0, float a = 255);

	void release();

private:
	GLuint               m_vbo{0};
	GLuint               m_vao{0};
	GLuint               m_attribSize{};
	std::vector<GLfloat> m_buffer{};
};
