#pragma once
#include "Shader.h"
#include "Texture.h"

// Abstract holding class for texture shader combinations used for rendering
class Material
{
public:
	Material(Texture& texture, Shader& shader, const GLint texUnit)
		: texture{texture},
		  shader{shader},
		  id{m_idGen++},
		  m_texIndex{texUnit} { }

	~Material() = default;

	// Takes and uses shader and sets up texture unit
	void compile();

	void bind();

public:
	Texture& texture;
	Shader&  shader;
	GLuint   id;
private:
	static GLuint m_idGen;
	GLint         m_texIndex;
};
