
#pragma once

#include "ParamCond.h"

namespace parser { namespace pddl {

class Exists : public ParamCond {

public:

	std::shared_ptr<Condition> cond;

	Exists() = default;

	Exists(const Exists& e, const Domain& d)
		: ParamCond( e )
	{
		if (e.cond) cond = e.cond->copy(d);
	}

	~Exists() override = default;

	void print(std::ostream& s) const override
	{
		s << "Exists" << params << ":\n";
		cond->print(s);
	}

	void PDDLPrint(std::ostream& s, unsigned indent, const TokenStruct<std::string>& ts, const Domain& d) const override;

	void parse(Filereader& f, TokenStruct<std::string>& ts, Domain& d) override;

	void addParams(int m, unsigned n) override
	{
		cond->addParams( m, n );
	}

	[[nodiscard]] std::shared_ptr<Condition> copy(const Domain& d) const override
	{
		return std::make_shared<Exists>(*this, d);
	}

};

} } // namespaces
