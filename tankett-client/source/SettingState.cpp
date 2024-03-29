#include "SettingState.h"
#include "Context.h"
#include "ClientStateStack.h"
namespace client
{

SettingState::SettingState()
	: MenuTemplate<Setting::Option>(Setting::OptionNames, static_cast<int>(Setting::Option::COUNT))
{
	mBackgroundShape.setFillColor(::sf::Color(0, 0, 0, 255));
	mBackgroundShape.setSize(::sf::Vector2f(getRenderWindow().getSize()));

	mCreditText.setFont(*Context::getInstance().fontManager->get(Font::Sansation));
	mCreditText.setString("WIP");
	mCreditText.setFillColor(::sf::Color::Blue);
	mCreditText.setCharacterSize(50);
	helper::Graphics::centreOrigin(mCreditText);
	::sf::Vector2u windowSize(getRenderWindow().getSize());
	mCreditText.setPosition((float)windowSize.x / 2,
		(float)windowSize.y / 2);
}

void SettingState::draw()
{
	getRenderWindow().setView(getRenderWindow().getDefaultView());

	getRenderWindow().draw(mBackgroundShape);
	getRenderWindow().draw(mCreditText);

	MenuTemplate<Setting::Option>::draw();
}

void SettingState::handleConfirmInput()
{
	Context::getInstance().stack->popState();
}
}