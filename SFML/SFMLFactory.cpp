/** @file SFMLFactory.cpp
 ** @author Piotr Krupa (piotrkrupa06@gmail.com)
 ** @license MIT License
 **/

#include "SFMLFactory.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>

#include <SFML/Graphics.hpp>

#include "SFMLSlot.h"
#include "SFMLTextureData.h"
#include "SFMLTextureAtlasData.h"
#include "SFMLArmatureProxy.h"
#include "SFMLDisplay.h"
#include "SFMLEventDispatcher.h"

DRAGONBONES_NAMESPACE_BEGIN

DragonBones* SFMLFactory::_dragonBonesInstance;
SFMLFactory* SFMLFactory::_factory;

SFMLFactory::SFMLFactory()
{
	_factory = this;

	if (_dragonBonesInstance == nullptr)
	{
		_soundEventDispatcher = std::make_unique<SFMLEventDispatcher>();

		_dragonBonesInstance = new DragonBones(_soundEventDispatcher.get());
	}

	_dragonBones = _dragonBonesInstance;
}

SFMLFactory::~SFMLFactory()
{
	clear();

	if (_dragonBonesInstance)
	{
		delete _dragonBonesInstance;
	}
}

DragonBonesData* SFMLFactory::loadDragonBonesData(const std::string& filePath,
												  const std::string& name,
												  const float scale)
{
	sf::FileInputStream stream;
	if (!stream.open(filePath))
	{
		std::cout << "Error to open skeleton file." << std::endl;
		return nullptr;
	}
	const sf::Int64 size = stream.getSize();
	char *chars = new char[size];
	const sf::Int64 readed = stream.read(chars, size);
	const std::string data = std::string(chars).substr(0, readed);
	delete[] chars;
	if (data.empty())
		return nullptr;

	return loadDragonBonesData(data.c_str(), name, scale);
}

DragonBonesData *SFMLFactory::loadDragonBonesData(char *data, const std::string &name, const float scale)
{
	if (!name.empty())
	{
		DragonBonesData* existedData = getDragonBonesData(name);
		if (existedData)
			return existedData;
	}
	return parseDragonBonesData(data, name, scale);
}

TextureAtlasData* SFMLFactory::loadTextureAtlasData(const std::string& filePath,
													sf::Texture* atlasTexture,
													const std::string& name,
													float scale)
{
	sf::FileInputStream stream;
	if (!stream.open(filePath))
	{
		std::cout << "Error to open atlas file." << std::endl;
		return nullptr;
	}
	const sf::Int64 size = stream.getSize();
	char *chars = new char[size];
	const sf::Int64 readed = stream.read(chars, size);
	const std::string data = std::string(chars).substr(0, readed);
	delete[] chars;
	if (data.empty())
		return nullptr;
	return loadTextureAtlasData(data.c_str(), atlasTexture, name, scale);
}

TextureAtlasData *SFMLFactory::loadTextureAtlasData(char *data,
													sf::Texture* atlasTexture,
													const std::string &name,
													float scale)
{
	return static_cast<SFMLTextureAtlasData*>(BaseFactory::parseTextureAtlasData(data, atlasTexture, name, scale));
}

SFMLArmatureProxy* SFMLFactory::buildArmatureDisplay(const std::string& armatureName, const std::string& dragonBonesName, const std::string& skinName, const std::string& textureAtlasName) const
{
	const auto armature = buildArmature(armatureName, dragonBonesName, skinName, textureAtlasName);

	if (armature != nullptr)
	{
		_dragonBones->getClock()->add(armature);

		return static_cast<SFMLArmatureProxy*>(armature->getDisplay());
	}

	return nullptr;
}

sf::Texture* SFMLFactory::getTextureDisplay(const std::string& textureName, const std::string& dragonBonesName) const
{
	const auto textureData = static_cast<SFMLTextureData*>(_getTextureData(dragonBonesName, textureName));
	if (textureData != nullptr && textureData->texture != nullptr)
	{
		return textureData->texture;
	}

	return nullptr;
}

void SFMLFactory::update(float lastUpdate)
{
	_dragonBonesInstance->advanceTime(lastUpdate);
}

TextureAtlasData* SFMLFactory::_buildTextureAtlasData(TextureAtlasData* textureAtlasData, void* textureAtlas) const
{
	auto textureAtlasData_ = static_cast<SFMLTextureAtlasData*>(textureAtlasData);

	if (textureAtlasData != nullptr)
	{
		if (textureAtlas != nullptr)
		{
			textureAtlasData_->setRenderTexture(static_cast<sf::Texture*>(textureAtlas));
		}
		else
		{
			DRAGONBONES_ASSERT(false, "No atlas texture");
		}
	}
	else
	{
		textureAtlasData_ = BaseObject::borrowObject<SFMLTextureAtlasData>();
	}

	return textureAtlasData_;
}

Armature* SFMLFactory::_buildArmature(const BuildArmaturePackage& dataPackage) const
{
	const auto armature = BaseObject::borrowObject<Armature>();
	const auto armatureDisplay = new SFMLArmatureProxy();

	armature->init(dataPackage.armature, armatureDisplay, armatureDisplay, _dragonBones);

	return armature;
}

Slot* SFMLFactory::_buildSlot(const BuildArmaturePackage& dataPackage, const SlotData* slotData, Armature* armature) const
{
	auto slot = BaseObject::borrowObject<SFMLSlot>();
	auto wrapperDisplay = new SFMLDisplay();

	slot->init(slotData, armature, wrapperDisplay, wrapperDisplay);

	return slot;
}

DRAGONBONES_NAMESPACE_END
