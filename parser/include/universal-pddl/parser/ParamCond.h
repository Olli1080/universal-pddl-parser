
#pragma once

#include "Condition.h"
#include "Basic.h"

namespace parser { namespace pddl {


// This is necessary for ADL
using ::operator<<;

class ParamCond : public Condition
{
public:
	std::string name;
	IntVec params;

	ParamCond() = default;

	ParamCond(std::string s, IntVec p = IntVec())
		: name(std::move(s)), params(std::move(p))
	{}

	ParamCond(const ParamCond& c)
		: name(c.name), params(c.params) {}

	[[nodiscard]] std::string c_str() const
	{
		return name;
	}

	void print(std::ostream& stream) const override
	{
		stream << name << params << "\n";
	}

	void printParams(unsigned first, std::ostream& s, TokenStruct<std::string>& ts, const Domain& d) const;
};

typedef std::vector<ParamCond*> ParamCondVec;

} } // namespaces
