#include "MenuTemplate.h"
template<typename EnumClass>
MenuTemplate<EnumClass>::MenuTemplate(const char* optionNames[], int optionCount)
	: mOptionNames(optionNames)
	, mOptionCount(optionCount)
	, mCurrentOption(static_cast<EnumClass>(0))
{
	setupOptions();
}

template<typename EnumClass>
bool MenuTemplate<EnumClass>::update(float deltaSeconds)
{
	return false;
}

template<typename EnumClass>
bool MenuTemplate<EnumClass>::handleEvent(const ::sf::Event& event)
{
	if (event.type != ::sf::Event::KeyPressed)
		return false;

	if (Input::eventInputCollectionPressed(event, Input::Collections::confirmInputs))
	{
		handleConfirmInput();
	}

	if (Input::eventInputCollectionPressed(event, Input::Collections::upInputs))
		helper::Enum::increment(mCurrentOption, mOptionCount, -1);

	if (Input::eventInputCollectionPressed(event, Input::Collections::downInputs))
		helper::Enum::increment(mCurrentOption, mOptionCount, 1);

	updateOptions();
	return false;
}

template<typename EnumClass>
void MenuTemplate<EnumClass>::draw()
{
	getRenderWindow().setView(getRenderWindow().getDefaultView());
	drawOptions();
}

template<typename EnumClass>
void MenuTemplate<EnumClass>::updateOptions()
{
	for (auto& pair : mOptionMap)
	{
		pair.second->setFillColor(::sf::Color::Blue);
	}
	mOptionMap[mCurrentOption]->setFillColor(::sf::Color::Green);
}

template<typename EnumClass>
void MenuTemplate<EnumClass>::setupOptions()
{
	for (int i = 0; i < mOptionCount; ++i)
	{
		EnumClass option = static_cast<EnumClass>(i);
		mOptionMap[option] = std::make_unique<::sf::Text>();
		::sf::Text& text = *mOptionMap[option];
		setupOptionsText(text, option);
		setupOptionsTextLocation(text, option);
	}

	updateOptions();
}


template<typename EnumClass>
void MenuTemplate<EnumClass>::setupOptionsText(::sf::Text& text, const EnumClass& option)
{
	text.setString(helper::Enum::getText(option, mOptionNames));
	text.setFont(*getContext().fontManager->get(Font::Sansation));
	text.setCharacterSize(40);
	helper::Graphics::centreOrigin(text);
	text.setFillColor(::sf::Color::Black);
}

template<typename EnumClass>
void MenuTemplate<EnumClass>::setupOptionsTextLocation(::sf::Text& text, const EnumClass& option)
{
	const auto windowSize = getRenderWindow().getSize();
	int optionIndex = static_cast<int>(option);
	text.setPosition((float)windowSize.x * 3 / 4,
		(float)windowSize.y
					 - 80 * (mOptionCount - optionIndex));
}

template<typename EnumClass>
void MenuTemplate<EnumClass>::drawOptions()
{
	for (auto const& pair : mOptionMap)
	{
		getRenderWindow().draw(*pair.second);
	}
}

template<typename EnumClass>
inline void MenuTemplate<EnumClass>::handleConfirmInput()
{
}
