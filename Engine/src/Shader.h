#pragma once
#include <glad/glad.h>
#include <glm/mat4x4.hpp>

class Shader
{
public:
	Shader() = default;

	~Shader()
	{
		if (m_id)
			glDeleteProgram(m_id);
		m_id = 0;
	}

	// Loader vertex and fragment shaders from file name and compile
	void load(const GLchar* vsFilename, const GLchar* fsFilename);

	// Sets the shader to active
	void use();

public:
	// Uniform sets
	void setInt(const GLchar* name, int value);
	void setMat4(const GLchar* name, glm::mat4 value);

private:
	// Compiles and executes debug errors if found
	void compile(const GLchar* vsData, const GLchar* fsData);
private:
	GLuint        m_id{};
	const GLchar* m_vsFilename{};
	const GLchar* m_fsFilename{};

};

