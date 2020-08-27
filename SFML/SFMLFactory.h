/** @file SFMLFactory.h
 ** @author Piotr Krupa (piotrkrupa06@gmail.com)
 ** @license MIT License
 **/

#pragma once

#include <string>
#include <vector>

#include <DragonBonesHeaders.h>

#include <SFML/Graphics/Texture.hpp>

#include "SFMLEventDispatcher.h"

DRAGONBONES_NAMESPACE_BEGIN

class SFMLArmatureProxy;
class SFMLTextureData;

class SFMLFactory : public BaseFactory
{
protected:
	static DragonBones*											_dragonBonesInstance;
	static SFMLFactory*											_factory;

	std::unique_ptr<SFMLEventDispatcher>						_soundEventDispatcher;

public:
	SFMLFactory();
	~SFMLFactory();

public:
	DragonBonesData* loadDragonBonesData(const std::string& filePath,
										 const std::string& name = std::string(),
										 const float scale = 1.f);

	DragonBonesData* loadDragonBonesData(char* data,
										 const std::string& name = std::string(),
										 const float scale = 1.f);

	TextureAtlasData* loadTextureAtlasData(const std::string& filePath,
										   sf::Texture* atlasTexture,
										   const std::string& name = std::string(),
										   float scale = 1.0f);

	TextureAtlasData* loadTextureAtlasData(char* data,
										   sf::Texture *atlasTexture,
										   const std::string& name = std::string(),
										   float scale = 1.0f);

	SFMLArmatureProxy* buildArmatureDisplay(const std::string& armatureName,
											const std::string& dragonBonesName = std::string(),
											const std::string& skinName = std::string(),
											const std::string& textureAtlasName = std::string()) const;

	sf::Texture* getTextureDisplay(const std::string& textureName,
								   const std::string& dragonBonesName = std::string()) const;

	void addSoundEventListener(const std::function<void(EventObject*)>& listener)
	{
		_soundEventDispatcher->addDBEventListener(EventObject::SOUND_EVENT, listener);
	}

	void update(float lastUpdate);

	static SFMLFactory* get() { return _factory; }

protected:
	TextureAtlasData* _buildTextureAtlasData(TextureAtlasData* textureAtlasData, void* textureAtlas) const override;
	Armature* _buildArmature(const BuildArmaturePackage& dataPackage) const override;
	Slot* _buildSlot(const BuildArmaturePackage& dataPackage, const SlotData* slotData, Armature* armature) const override;
};

DRAGONBONES_NAMESPACE_END
