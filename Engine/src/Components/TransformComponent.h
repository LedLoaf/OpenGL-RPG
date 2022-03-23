#pragma once
#include "Components/BaseComponent.h"
#include "Rect.h"


namespace Component
{
	/* Locally bound transformational information. Not where the entity is located but rather located relative to an invisible plane */
	class Transform : public IComponent, public Rect
	{
	public:
		Transform(const Rect& rect, const float scale)
			: Rect(rect), scale(scale)
		{
		}

		Transform(const Rect& rect)
			: Transform(rect, 1.0f)
		{
		}

		Transform(const float x, const float y, const float length)
			: Transform(Rect{ x, y, length, length }, 1.0f)
		{
		}

		Transform(const float x, const float y, const float length, const float scale)
			: Transform(Rect{ x, y, length, length }, scale)
		{
		}

		float scale;
	};
}


