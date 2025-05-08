
#pragma once

#include "Condition.h"

namespace parser { namespace pddl {

class And : public Condition
{
public:

	std::vector<std::shared_ptr<Condition>> conds;

	And() = default;

	And(const And& a, const Domain& d)
	{
		conds.reserve(a.conds.size());
		for (auto& cond : a.conds)
			conds.emplace_back(cond->copy(d));
	}

	~And() override = default;

	void print(std::ostream& s) const override
	{
		for (auto& cond : conds)
			cond->print( s );
	}

	void PDDLPrint( std::ostream & s, unsigned indent, const TokenStruct< std::string > & ts, const Domain & d ) const override;

	void parse( Filereader & f, TokenStruct< std::string > & ts, Domain & d ) override;

	void add(const std::shared_ptr<Condition>& cond)
	{
		conds.emplace_back(cond);
	}

	void addParams(int m, unsigned n) override
	{
		for (auto& cond : conds)
			cond->addParams( m, n );
	}

	[[nodiscard]] std::shared_ptr<Condition> copy(const Domain& d) const override
	{
		return std::make_shared<And>(*this, d);
	}

};

} } // namespaces