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

	Rect(const float x, const float y, const float w, const float h)
		: x(x), y(y), w(w), h(h)
	{
	}

	Rect(const float x, const float y, const float length)
		: x(x), y(y), w(length), h(length)
	{
	}

	Rect(const glm::vec2 pos, const float length)
		: x(pos.x), y(pos.y), w(length), h(length)
	{
	}

	Rect(const glm::vec2 pos)
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

	Rect& operator*(float scale)
	{
		this->w *= scale;
		this->h *= scale;
		return *this;
	}

	void set(const float x, const float y, const float w, const float h)
	{
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;
	}

	void set(const Rect& rect)
	{
		set(rect.x, rect.y, rect.w, rect.h);
	}

	void set(Rect&& rect)
	{
		set(rect.x, rect.y, rect.w, rect.h);
	}

	float x, y, w, h;
};