#pragma once

// Forward declaration
namespace sf
{
class Drawable;
class RenderStates;
}

namespace mw
{

namespace Rendering
{
enum Layer
{
	Background,
	Midground,
	Forground,
	Default,
	Bullet,
	Turret,
	UI,
	LayerCount,
};
}

struct RenderInfo
{
	explicit RenderInfo(const ::sf::Drawable& drawable,
						const ::sf::RenderStates& states = ::sf::RenderStates::Default)
		: drawable(&drawable)
		, states(states)
	{
	}

	const ::sf::Drawable* drawable = nullptr;
	::sf::RenderStates states;
};



}