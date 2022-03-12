#pragma once
#include <glad/glad.h>

class Texture
{
public:
	Texture() : Texture(GL_RGBA, GL_RGBA, GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST) {}

	Texture(GLint imageFormat)
		: Texture(imageFormat, imageFormat, GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST)
	{
	}


	Texture(GLint internalFormat, GLint imageFormat, GLint wrapS, GLint wrapT, GLint filterMin, GLint filterMag)
		:
		internalFormat{ internalFormat }
		, imageFormat{ imageFormat }
		, wrap_s{ wrapS }
		, wrap_t{ wrapT }
		, filterMin{ filterMin }
		, filterMag{ filterMag }
		, m_id{ 0 }
	{
	
	}

	~Texture()
	{
		if(m_id) 
			glDeleteTextures(1, &m_id);
		m_id = 0;
	}

	void load(const char* fileName);

	void bind();

	GLuint getId() const { return m_id; }

public:
	// Image dimensions
	GLint width{}, height{};

	// Texture format
	GLint internalFormat{};
	GLint imageFormat{};

	// Texture configurations
	GLint wrap_s{};
	GLint wrap_t{};
	GLint filterMin{};
	GLint filterMag{};
private:
	GLuint m_id{};
};

