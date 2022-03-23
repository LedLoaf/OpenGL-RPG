#pragma once
#include "Components/BaseComponent.h"

namespace Component
{
	class IController : public IComponent
	{
	public:
		virtual bool keyDownLeft() = 0;
		virtual bool keyDownRight() = 0;
		virtual bool keyDownDown() = 0;
		virtual bool keyDownUp() = 0;
	};
}