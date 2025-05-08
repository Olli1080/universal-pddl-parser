
#pragma once

#include "Lifted.h"

namespace parser { namespace pddl {

class Derived : public Lifted {

public:

	std::shared_ptr<Condition> cond;
	std::shared_ptr<Lifted> lifted;

	Derived() = default;

	Derived(const std::string& s)
		: Lifted(s) {}

	Derived(const Derived& z, Domain& d);

	void print( std::ostream & stream ) const override
	{
		stream << "Derived ";
		ParamCond::print( stream );
		if (cond) cond->print( stream );
	}

	void PDDLPrint( std::ostream & s, unsigned indent, const TokenStruct< std::string > & ts, const Domain & d ) const override;

	void parse( Filereader & f, TokenStruct< std::string > & ts, Domain & d );

	void addParams( int m, unsigned n ) override
	{
		for (int& param : params)
			if (param >= m ) param += n;
	}

	std::shared_ptr<Condition> copy(Domain& d) override
	{
		return std::make_shared<Derived>(*this, d);
	}

};

} } // namespaces
