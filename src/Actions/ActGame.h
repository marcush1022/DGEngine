#pragma once

#include "Action.h"
#include "Game.h"

class ActGameClearPlayingSounds : public Action
{
public:
	virtual bool execute(Game& game)
	{
		game.Resources().clearPlayingSounds();
		return true;
	}
};

class ActGameClose : public Action
{
public:
	virtual bool execute(Game& game)
	{
		game.Window().close();
		return true;
	}
};

class ActGameEnableInput : public Action
{
private:
	bool enable;

public:
	ActGameEnableInput(bool enable_) : enable(enable_) {}

	virtual bool execute(Game& game)
	{
		game.enableInput(enable);
		return true;
	}
};

class ActGamePauseOnFocusLoss : public Action
{
private:
	bool pause;

public:
	ActGamePauseOnFocusLoss(bool pause_) : pause(pause_) {}

	virtual bool execute(Game& game)
	{
		game.PauseOnFocusLoss(pause);
		return true;
	}
};

class ActGameSetMusicVolume : public Action
{
private:
	Variable volume;

public:
	ActGameSetMusicVolume(const Variable& volume_) : volume(volume_) {}

	virtual bool execute(Game& game)
	{
		auto vol = game.getVarOrProp<int64_t, unsigned>(volume, 100u);
		game.MusicVolume(vol);
		return true;
	}
};

class ActGameSetProperty : public Action
{
private:
	std::string prop;
	Variable value;
	sf::Vector2i propRange;
	sf::Vector2i valueRange;
	bool hasPropRange{ false };
	bool hasValueRange{ false };

public:
	ActGameSetProperty(const std::string& prop_, const Variable& value_)
		: prop(prop_), value(value_) {}

	void setPropRange(const sf::Vector2i& propRange_)
	{
		propRange = propRange_;
		hasPropRange = true;
	}
	void setValueRange(const sf::Vector2i& valueRange_)
	{
		valueRange = valueRange_;
		hasValueRange = true;
	}

	virtual bool execute(Game& game)
	{
		auto prop2 = game.getVarOrPropString(prop);
		auto value2 = game.getVarOrProp(value);
		if (value2.is<int64_t>() == true)
		{
			if (hasPropRange == false && hasValueRange == false)
			{
				game.setProperty(prop2, value2);
			}
			else
			{
				auto val = (long)value2.get<int64_t>();
				if (hasValueRange == true)
				{
					if (val < (long)valueRange.x)
					{
						val = (long)valueRange.x;
					}
					else if (val > (long)valueRange.y)
					{
						val = (long)valueRange.y;
					}
				}
				else if (hasPropRange == true)
				{
					if (val < (long)propRange.x)
					{
						val = (long)propRange.x;
					}
					else if (val >(long)propRange.y)
					{
						val = (long)propRange.y;
					}
				}
				if (hasPropRange == true)
				{
					auto x = valueRange.x;
					auto y = valueRange.y;
					auto valueDiff = x > y ? x - y : y - x;
					x = propRange.x;
					y = propRange.y;
					auto PropDiff = x > y ? x - y : y - x;

					val -= valueRange.x;
					val = std::lround(val * PropDiff / (double)valueDiff) + propRange.x;
				}
				game.setProperty(prop2, Variable((int64_t)val));
			}
		}
		else
		{
			game.setProperty(prop2, value2);
		}
		return true;
	}
};

class ActGameSetSoundVolume : public Action
{
private:
	Variable volume;

public:
	ActGameSetSoundVolume(const Variable& volume_) : volume(volume_) {}

	virtual bool execute(Game& game)
	{
		auto vol = game.getVarOrProp<int64_t, unsigned>(volume, 100u);
		game.SoundVolume(vol);
		return true;
	}
};
