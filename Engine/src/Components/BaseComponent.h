#pragma once

/* Stores generics and data structures for systems to manipulate and display on screen */
class IComponent
{
public:
	static long long count; // count component references

	IComponent() { ++count; }
	virtual ~IComponent() { --count; } // make sure if overloaded to call ~Comp()

	// Delete all functions that could possibly copy one component onto another
	//IComponent(const IComponent&) = delete;
	//IComponent(IComponent&&) = delete;
	//IComponent& operator=(IComponent&) = delete;
	//IComponent& operator=(IComponent&&) = delete;
};