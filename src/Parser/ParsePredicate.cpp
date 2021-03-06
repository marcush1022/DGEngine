#include "ParsePredicate.h"
#include "Predicates/PredIO.h"
#include "Predicates/PredPlayer.h"
#include "Utils.h"
#include "Utils/ParseUtils.h"

namespace Parser
{
	using namespace rapidjson;

	std::shared_ptr<Predicate> parsePredicateObj(Game& game, const Value& elem)
	{
		if (elem.HasMember("name") == false ||
			elem["name"].IsString() == false)
		{
			return nullptr;
		}
		switch (str2int16(elem["name"].GetString()))
		{
		case str2int16("file.exists"):
		{
			return std::make_shared<PredFileExists>(getStringKey(elem, "file"));
		}
		case str2int16("player.canEquipItem"):
		{
			return std::make_shared<PredPlayerCanEquip>(
				getStringKey(elem, "level"),
				getStringKey(elem, "player"),
				getItemLocationKey(elem, "item"));
		}
		case str2int16("player.getMaxGoldCapacity"):
		{
			return std::make_shared<PredPlayerGetMaxGoldCapacity>(
				getStringKey(elem, "level"),
				getStringKey(elem, "player"));
		}
		case str2int16("player.hasFreeItemSlot"):
		{
			return std::make_shared<PredPlayerHasFreeItemSlot>(
				getStringKey(elem, "level"),
				getStringKey(elem, "player"),
				getItemCoordInventoryKey(elem, "item"));
		}
		default:
			return nullptr;
		}
	}
}
