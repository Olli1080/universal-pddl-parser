
#pragma once

#include "ParamCond.h"

namespace parser { namespace pddl {

class Forall : public ParamCond
{
public:
	std::shared_ptr<Condition> cond;

	Forall() = default;

	Forall(const Forall& f, Domain& d)
		: ParamCond( f )
	{
		if (f.cond) cond = f.cond->copy(d);
	}

	~Forall() override = default;

	void print(std::ostream& s) const override
	{
		s << "Forall" << params << ":\n";
		if ( cond ) cond->print(s);
	}

	void PDDLPrint(std::ostream& s, unsigned indent, const TokenStruct<std::string>& ts, const Domain& d) const override;

	void parse(Filereader& f, TokenStruct<std::string>& ts, Domain& d) override;

	void addParams(int m, unsigned n) override
	{
		cond->addParams(m, n);
	}

	std::shared_ptr<Condition> copy(Domain& d) override
	{
		return std::make_shared<Forall>(*this, d);
	}

};

} } // namespaces