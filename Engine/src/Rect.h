#pragma once
#include <glad/glad.h>
#include <glm/vec2.hpp>

/* Defines a rectangle (x,y,w,h) */
struct Rect
{
	Rect(const Rect& rect)
		: x(rect.x), y(rect.y), w(rect.w), h(rect.h)
	{
	}

	Rect()
		: x(0.0f), y(0.0f), w(0.0f), h(0.0f)
	{
	}

	Rect(GLfloat x, GLfloat y, GLfloat width, GLfloat height)
		: x(x), y(y), w(width), h(height)
	{
	}

	Rect(GLfloat x, GLfloat y, GLfloat length)
		: x(x), y(y), w(length), h(length)
	{
	}

	Rect(glm::vec2 pos, GLfloat length)
		: x(pos.x), y(pos.y), w(length), h(length)
	{
	}

	Rect(glm::vec2 pos)
		: x(pos.x), y(pos.y), w(0.0f), h(0.0f)
	{
	}

	Rect& operator=(const Rect& rect)
	{
		set(rect);
	}

	Rect& operator=(Rect&& rect) noexcept
	{
		set(rect);
	}

	Rect& operator*(float scaler)
	{
		this->w *= scaler;
		this->h *= scaler;
		return *this;
	}

	void set(float x, float y, float width, float height)
	{
		this->x = x;
		this->y = y;
		this->w = width;
		this->h = height;
	}

	void set(const Rect& rect)
	{
		set(rect.x, rect.y, rect.w, rect.h);
	}

	void set(Rect&& rect)
	{
		set(rect.x, rect.y, rect.w, rect.h);
	}

	float x;
	float y;
	float w;
	float h;
};