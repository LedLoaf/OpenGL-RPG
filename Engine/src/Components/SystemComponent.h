#pragma once
#include "Components/BaseComponent.h"

namespace Component
{
	/* Abstract class for executing updates on components and the window */
	class ISystem : public IComponent
	{
	public:
		virtual void execute() = 0;
	};
}