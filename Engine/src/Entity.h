#pragma once
#include "Logger.h"
#include "SplayTree.h"
#include "Components/BaseComponent.h"

#include "glad/glad.h"

#include <array>
#include <iostream>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>


using ComponentID = std::size_t;

/////////////////////////////////////////////////////////
//  Helper Functions
/////////////////////////////////////////////////////////

// Generates a new ID
inline ComponentID getNewComponentTypeID()
{
	static ComponentID id = 0u;
	return ++id;
}

// Depending on how many components there are creates a separate getComponentTypeID for each, thus giving the illusion of an unique id
template <typename T>
inline ComponentID getComponentTypeID() noexcept
{
	// Assert that it inherits BaseComponent
	static_assert(std::is_base_of_v<IComponent, T>);
	static ComponentID typeID = getNewComponentTypeID();
	return typeID;
}

/////////////////////////////////////////////////////////

/* Storage class for components and other entities beneath it can either id and retrieve components/children, or treat them as lists. */
class Entity
{
public:
	static long long count; // count entity references
public:
	Entity()
	{
		++count;
	}

	~Entity()
	{
		--count;
	}

	// delete all functions that could possibly copy one entity onto another
	Entity(const Entity&) = delete;
	Entity(Entity&&) = delete;
	Entity& operator=(const Entity&) = delete;
	Entity& operator=(Entity&&) = delete;

	Entity* getChild(const std::size_t position)
	{
		const auto rChild = m_children.search(position);

		if (!rChild)
			Logger::error("Could not find child at position = " + std::to_string(position), Logger::SEVERITY::HIGH);

		return rChild;
	}

	Entity* getChild(const std::string& id)
	{
		const auto hashed_str = std::hash<std::string>{}(id);
		const auto rChild    = m_children.search(hashed_str);

		if (!rChild)
			Logger::error("Could not find child at string id = " + id, Logger::SEVERITY::HIGH);

		return rChild;
	}

	std::vector<Entity*> getChildList()
	{
		return m_children.getOrderedList();
	}

	bool hasChild(const std::string& id)
	{
		const auto hashedStr = std::hash<std::string>{}(id);
		return m_children.search(hashedStr);
	}

	void removeChild(std::string id)
	{
		const auto hashedStr = std::hash<std::string>{}(id);
		m_children.remove(hashedStr);
	}

	std::size_t childrenSize()
	{
		return m_children.size();
	}

	// Inserts entity into child tree, and treats it like an array
	Entity* push_back_child(Entity* child)
	{
		return m_children.insert(child);
	}

	// Inserts entity into child tree with hash of string as unique id
	Entity* addIDChild(Entity* entity, const std::string& id)
	{
		const auto hashedStr = std::hash<std::string>{}(id);
		return m_children.insert(hashedStr, entity);
	}

	Entity* addIDChild(const std::string& id)
	{
		const auto       entity         = new Entity();
		const auto hashedStr = std::hash<std::string>{}(id);
		return m_children.insert(hashedStr, entity);
	}

	// inserts entity into child tree with unique id
	Entity* addIDChild(Entity* entity, const std::size_t id)
	{
		return m_children.insert(id, entity);
	}

	// Gets component of matching unique id, throws if not there
	template <typename T>
	T* getComponent()
	{
		auto r_comp = m_components.search(getComponentTypeID<T>());

		if (!r_comp)
			Logger::error("Could not find component, component id = " + std::to_string(getComponentTypeID<T>()), Logger::SEVERITY::HIGH);

		auto casted_r_comp = dynamic_cast<T*>(r_comp);

		if (!casted_r_comp)
			Logger::error("Component not of casted type", Logger::SEVERITY::HIGH);

		return casted_r_comp;
	}

	// Searches component tree for templated component at pos
	template <typename T>
	T* getComponent(const int position)
	{
		auto rBaseComponent = m_components.search(position);

		if (!rBaseComponent)
			Logger::error("Could not find component, position = " + std::to_string(position), Logger::SEVERITY::HIGH);

		auto castedRComp = dynamic_cast<T*>(rBaseComponent);

		if (!castedRComp)
			Logger::error("Component not of casted type, position = " + std::to_string(position), Logger::SEVERITY::HIGH);

		return castedRComp;
	}

	// searches component tree for specific component of hashed string
	template <typename T>
	T* get_component(const std::string id)
	{
		const auto hashedStr = std::hash<std::string>{}(id);
		auto rComp = m_components.search(hashedStr);

		if (!rComp)
			Logger::error("could not find component, str id = " + id, Logger::SEVERITY::HIGH);

		auto castedRComp = dynamic_cast<T*>(rComp);

		if (!castedRComp)
			Logger::error("component not of casted type, str id = " + id, Logger::SEVERITY::HIGH);


		return castedRComp;
	}


	// Checks if unique ID of component is located in component tree
	template <typename T>
	bool hasComponent()
	{
		return m_components.search(getComponentTypeID<T>());
	}

	// checks if string id of component is located in component tree
	template <typename T> bool hasComponent(const std::string id)
	{
		const auto hashedStr = std::hash<std::string>{}(id);
		return m_components.search(hashedStr);
	}

	// checks if id of component is located in component tree
	template <typename T> bool has_component(const std::size_t position)
	{
		return m_components.search(position);
	}

	// Adds component to splay tree and gives it a uniquely id, can't have two of the same component per entity
	template <typename T, typename... TArgs>
	T* addComponent(TArgs&&...args)
	{
		T* comp(new T(std::forward<TArgs>(args)...));
		return static_cast<T*>(m_components.insert(getComponentTypeID<T>(), comp));
	}

	// Adds component to splay tree using a hashed string
	template<typename T, typename... TArgs>
	T* addIDComponent(const std::string id, TArgs&&...args)
	{
		static_assert (std::is_base_of_v<IComponent, T>, "addIDComponent(std::string) T not a component");
		T* component(new T(std::forward<TArgs>(args)...));
		auto hashedStr = std::hash<std::string>{}(id);
		m_components.insert(hashedStr, component);
		return component;
	}

	// adds component to splay tree using a hashed string
	template<typename T, typename... TArgs>
	T* addIDComponent(std::size_t id, TArgs&&... args)
	{
		static_assert (std::is_base_of_v<IComponent, T>, "addIDComponent(std::size_t) T not a component");
		T* c(new T(std::forward<TArgs>(args)...));
		m_components.insert(id, c);
		return c;
	}

	// Adds a component to splay tree and treats it like an array, can have two of the same component in an entity
	template <typename T, typename... TArgs>
	T* push_back(TArgs&&...args)
	{
		static_assert (std::is_base_of_v<IComponent, T>, "push_back() not a component");
		T* c(new T(std::forward<TArgs>(args)...));
		return static_cast<T*>(m_components.insert(c));
	}

	// Gets the array of components
	template<typename T>
	std::vector<IComponent*> getComponentList()
	{
		std::vector<T*> tcompList;
		for (auto comp : m_components.getOrderedList())
			tcompList.push_back(dynamic_cast<T*>(comp));
		return tcompList;
	}

	std::size_t componentsSize()
	{
		return m_components.size();
	}

	// delete all components and children
	void clear()
	{
		m_children.clear();
		m_components.clear();
	}

	// delete all entity children
	void clearChildren()
	{
		m_children.clear();
	}

private:
	SplayTree<IComponent> m_components{};
	SplayTree<Entity>        m_children{};
};
