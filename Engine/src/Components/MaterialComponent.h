#pragma once
#include <string>

#include "BaseComponent.h"
#include "Logger.h"

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
			Logger::message("Initializing Material: TexIndex: " + std::to_string(m_texIndex) + " with Texture[" + std::to_string(this->texture.getId()) + "] and Shader[" + std::to_string(this->shader.getID()) + "]");

		}

		~Material() override
		{
			Logger::message("Destroying Material: TexIndex: " + std::to_string(m_texIndex) + " with Texture[" + std::to_string(this->texture.getId()) + "] and Shader[" + std::to_string(this->shader.getID()) + "]");

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