
#pragma once

#include "Condition.h"

namespace parser { namespace pddl {

class When : public Condition {

public:

	std::shared_ptr<Condition> pars, cond;

	When() = default;

	When(const When& w, const Domain& d)
	{
		if (w.pars) 
			pars = w.pars->copy(d);
		if (w.cond) 
			cond = w.cond->copy(d);
	}

	~When() override = default;

	void print(std::ostream& s) const override
	{
		s << "WHEN:\n";
		if (pars) 
			pars->print(s);
		if (cond) 
			cond->print(s);
	}

	void PDDLPrint( std::ostream & s, unsigned indent, const TokenStruct< std::string > & ts, const Domain & d ) const override;

	void parse( Filereader & f, TokenStruct< std::string > & ts, Domain& d) override;

	void addParams( int m, unsigned n ) override
	{
		pars->addParams( m, n );
		cond->addParams( m, n );
	}

	[[nodiscard]] std::shared_ptr<Condition> copy(const Domain& d) const override
	{
		return std::make_shared<When>(*this, d);
	}

};

} } // namespaces
