#pragma once
#include "Components/BaseComponent.h"

namespace Component
{
	/* Abstract class for executing updates on components and the window */
	class System : public IComponent
	{
	public:
		System() = default;
		virtual void execute() = 0;
	};
}