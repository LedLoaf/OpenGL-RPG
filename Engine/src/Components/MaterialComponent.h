#pragma once
#include "BaseComponent.h"

#include "ShaderComponent.h"
#include "TextureComponent.h"

namespace Component
{
	/* Abstract holding class for texture shader combinations used for rendering */
	class Material final : public IComponent
	{
	public:
		Material(Texture& texture, Shader& shader, const GLint textureIndex)
			: texture{ texture },
			shader{ shader },
			id{ m_idGen++ },
			m_texIndex{ textureIndex }
		{
			std::cout << "Initializing Material: TexIndex: "<<m_texIndex<<" with Texture["<<this->texture.getId()<<"] and Shader["<<this->shader.getID()<<"]\n";

		}

		~Material() override
		{
			std::cout << "Destroying Material: TexIndex: " << m_texIndex << " with Texture[" << texture.getId() << "] and Shader[" << shader.getID() << "]\n";

		}

		// Takes and uses shader and sets up texture unit
		void compile();

		void bind();

	public:
		Texture& texture;
		Shader& shader;
		GLuint   id;
	private:
		static GLuint m_idGen;
		GLint         m_texIndex;
	};
}