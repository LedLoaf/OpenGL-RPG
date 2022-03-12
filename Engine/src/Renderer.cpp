#include "Renderer.h"

Renderer::Renderer(const std::vector<GLint>& attributes)
	: m_attribSize(0)
{
	// Calculate total attribute size	

	for (const auto attrib : attributes) {
		m_attribSize += attrib;
	}

	// Create storage in GPU
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	glBindVertexArray(m_vao);

	// Bind and input buffer data
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	auto ptrStride = 0ull;
	for (auto i = 0u; i < attributes.size(); ++i) {
		glVertexAttribPointer(i, attributes[i], GL_FLOAT, GL_FALSE, m_attribSize * sizeof(float), reinterpret_cast<GLvoid*>(ptrStride));
		glEnableVertexAttribArray(i);
		ptrStride += attributes[i] * sizeof(float);
	}
}


Renderer::Renderer(Renderer&& other)
	: m_vbo(other.m_vbo),
	  m_vao(other.m_vao)
{
	other.m_vbo = 0;
	other.m_vao = 0;
}

Renderer& Renderer::operator=(Renderer&& other)
{
	//ALWAYS check for self-assignment
	if (this != &other) {
		release();
		m_vbo       = other.m_vbo;
		m_vao       = other.m_vao;
		other.m_vbo = 0;
		other.m_vao = 0;
	}

	return *this;
}

void Renderer::draw(const Rect& dest, const Rect& src)
{
	// Set up vertex data and create a quad using 2 triangles

	// First triangle
	// Bottom Left
	m_buffer.push_back(dest.x);
	m_buffer.push_back(dest.y + dest.w);
	m_buffer.push_back(src.x);
	m_buffer.push_back(src.y + src.w);

	//  Top Right
	m_buffer.push_back(dest.x + dest.z);
	m_buffer.push_back(dest.y);
	m_buffer.push_back(src.x + src.z);
	m_buffer.push_back(src.y);

	// Top Left
	m_buffer.push_back(dest.x);
	m_buffer.push_back(dest.y);
	m_buffer.push_back(src.x);
	m_buffer.push_back(src.y);

	// Second Triangle
	// Bottom Left
	m_buffer.push_back(dest.x);
	m_buffer.push_back(dest.y + dest.w);
	m_buffer.push_back(src.x);
	m_buffer.push_back(src.y + src.w);

	// Bottom Right
	m_buffer.push_back(dest.x + dest.z);
	m_buffer.push_back(dest.y + dest.w);
	m_buffer.push_back(src.x + src.z);
	m_buffer.push_back(src.y + src.w);

	// Top Right
	m_buffer.push_back(dest.x + dest.z);
	m_buffer.push_back(dest.y);
	m_buffer.push_back(src.x + src.z);
	m_buffer.push_back(src.y);
}

void Renderer::display()
{
	// Re-buffer changes to data
	glBufferData(GL_ARRAY_BUFFER, m_buffer.size() * sizeof(float), m_buffer.data(), GL_STATIC_DRAW);

	// Draw triangles
	glDrawArrays(GL_TRIANGLES, 0, m_buffer.size() / m_attribSize);

	// Clear buffer for next cycle
	m_buffer.clear();
}

void Renderer::clear(const float r, const float g, const float b, const float a)
{
	// clear screen to black
	glClearColor(r / 255, g / 255, b / 255, a / 255);

	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::release()
{
	// Delete buffers if they exist
	if (m_vbo)
		glDeleteBuffers(1, &m_vbo);
	if (m_vao)
		glDeleteBuffers(1, &m_vao);
}
