#include "TextureComponent.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>

namespace Component
{
	void Texture::load(const char* fileName)
	{
		// Load image properties
		int nrChannels;
		int imgWidth, imgHeight;

		//	stbi_set_flip_vertically_on_load(true);
		const auto image = stbi_load(fileName, &imgWidth, &imgHeight, &nrChannels, STBI_rgb_alpha);		// store image data

		// set dimensions
		this->width  = imgWidth;
		this->height = imgHeight;

		if (!image) {
			std::cerr << "Failed to load texture: " << fileName << "\n";
			throw;
		}
		else {
			std::cout << "Loading Texture [" << m_id << "] " << width << "x" << height << " at location : \"" << fileName << "\" \n";
		}

		glBindTexture(GL_TEXTURE_2D, m_id);

		// Create our texture parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMin);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMag);

		// Create image in OpenGL
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, imageFormat, GL_UNSIGNED_BYTE, image);

		// Unbind texture
		glBindTexture(GL_TEXTURE_2D, 0);

		// Free image data
		stbi_image_free(image);
	}

	void Texture::bind()
	{
		glBindTexture(GL_TEXTURE_2D, m_id);
	}
}
