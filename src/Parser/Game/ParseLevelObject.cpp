#include "ParseLevelObject.h"
#include "Game/ImageLevelObject.h"
#include "Parser/ParseAction.h"
#include "Parser/Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	void parseLevelObject(Game& game, const Value& elem)
	{
		if (isValidString(elem, "id") == false
			|| isValidString(elem, "texture") == false)
		{
			return;
		}
		std::string id(elem["id"].GetString());
		if (isValidId(id) == false)
		{
			return;
		}

		auto level = game.Resources().getLevel(getStringKey(elem, "level"));
		if (level == nullptr)
		{
			return;
		}

		auto mapPos = getVector2uKey<MapCoord>(elem, "mapPosition");
		auto mapSize = level->Map().MapSize();
		if (mapPos.x >= mapSize.x || mapPos.y >= mapSize.y)
		{
			return;
		}
		auto& mapCell = level->Map()[mapPos];

		if (mapCell.getObject<ImageLevelObject>() != nullptr)
		{
			return;
		}

		auto texture = game.Resources().getTexture(elem["texture"].GetString());
		if (texture == nullptr)
		{
			return;
		}

		auto levelObj = std::make_shared<ImageLevelObject>(*texture);

		levelObj->MapPosition(mapPos);
		mapCell.addFront(levelObj);

		levelObj->Hoverable(getBoolKey(elem, "enableHover", true));

		if (elem.HasMember("textureRect"))
		{
			sf::IntRect rect(0, 0, 32, 32);
			levelObj->setTextureRect(getIntRectKey(elem, "textureRect", rect));
		}

		levelObj->Id(id);
		levelObj->Name(getStringKey(elem, "name"));

		if (elem.HasMember("action") == true)
		{
			levelObj->setAction(parseAction(game, elem["action"]));
		}

		level->addLevelObject(levelObj);
	}
}
