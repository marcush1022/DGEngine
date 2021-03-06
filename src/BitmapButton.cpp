#include "BitmapButton.h"
#include "Game.h"
#include "GameUtils.h"
#include "Utils.h"

void BitmapButton::setAction(uint16_t nameHash16, const std::shared_ptr<Action>& action)
{
	switch (nameHash16)
	{
	case str2int16("click"):
		clickAction = action;
		return;
	case str2int16("doubleClick"):
		doubleClickAction = action;
		return;
	case str2int16("clickDrag"):
		clickDragAction = action;
		return;
	case str2int16("clickIn"):
		clickInAction = action;
		return;
	case str2int16("clickOut"):
		clickOutAction = action;
		return;
	case str2int16("focus"):
		focusAction = action;
		return;
	case str2int16("hoverEnter"):
		hoverEnterAction = action;
		return;
	case str2int16("hoverLeave"):
		hoverLeaveAction = action;
		return;
	}
}

void BitmapButton::click(Game& game, bool playSound)
{
	if (enabled == false)
	{
		return;
	}
	if (clickAction != nullptr)
	{
		if (focusEnable == true)
		{
			game.Resources().setFocused(this);
			if (focusOnClick == true)
			{
				game.Events().addBack(focusAction);
			}
		}
		if (playSound == true)
		{
			game.addPlayingSound(clickSound.get());
		}
		game.Events().addBack(clickAction);
	}
}

void BitmapButton::focus(Game& game) const
{
	game.addPlayingSound(focusSound.get());
	game.Events().addFront(focusAction);
}

void BitmapButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (visible == true)
	{
		target.draw(sprite, states);
	}
}

void BitmapButton::updateSize(const Game& game)
{
	if (game.StretchToFit() == true)
	{
		return;
	}
	auto pos = sprite.getPosition();
	auto size = sf::Vector2f(sprite.getGlobalBounds().width, sprite.getGlobalBounds().height);
	GameUtils::setAnchorPosSize(anchor, pos, size, game.OldWindowSize(), game.WindowSize());
	sprite.setPosition(pos);
	if (resizable == true)
	{
		sprite.setTextureRect(sf::IntRect(0, 0, (int)size.x, (int)size.y));
	}
}

void BitmapButton::update(Game& game)
{
	if (visible == false)
	{
		return;
	}

	auto rect = sprite.getGlobalBounds();
	if (rect.contains(game.MousePositionf()))
	{
		if (captureScrollEvent == true &&
			game.wasMouseScrolled() == true)
		{
			game.clearMouseScrolled();
		}
		if (hovered == false)
		{
			hovered = true;
			if (hoverEnterAction != nullptr)
			{
				game.Events().addBack(hoverEnterAction);
			}
		}
		if (game.getMouseButton() == sf::Mouse::Left)
		{
			if (game.wasMouseClicked() == true)
			{
				game.clearMouseClicked();
				beingDragged = true;
				wasClicked = true;
				if (clickInAction != nullptr)
				{
					game.Events().addFront(clickInAction);
				}
				if (clickUp == false)
				{
					click(game, true);
					if (game.wasMouseDoubleClicked() == true)
					{
						if (doubleClickAction != nullptr)
						{
							game.Events().addBack(doubleClickAction);
						}
					}
				}
			}
			else if (game.wasMouseReleased() == true)
			{
				if (clickOutAction != nullptr)
				{
					game.Events().addFront(clickOutAction);
				}
				if (clickUp == true && beingDragged == true)
				{
					click(game, true);
				}
				beingDragged = false;
			}
		}
	}
	else
	{
		if (game.wasMouseReleased() == true &&
			game.getMouseButton() == sf::Mouse::Left)
		{
			beingDragged = false;
			if (wasClicked == true)
			{
				wasClicked = false;
				if (clickOutAction != nullptr)
				{
					game.Events().addFront(clickOutAction);
				}
			}
		}
		if (hovered == true)
		{
			hovered = false;
			if (hoverLeaveAction != nullptr)
			{
				game.Events().addFront(hoverLeaveAction);
			}
		}
	}
	if (beingDragged == true && game.wasMouseMoved() == true)
	{
		if (clickDragAction != nullptr)
		{
			game.Events().addFront(clickDragAction);
		}
	}
}

bool BitmapButton::getProperty(const std::string& prop, Variable& var) const
{
	if (prop.size() <= 1)
	{
		return false;
	}
	auto props = Utils::splitStringIn2(prop, '.');
	return GameUtils::getUIObjProp(*this, str2int16(props.first.c_str()), props.second, var);
}
