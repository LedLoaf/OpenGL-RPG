#pragma once
#include "Components/BaseComponent.h"
#include "Rect.h"

namespace Component
{
	/* Stores all components used to render a rect on the window */
	struct Render : public IComponent
	{
		Render(const Rect& src, const Rect& dest)
			: src(src),
			  dest(dest) { }

		Render(const Rect& src)
			: src(src),
			  dest{} {}

		Rect src, // src position for image
			 dest; // actual position on the screen
	};
}
