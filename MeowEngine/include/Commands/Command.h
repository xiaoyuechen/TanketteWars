#pragma once
#include "CommandCategory.h"

#include <functional>
#include <cassert>

namespace mw
{
struct Command
{
	::std::function<void(class Actor&, float)> action;
	CommandCategory category = CommandCategory::None;
};

template<typename ActorType, typename Function>
::std::function<void(Actor&, float)> derivedAction(Function fn)
{
	return [=](Actor& node, float deltaSeconds)
	{
		auto ptr = dynamic_cast<ActorType*>(&node);
		assert(ptr);
		fn(*ptr, deltaSeconds);
	};
}
}