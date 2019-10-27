#include "World.h"
#include "Context.h"
#include "Units/unit.h"
#include "Actors/Actor.h"
#include "Actors/CameraActor.h"
#include "Tank.h"
#include "Actors/Obstacle.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <iostream>

using namespace tankett;
namespace client
{

World::World()
	: mWindow(*Context::getInstance().window)
	, mTextureManager(*Context::getInstance().textureManager)
	, mMapManager(*Context::getInstance().mapManager)
	, mCamera(nullptr)
	, mAvatar(nullptr)
{
	loadResources();
	buildScene();
}

World::~World()
{
	mTextureManager.unload(Texture::Middle);
	mTextureManager.unload(Texture::Avatar);
}

void World::loadResources()
{
	loadMaps();
	loadTextures();
}

void World::loadMaps()
{
	mMapManager.load(Map::DefaultMap, "../Assets/Maps/DefaultMap.txt");
}

void World::loadTextures()
{
	mTextureManager.load(Texture::Middle, "../Assets/sunny-land-files/environment/middle.png");
	mTextureManager.load(Texture::TankBlackHull, "../Assets/PNG/Tanks/tankBlack_outline.png");
	mTextureManager.load(Texture::TankBlackBarrel, "../Assets/PNG/Tanks/barrelBlack_outline.png");
	mTextureManager.load(Texture::TankBlackBullet, "../Assets/PNG/Bullets/bulletSilverSilver_outline.png");
	mTextureManager.load(Texture::Oil, "../Assets/PNG/Obstacles/oil.png");
}

void World::buildScene()
{
	/* this is the scene graph:
							sceneGraph
		 /        				|			              \
	  tanks               backgroundNode					camera
								|
						background sprite
	*/

	::sf::Texture* oilTexture = mTextureManager.get(Texture::Oil);
	Map* map = mMapManager.get(Map::DefaultMap);
	auto mapObj = map->buildMap(unit::unit2pix(1), unit::unit2pix(1), *oilTexture);
	mSceneGraph.attachChild(std::move(mapObj));


	for (int i = 0; i < 4; ++i)
	{
		::sf::Texture* tankHullTexture = mTextureManager.get(Texture::TankBlackHull);
		::sf::Texture* tankBarrelTexture = mTextureManager.get(Texture::TankBlackBarrel);
		::sf::Texture* tankBulletTexture = mTextureManager.get(Texture::TankBlackBullet);
		std::unique_ptr<Tank> tankActor = std::make_unique<Tank>(*tankHullTexture, *tankBarrelTexture, *tankBulletTexture);
		::sf::Vector2i randomTile = map->getRandomEmptyTile();
		tankActor->setPosition(randomTile.x * unit::unit2pix(1), randomTile.y * unit::unit2pix(1));
		tankActor->setCommandCategory((CommandCategory)((int)CommandCategory::Tank0 << i));
		if (i == 0)
			mAvatar = tankActor.get();
		mSceneGraph.attachChild(std::move(tankActor));
	}

	// Create a camera and attach to the avatar
	std::unique_ptr<CameraActor> cameraActor(std::make_unique<CameraActor>());
	mCamera = cameraActor.get();
	mAvatar->attachChild(std::move(cameraActor));
	const ::sf::Vector2f cameraSize(1920, 1080);
	mCamera->setSize(cameraSize);
	//mCamera->setPosition(cameraSize.x / 2, cameraSize.y / 2);


	// Create a background node and attach to the scene graph
	std::unique_ptr<Actor> backgroundNode(std::make_unique<Actor>());

	::sf::Texture* backgroundTexture = mTextureManager.get(Texture::Back);
	backgroundTexture->setRepeated(true);
	std::unique_ptr<SpriteActor> backgroundSprite(std::make_unique<SpriteActor>(
		*backgroundTexture,
		Rendering::Layer::Background));
	backgroundSprite->getSprite()->setOrigin(0, 0);
	::sf::IntRect backgroundRect(0, 0, (int)cameraSize.x, (int)cameraSize.y);
	backgroundSprite->getSprite()->setTextureRect(backgroundRect);
	backgroundNode->attachChild(std::move(backgroundSprite));

	mSceneGraph.attachChild(std::move(backgroundNode));
}

void World::update(float deltaSeconds)
{
	mWindow.setView(*mCamera);

	while (!mCommandQueue.isEmpty())
		mSceneGraph.onCommand(mCommandQueue.pop(), deltaSeconds);

	mSceneGraph.update(deltaSeconds);
	mSceneGraph.reportCollisionInfo(mPhysicsEngine);
	mPhysicsEngine.checkCollision();
	mSceneGraph.enforceDestruction(mSceneGraph);
}

void World::draw()
{
	mWindow.setView(*mCamera);
	mSceneGraph.reportRenderInfo(mRenderer);
	mWindow.draw(mRenderer);
	mRenderer.clearRenderBuffer();
}
}