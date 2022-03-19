#pragma once
#include "Components/RectComponent.h"

/*
Axis-Aligned Bounding Box collision detection between two rectangles and a point and a rectangle
"One of the simpler forms of collision detection is between two rectangles that are axis aligned — meaning no rotation.
The algorithm works by ensuring there is no gap between any of the 4 sides of the rectangles. Any gap means a collision does not exist."
- MDN contributors
https://developer.mozilla.org/en-US/docs/Games/Techniques/2D_collision_detection
*/

namespace AABB
{
	bool collide(const Component::Rectangle& rectA, const Component::Rectangle& rectB);
	bool collide(glm::vec2 point, const Component::Rectangle& rectB);
}