#include "Renderer.h"

constexpr auto VERTICES = 6u;

Renderer::Renderer(const std::vector<GLint>& attributes, const GLuint maxSprites)
	: m_maxSprites(maxSprites)
{
	// Calculate total attribute size	
	for (const auto attrib : attributes) {
		m_attribSize += attrib;
	}

	// Create buffers in GPU
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);

	// Bind buffers
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	// Create and bind attributes to vbo
	auto ptrStride = 0ull;
	for (auto i = 0u; i < attributes.size(); ++i) {
		glVertexAttribPointer(i, attributes[i], GL_FLOAT, GL_FALSE, static_cast<GLsizei>(m_attribSize * sizeof(float)), reinterpret_cast<GLvoid*>(ptrStride));
		glEnableVertexAttribArray(i);
		ptrStride += attributes[i] * sizeof(float);
	}
}


Renderer::Renderer(Renderer&& other) noexcept
	: m_vbo(other.m_vbo),
	  m_vao(other.m_vao)
{
	// Make the assigning renderer useless
	other.m_vbo = 0;
	other.m_vao = 0;
}

Renderer& Renderer::operator=(Renderer&& other) noexcept
{
	// ALWAYS check for self-assignment
	if (this != &other) {
		release();
		m_vbo       = other.m_vbo;
		m_vao       = other.m_vao;
		m_currentMaterial = other.m_currentMaterial;
		m_maxSprites = other.m_maxSprites;
		other.m_vbo = 0;
		other.m_vao = 0;

	}

	return *this;
}

void Renderer::draw(const Rectf& dest, const Rectf& src, Material& mat)
{
	// Checks if buffer is over sprite limit or current material isn't set
	// Finally checks if the current material has a different id from the new material
	if ((m_buffer.size() >= static_cast<std::size_t>(m_maxSprites) * m_maxSprites * VERTICES || !m_currentMaterial)
		|| m_currentMaterial->id != mat.id)
	{
		// Flush out current batch and start on the next one
		flush();
		m_currentMaterial = &mat;
	}

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
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(m_buffer.size() * sizeof(float)), m_buffer.data(), GL_STATIC_DRAW);

	// Draw triangles
	glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_buffer.size() / m_attribSize));

	// Clear buffer for next cycle
	m_buffer.clear();
}

void Renderer::clear(const float r, const float g, const float b, const float a)
{
	// Clear screen to black
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

void Renderer::flush()
{
	if (m_buffer.empty()) return;

	// Make sure the current batch is clear and ready to used
	if (!m_currentMaterial) {
		m_buffer.clear();
		return;
	}

	// Set uniforms and use shader
	m_currentMaterial->compile();

	// Bind texture to appropriate slot
	m_currentMaterial->bind();

	// Re-buffer changes to data
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(m_buffer.size() * sizeof(GLfloat)), m_buffer.data(), GL_STATIC_DRAW);

	// Draw triangles
	glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_buffer.size() / m_attribSize));

	// Clear buffer for next cycle
	m_buffer.clear();
}

void Renderer::beginDraw()
{
	// Make sure we aren't batching with previous material
	m_currentMaterial = nullptr;
}

void Renderer::endDraw()
{
	// Flush everything out
	flush();
}
