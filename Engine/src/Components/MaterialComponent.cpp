#include "MaterialComponent.h"

namespace Component
{
	GLuint Material::m_idGen = 0u;

	void Material::compile()
	{
		// Set up the tex unit to the correct value
		shader.use();
		shader.setInt("image", m_texIndex);
	}

	void Material::bind()
	{
		// Activate defined tex unit and bind appropriate texture onto it
		glActiveTexture(GL_TEXTURE0 + m_texIndex);
		texture.bind();
	}
}