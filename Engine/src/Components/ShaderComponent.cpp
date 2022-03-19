#include "ShaderComponent.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace Component
{
	void Shader::load(const GLchar* vsFilename, const GLchar* fsFilename)
	{
		m_vsFilename = vsFilename;
		m_fsFilename = fsFilename;

		std::string vsCode;
		std::string fsCode;
		Logger::message("Loading Shader [" + std::to_string(m_id) + "]" + "\t VertexShader: " + m_vsFilename + "\tFragmentShader: " + m_fsFilename);

		// Load shaders from file
		try {
			// Open file path
			std::ifstream     vsFile(m_vsFilename);
			std::stringstream vsStream;

			// Read into temp string streams
			vsStream << vsFile.rdbuf();

			// Close file
			vsFile.close();

			// Convert streams into strings
			vsCode = vsStream.str();
		} catch (std::exception& e) {
			Logger::error("Failed to read vertex shader: " + std::string(m_vsFilename) + " : " + std::string(e.what()), Logger::SEVERITY::LOW);
			exit(1);
		}

		try {
			// Open file path
			std::ifstream     fsFile(m_fsFilename);
			std::stringstream fsStream;

			// Read into temp string streams
			fsStream << fsFile.rdbuf();

			// Close file
			fsFile.close();

			// Convert streams into strings
			fsCode = fsStream.str();
		} catch (std::exception& e) {
			Logger::error("Failed to read fragment shader: " + std::string(m_vsFilename) + " : " + std::string(e.what()), Logger::SEVERITY::LOW);
			exit(1);
		}

		compile(vsCode.c_str(), fsCode.c_str());
	}

	void Shader::compile(const GLchar* vsData, const GLchar* fsData)
	{
		// Compile debug flags
		GLint  success;
		GLchar infoLog[512];

		// Vertex shader
		const GLuint vs = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vs, 1, &vsData, nullptr);
		glCompileShader(vs);
		// Fragment shader
		const GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fs, 1, &fsData, nullptr);
		glCompileShader(fs);


		// Check for vertex compile errors
		glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vs, 512, nullptr, infoLog);
			std::stringstream ss;
			ss << "Vertex shader compilation failure: " << vsData;
			ss << infoLog;
			Logger::error(ss.str(), Logger::SEVERITY::LOW);
			glDeleteShader(vs);
			glDeleteShader(fs);
		}

		// Check for fragment compile errors
		glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fs, 512, nullptr, infoLog);
			std::stringstream ss;
			ss << "Fragment shader compilation failure: " << vsData;
			ss << infoLog;
			Logger::error(ss.str(), Logger::SEVERITY::LOW);
			glDeleteShader(vs);
			glDeleteShader(fs);
		}

		// Shader program init
		m_id = glCreateProgram();
		glAttachShader(m_id, vs);
		glAttachShader(m_id, fs);
		glLinkProgram(m_id);

		// Linking errors
		glGetProgramiv(m_id, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(m_id, 512, nullptr, infoLog);
			std::stringstream ss;
			ss << "Program linking failure: (id = " << m_id << ") (VertexShader: " << vsData << ", FragmentShader : " << fsData << ")";
			ss << infoLog;
			Logger::error(ss.str(), Logger::SEVERITY::LOW);
			exit(1);
		}

		// Delete shader files
		glDeleteShader(vs);
		glDeleteShader(fs);
	}

	void Shader::use()
	{
		// Use program for drawing
		glUseProgram(m_id);
	}

	GLuint Shader::getID() const { return m_id; }
	
	GLint Component::Shader::get_attrib_location(const GLchar* attribName)
	{
		return glGetAttribLocation(m_id, attribName);
	}

	void Component::Shader::setBool(const GLchar* name, const GLboolean value)
	{
		glUniform1i(glGetUniformLocation(m_id, name), value);
	}

	void Component::Shader::setInt(const GLchar* name, const GLint value)
	{
		glUniform1i(glGetUniformLocation(m_id, name), value);
	}

	void Component::Shader::setFloat(const GLchar* name, const GLfloat value)
	{
		glUniform1f(glGetUniformLocation(m_id, name), value);
	}

	void Component::Shader::setVec2f(const GLchar* name, const glm::vec2& value)
	{
		glUniform2fv(glGetUniformLocation(m_id, name), 1, &value[0]);
	}

	void Component::Shader::setVec2f(const GLchar* name, const GLfloat x, const GLfloat y)
	{
		glUniform2f(glGetUniformLocation(m_id, name), x, y);
	}

	void Component::Shader::setVec3f(const GLchar* name, const glm::vec3& value)
	{
		glUniform3fv(glGetUniformLocation(m_id, name), 1, &value[0]);
	}

	void Component::Shader::setVec3f(const GLchar* name, const GLfloat x, const GLfloat y, const GLfloat z)
	{
		glUniform3f(glGetUniformLocation(m_id, name), x, y, z);
	}

	void Component::Shader::setVec4f(const GLchar* name, const glm::vec4& value)
	{
		glUniform4fv(glGetUniformLocation(m_id, name), 1, &value[0]);
	}

	auto Component::Shader::setVec4f(const GLchar* name,
									  const GLfloat x,
									  const GLfloat y, const GLfloat z, const GLfloat w) -> void
	{
		glUniform4f(glGetUniformLocation(m_id, name), x, y, z, w);
	}

	void Component::Shader::setMat2(const GLchar* name, const glm::mat2& matrix)
	{
		glUniformMatrix2fv(glGetUniformLocation(m_id, name), 1, GL_FALSE, &matrix[0][0]);
	}

	void Component::Shader::setMat3(const GLchar* name, const glm::mat3& matrix)
	{
		glUniformMatrix3fv(glGetUniformLocation(m_id, name), 1, GL_FALSE, &matrix[0][0]);
	}

	void Component::Shader::setMat4(const GLchar* name, const glm::mat4& matrix)
	{
		glUniformMatrix4fv(glGetUniformLocation(m_id, name), 1, GL_FALSE, &matrix[0][0]);
	}

}
