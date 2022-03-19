#pragma once
#include "BaseComponent.h"
#include "Rect.h"

namespace Component
{
/* Component implementation for rect data structure */
	class Rectangle : public Rect, public IComponent
	{
	public:
		Rectangle(const Rect& rect)
			: Rect(rect)
		{
		}
		Rectangle(const float x, const float y, const float w, const float h)
			: Rect(x, y, w, h)
		{
		}

		Rectangle()
			: Rect()
		{
		}
	};
#define Transform Rectangle
#define Src Rectangle
}