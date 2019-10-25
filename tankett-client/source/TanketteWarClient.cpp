#include "TanketteWarClient.h"
#include "ClientState.h"
#include "TitleState.h"
#include "MenuState.h"
#include "GameState.h"
#include "PauseState.h"
#include "CreditState.h"
#include "SettingState.h"
#include "ConnectState.h"
#include "ClientContext.h"


#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Color.hpp>
#include <string>  
#include <alpha.h>
#include <tankett_shared.h>
#include <cstdlib>

namespace mw
{
	Game* Game::create()
	{
		return new TanketteWarClient();
	}
}

TanketteWarClient::TanketteWarClient()
	try 
	: mWindow(
	sf::VideoMode(960, 540),
	"Tankket Wars",
	sf::Style::Titlebar | sf::Style::Close)
	, mPlayerController(mWindow)
	, mStateStack()
	, mFPSMeter(mStatsText)
{
	mWindow.setKeyRepeatEnabled(true);

	mTextureManager.load(Texture::Back,
						 "../Assets/sunny-land-files/environment/back.png");

	mFontManager.load(Font::MineCraft, "../Assets/Fonts/Minecraft.ttf");
	mStatsText.setFont(*mFontManager.get(Font::MineCraft));
	mStatsText.setCharacterSize(20);
	mStatsText.setPosition(10, 10);
	mStatsText.setFillColor(sf::Color::Magenta);

	registerStates();
	mStateStack.pushState(StateID::Title);

	ClientContext& context = ClientContext::getInstance();
	context.stack = &mStateStack;
	context.window = &mWindow;
	context.playerController = &mPlayerController;
	context.fontManager = &mFontManager;
	context.textureManager = &mTextureManager;
	context.mapManager = &mMapManager;
	context.networkManager = &mNetworkManager;

}
catch (const std::runtime_error & e)
{
	std::cout << "Exception: " << e.what() << std::endl;
	std::terminate();
}

const sf::Time updateInterval = sf::seconds(1.f / 60.f);
const sf::Time networkInterval = sf::seconds(1.f / PROTOCOL_SEND_PER_SEC);
void TanketteWarClient::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	sf::Time timeSinceLastNetwork = sf::Time::Zero;

	while (mWindow.isOpen())
	{
		sf::Time lastLoopDuration = clock.getElapsedTime();
		// network fixed send
		timeSinceLastNetwork += lastLoopDuration;

		while (timeSinceLastNetwork > networkInterval)
		{
			mNetworkManager.send();
			timeSinceLastNetwork -= networkInterval;
		}
		mNetworkManager.receive();

		// Fix the update interval. 
		// In case render takes longer than TimePerFrame
		lastLoopDuration = clock.restart();
		timeSinceLastUpdate += lastLoopDuration;
		while (timeSinceLastUpdate > updateInterval)
		{
			timeSinceLastUpdate -= updateInterval;
			handleInputs();
			update(updateInterval.asSeconds());

			if (mStateStack.isEmpty())
				mWindow.close();
		}

		mFPSMeter.update(lastLoopDuration);

		render();
	}
}

void TanketteWarClient::registerStates()
{
	mStateStack.registerState<TitleState>(StateID::Title);
	mStateStack.registerState<MenuState>(StateID::Menu);
	mStateStack.registerState<GameState>(StateID::Game);
	mStateStack.registerState<PauseState>(StateID::Pause);
	mStateStack.registerState<CreditState>(StateID::Credit);
	mStateStack.registerState<SettingState>(StateID::Setting);
	mStateStack.registerState<ConnectState>(StateID::Connect);
}

void TanketteWarClient::handleInputs()
{
	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		mStateStack.handleEvent(event);

		// Closing window events
		if (event.type == sf::Event::Closed)
			mStateStack.clearStates();
	}
}

void TanketteWarClient::update(float deltaSeconds)
{
	mStateStack.update(deltaSeconds);
}

void TanketteWarClient::render()
{
	mWindow.clear();

	mStateStack.draw();

	// Move to default view so that stats can be drawn at the right position
	mWindow.setView(mWindow.getDefaultView());
	// Draw stats
	mWindow.draw(mStatsText);

	mWindow.display();
}
