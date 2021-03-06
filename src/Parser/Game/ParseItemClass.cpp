#include "ParseItemClass.h"
#include "Game/ItemClass.h"
#include "Parser/ParseAction.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	std::shared_ptr<ItemClass> parseItemClassHelper(const Game& game,
		const Level& level, const Value& elem, std::string& id)
	{
		if (isValidString(elem, "id") == false)
		{
			return nullptr;
		}
		id = std::string(elem["id"].GetString());
		if (isValidId(id) == false)
		{
			return nullptr;
		}
		if (level.hasItemClass(id) == true)
		{
			return nullptr;
		}

		std::shared_ptr<ItemClass> itemClass;

		if (isValidString(elem, "fromId") == true)
		{
			std::string fromId(elem["fromId"].GetString());
			if (fromId != id)
			{
				auto obj = level.getItemClass(fromId);
				if (obj == nullptr)
				{
					return nullptr;
				}
				itemClass = std::make_shared<ItemClass>(*obj);
			}
		}

		if (isValidString(elem, "celTextureDrop") == false ||
			isValidString(elem, "celTextureInventory") == false ||
			isValidString(elem, "celTextureInventoryUnusable") == false)
		{
			return itemClass;
		}

		auto celTexture = game.Resources().getCelTextureCacheVec(
			elem["celTextureDrop"].GetString());
		if (celTexture == nullptr)
		{
			return itemClass;
		}
		auto celDropIdx = (size_t)getUIntKey(elem, "celIndexDrop");
		if (celDropIdx >= celTexture->size())
		{
			return itemClass;
		}
		auto celTextureInv = game.Resources().getCelTextureCache(
			elem["celTextureInventory"].GetString());
		if (celTextureInv == nullptr)
		{
			return itemClass;
		}
		auto celTextureInvUnusable = game.Resources().getCelTextureCache(
			elem["celTextureInventoryUnusable"].GetString());
		if (celTextureInvUnusable == nullptr)
		{
			return itemClass;
		}
		auto celInventoryIdx = (size_t)getUIntKey(elem, "celIndexInventory");
		if (celInventoryIdx >= celTextureInv->size() ||
			celInventoryIdx >= celTextureInvUnusable->size())
		{
			return itemClass;
		}

		if (itemClass == nullptr)
		{
			itemClass = std::make_shared<ItemClass>(celTexture, celDropIdx,
				celTextureInv, celTextureInvUnusable, celInventoryIdx);
		}
		else
		{
			itemClass->setCelTextureDrop(celTexture);
			itemClass->setCelDropIdx(celDropIdx);
			itemClass->setCelTextureInventory(celTextureInv);
			itemClass->setCelTextureInventoryUnusable(celTextureInvUnusable);
			itemClass->setCelInventoryIdx(celInventoryIdx);
		}
		return itemClass;
	}

	void parseItemClass(Game& game, const Value& elem)
	{
		auto level = game.Resources().getLevel(getStringKey(elem, "level"));
		if (level == nullptr)
		{
			return;
		}
		std::string id;
		auto itemClass = parseItemClassHelper(game, *level, elem, id);
		if (itemClass == nullptr)
		{
			return;
		}

		if (elem.HasMember("name") == true)
		{
			itemClass->Name(getStringVal(elem["name"]));
		}
		if (elem.HasMember("shortName") == true)
		{
			itemClass->ShortName(getStringVal(elem["shortName"]));
		}
		if (elem.HasMember("type") == true)
		{
			itemClass->Type(getStringVal(elem["type"]));
		}
		if (elem.HasMember("inventorySize") == true)
		{
			itemClass->InventorySize(getItemXYVal(elem["inventorySize"], ItemXY(1, 1)));
		}

		if (elem.HasMember("defaults") == true)
		{
			const auto& defaults = elem["defaults"];
			if (defaults.IsObject() == true)
			{
				for (auto it = defaults.MemberBegin(); it != defaults.MemberEnd(); ++it)
				{
					if (it->name.GetStringLength() > 0)
					{
						itemClass->setDefault(it->name.GetString(),
							getMinMaxIntVal<LevelObjValue>(it->value));
					}
				}
			}
		}

		if (elem.HasMember("action") == true)
		{
			itemClass->setAction(parseAction(game, elem["action"]));
		}
		if (elem.HasMember("actionDropLevel") == true)
		{
			itemClass->setActionDropLevel(parseAction(game, elem["actionDropLevel"]));
		}
		if (elem.HasMember("actionPickLevel") == true)
		{
			itemClass->setActionPickLevel(parseAction(game, elem["actionPickLevel"]));
		}
		if (elem.HasMember("actionDropInventory") == true)
		{
			itemClass->setActionDropInventory(parseAction(game, elem["actionDropInventory"]));
		}
		if (elem.HasMember("actionPickInventory") == true)
		{
			itemClass->setActionPickInventory(parseAction(game, elem["actionPickInventory"]));
		}
		if (elem.HasMember("prefix") == true)
		{
			auto namer = level->getNamer(getStringVal(elem["prefix"]));
			itemClass->setPrefix(namer);
		}
		if (elem.HasMember("suffix") == true)
		{
			auto namer = level->getNamer(getStringVal(elem["suffix"]));
			itemClass->setSuffix(namer);
		}

		if (elem.HasMember("description1") == true)
		{
			auto namer = level->getNamer(getStringVal(elem["description1"]));
			itemClass->setDescription(0, namer);
		}
		if (elem.HasMember("description2") == true)
		{
			auto namer = level->getNamer(getStringVal(elem["description2"]));
			itemClass->setDescription(1, namer);
		}
		if (elem.HasMember("description3") == true)
		{
			auto namer = level->getNamer(getStringVal(elem["description3"]));
			itemClass->setDescription(2, namer);
		}
		if (elem.HasMember("description4") == true)
		{
			auto namer = level->getNamer(getStringVal(elem["description4"]));
			itemClass->setDescription(3, namer);
		}
		if (elem.HasMember("description5") == true)
		{
			auto namer = level->getNamer(getStringVal(elem["description5"]));
			itemClass->setDescription(4, namer);
		}

		if (elem.HasMember("price1") == true)
		{
			itemClass->setPriceFormula(0, getStringVal(elem["price1"]));
		}
		if (elem.HasMember("price2") == true)
		{
			itemClass->setPriceFormula(1, getStringVal(elem["price2"]));
		}
		if (elem.HasMember("price3") == true)
		{
			itemClass->setPriceFormula(2, getStringVal(elem["price3"]));
		}
		if (elem.HasMember("price4") == true)
		{
			itemClass->setPriceFormula(3, getStringVal(elem["price4"]));
		}
		if (elem.HasMember("price5") == true)
		{
			itemClass->setPriceFormula(4, getStringVal(elem["price5"]));
		}

		level->addItemClass(id, itemClass);
	}
}
