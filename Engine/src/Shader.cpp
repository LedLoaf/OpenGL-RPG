#include "Shader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

void Shader::load(const GLchar* vsFilename, const GLchar* fsFilename)
{
	m_vsFilename = vsFilename;
	m_fsFilename = fsFilename;

	std::string vsCode;
	std::string fsCode;

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
		std::cerr << "Failed to read vertex shader: " << m_vsFilename << " : " << e.what() << "\n";
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
		std::cerr << "Failed to read fragment shader: " << m_fsFilename << " : " << e.what() << "\n";
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

	// Check for vertex compile errors
	glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vs, 512, nullptr, infoLog);
		std::cerr << "Vertex shader compilation failure: " << vsData << "\n";
	}

	// Fragment shader
	const GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fsData, nullptr);
	glCompileShader(fs);

	// Check for fragment compile errors
	glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fs, 512, nullptr, infoLog);
		std::cerr << "Fragment shader compilation failure: " << fsData << "\n";
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
		std::cerr << "Program linking failure: (id = " << m_id << ") (VertexShader: " << vsData << ", FragmentShader : " << fsData << ")" << "\n";
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

void Shader::setInt(const GLchar* name, const int value)
{
	glUniform1i(glGetUniformLocation(m_id, name), value);
}

void Shader::setMat4(const GLchar* name, glm::mat4 value)
{
	glUniformMatrix4fv(glGetUniformLocation(m_id, name), 1,GL_FALSE, &value[0][0]);
}
