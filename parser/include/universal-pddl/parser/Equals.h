
#pragma once

#include "Ground.h"

namespace parser { namespace pddl {

class Equals : public Ground
{
public:

	Equals(const IntVec & p = IntVec())
		: Ground("=", p) {}

	Equals(const Equals& e)
		: Ground("=", e.params) {}

	void PDDLPrint(std::ostream& s, unsigned indent, const TokenStruct<std::string>& ts, const Domain& d) const override;

	void parse(Filereader& f, TokenStruct<std::string>& ts, Domain& d) override;

	[[nodiscard]] std::shared_ptr<Condition> copy(const Domain& d) const override
	{
		return std::make_shared<Equals>(*this);
	}

};

} } // namespaces
