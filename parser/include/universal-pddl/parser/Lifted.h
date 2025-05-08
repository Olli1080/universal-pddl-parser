
#pragma once

#include "ParamCond.h"

namespace parser { namespace pddl {

class Lifted : public ParamCond
{
public:

	Lifted() = default;

	Lifted(const std::string& s)
		: ParamCond( s ) {}

	Lifted(const ParamCond& c)
		: ParamCond( c ) {}

	void PDDLPrint( std::ostream & s, unsigned indent, const TokenStruct< std::string > & ts, const Domain & d ) const override;

	void parse( Filereader & f, TokenStruct< std::string > & ts, Domain & d ) override;

	void addParams( int m, unsigned n ) override {}

	std::shared_ptr<Condition> copy(Domain& d) override
	{
		return std::make_shared<Lifted>(*this);
	}

};

typedef std::vector< Lifted * > LiftedVec;

} } // namespaces