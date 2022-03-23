#include "AABB.h"

bool AABB::collide(const Component::Rectangle& rectA, const Component::Rectangle& rectB)
{
	return (rectA.x < rectB.x + rectB.w
		    && rectA.x + rectA.w > rectB.x)
			&& (rectA.y < rectB.y + rectB.h
			&& rectA.y + rectA.h > rectB.y);
}

bool AABB::collide(const glm::vec2 point, const Component::Rectangle& rectB)
{
	return (point.x < rectB.x + rectB.w
		    && point.x > rectB.x)
			&& (point.y < rectB.y + rectB.h
			&& point.y > rectB.y);
}
