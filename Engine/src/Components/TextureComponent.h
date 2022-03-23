#pragma once
#include <iostream>
#include <string>

#include "Components/BaseComponent.h"

#include <glad/glad.h>

#include "Logger.h"

namespace Component
{
	/* Image handling class that's responsible for loading and binding textures to OpenGL texture slots. */
	class Texture final : public IComponent
	{
	public:
		Texture()
			: Texture(GL_RGBA, GL_RGBA, GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST) {}

		explicit Texture(const int imageFormat)
			: Texture(imageFormat, imageFormat, GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST) { }

		Texture(const int internalFormat, const int imageFormat, const int wrapS, const int wrapT, const int filterMin, const int filterMag)
			: internalFormat{internalFormat},
			  imageFormat{imageFormat},
			  wrapS{wrapS},
			  wrapT{wrapT},
			  filterMin{filterMin},
			  filterMag{filterMag},
			  m_id{0}
		{
			glGenTextures(1, &m_id);
		}

		~Texture() override
		{
			Logger::message("Destroying Texture [" + std::to_string(m_id) + "]");

			if (m_id)
				glDeleteTextures(1, &m_id);
			m_id = 0;
		}

		void load(const char* fileName);

		void bind();

		unsigned int getId() const { return m_id; }

	public:
		// Image dimensions
		int width{}, height{};

		// Texture format
		int internalFormat{};
		int imageFormat{};

		// Texture configurations
		int wrapS{};
		int wrapT{};
		int filterMin{};
		int filterMag{};
	private:
		unsigned int m_id{};
	};
}
