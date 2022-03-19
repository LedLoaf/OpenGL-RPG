#pragma once
#include "Components/BaseComponent.h"
#include "Rect.h"


namespace Component
{
/* Locally bound transformational information. Not where the entity is located but rather located relative to an invisible plane */
	struct Transform : public IComponent
	{
		Transform(const Rect& rect, const GLfloat scale)
			: rect(rect), scale(scale)
		{
			//Logger::message("Initializing Transform component (" + std::to_string(rect.x) + ", " + std::to_string(rect.y) + ", " + std::to_string(rect.w) + ", " + std::to_string(rect.h) + ") " + "Scale: "+ std::to_string(this->scale));
		}

		Transform(const Rect& rect)
			: Transform(rect, 1.0f)
		{
		}

		Transform(const GLfloat x, const GLfloat y, const GLfloat length)
			: Transform(Rect{ x, y, length, length }, 1.0f)
		{
		}

		Rect rect;
		GLfloat scale;
	};
}


