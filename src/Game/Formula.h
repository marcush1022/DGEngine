#pragma once

#include "LevelObject.h"
#include <string>
#include "Variant.h"
#include <vector>

// use brackets to force order
// ex: 2 + 2 * 4 = 16
// ex: 2 + (2 * 4) = 10
class Formula
{
private:
	enum class FormulaOp
	{
		Add,
		Subtract,
		Multiply,
		Divide,
		LeftBracket,
		RightBracket,
	};
	typedef mapbox::util::variant<FormulaOp, double, std::string> FormulaElement;

	std::vector<FormulaElement> elements;

	double eval(size_t& idx, const LevelObject& query) const;

public:
	Formula() {}
	Formula(const std::string& formula);

	bool empty() const { return elements.empty(); }

	double eval(const LevelObject& query) const;

	std::string toString() const;
};
