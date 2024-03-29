#include "TitleState.h"

#include "Helpers/Helper.h"
#include "StateIdentifiers.h"
#include "Context.h"
#include "ClientStateStack.h"
#include "ResourceManagers/ResourceIdentifiers.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

namespace client
{

TitleState::TitleState()
	: mTextTime(0)
{
	setupContinueText();
	setupTitleText();
}

TitleState::~TitleState()
{
}

bool TitleState::update(float deltaSeconds)
{
	continueTextBlink(deltaSeconds);
	return false;
}

bool TitleState::handleEvent(const ::sf::Event& event)
{
	ClientStateStack& stack = *Context::getInstance().stack;
	if (event.type == ::sf::Event::KeyPressed)
	{
		stack.popState();
		stack.pushState(StateID::Menu);
	}
	return false;
}

void TitleState::draw()
{
	getRenderWindow().draw(mTitleText);
	getRenderWindow().draw(mContinueText);
}

void TitleState::setupContinueText()
{
	mContinueText.setString("Press any key to Continue");
	mContinueText.setFont(*getContext().fontManager->get(Font::Sansation));
	mContinueText.setFillColor(::sf::Color::White);
	helper::Graphics::centreOrigin(mContinueText);
	::sf::Vector2u windowSize(getRenderWindow().getSize());
	mContinueText.setPosition((float)windowSize.x / 2,
		(float)windowSize.y - 100);
}

void TitleState::setupTitleText()
{
	mTitleText.setString("Tankette Wars");
	mTitleText.setCharacterSize(70);
	mTitleText.setFont(*getContext().fontManager->get(Font::Sansation));
	mTitleText.setFillColor(::sf::Color::Cyan);
	helper::Graphics::centreOrigin(mTitleText);
	::sf::Vector2u windowSize(getRenderWindow().getSize());
	mTitleText.setPosition((float)windowSize.x / 2,
		(float)windowSize.y / 2);

}

constexpr float PI = 3.14159265f;
void TitleState::continueTextBlink(float deltaSeconds)
{
	mTextTime += deltaSeconds;
	if (mTextTime > PI)
		mTextTime = 0;
	float textAlpha = abs(sin(mTextTime));

	::sf::Color textColor(mContinueText.getFillColor());
	::sf::Color newColor(textColor.a, textColor.g, textColor.b,
		(::sf::Uint8) (textAlpha * 255));
	mContinueText.setFillColor(newColor);
}

}