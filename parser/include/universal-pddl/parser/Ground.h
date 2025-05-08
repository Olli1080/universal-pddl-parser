
#pragma once

#include "Lifted.h"

namespace parser { namespace pddl {

class Ground : public ParamCond {

public:

	std::weak_ptr<Lifted> lifted;

	Ground() = default;

	Ground(const std::string& s, const IntVec & p = IntVec())
		: ParamCond( s, p ) {}

	Ground(const std::shared_ptr<Lifted>& l, const IntVec& p = IntVec())
		: ParamCond(l->name, p), lifted(l) {}

	Ground(const Ground& g, const Domain& d);

	void PDDLPrint(std::ostream& s, unsigned indent, const TokenStruct<std::string>& ts, const Domain& d) const override;

	void parse(Filereader& f, TokenStruct<std::string>& ts, Domain& d) override;

	void addParams(int m, unsigned n) override
	{
		for (int& param : params)
			if (param >= m ) param += n;
	}

	[[nodiscard]] std::shared_ptr<Condition> copy(const Domain& d) const override
	{
		return std::make_shared<Ground>(*this, d);
	}

};

typedef std::vector<Ground*> GroundVec;

} } // namespaces
