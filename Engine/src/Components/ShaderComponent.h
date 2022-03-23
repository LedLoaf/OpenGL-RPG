#pragma once
#include <iostream>
#include <string>

#include "Components/BaseComponent.h"

#include <glad/glad.h>
#include <glm/mat4x4.hpp>

#include "Logger.h"

namespace Component
{
	/* Loads, operates and updates to held shader id. */
	class Shader final : public IComponent
	{
	public:
		Shader() = default;

		~Shader() override
		{
			Logger::message("Destroying shader [" + std::to_string(m_id) + "]");

			if (m_id)
				glDeleteProgram(m_id);
			m_id = 0;
		}

		// Loader vertex and fragment shaders from file name and compile
		void load(const GLchar* vsFilename, const GLchar* fsFilename);

		// Sets the shader to active
		void use();

		GLuint getID() const;

	public:
		GLint get_attrib_location(const GLchar* attribName);

		// Uniform sets
		void setBool(const GLchar* name, GLboolean value);
		void setInt(const GLchar* name,  GLint value);
		void setFloat(const GLchar* name, GLfloat value);

		void setVec2f(const GLchar* name, const glm::vec2& value);
		void setVec2f(const GLchar* name, GLfloat x, GLfloat y);

		void setVec3f(const GLchar* name, const glm::vec3& value);
		void setVec3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z);

		void setVec4f(const GLchar* name, const glm::vec4& value);
		void setVec4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);

		void setMat2(const GLchar* name, const glm::mat2& matrix);
		void setMat3(const GLchar* name, const glm::mat3& matrix);
		void setMat4(const GLchar* name, const glm::mat4& matrix);
	private:
		// Compiles and executes debug errors if found
		void compile(const GLchar* vsData, const GLchar* fsData);

	private:
		GLuint        m_id{};
		const GLchar* m_vsFilename{};
		const GLchar* m_fsFilename{};
	};
}
