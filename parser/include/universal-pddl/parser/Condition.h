
#pragma once

#include "Basic.h"
#include "Filereader.h"
#include "Type.h"

namespace parser { namespace pddl {

class Condition
{
public:

	virtual ~Condition() = default;

	virtual void print( std::ostream & stream ) const = 0;

	virtual void PDDLPrint( std::ostream & s, unsigned indent, const TokenStruct< std::string > & ts, const Domain & d ) const = 0;

	virtual void parse( Filereader & f, TokenStruct< std::string > & ts, Domain & d ) = 0;

	virtual void addParams( int m, unsigned n ) = 0;

	[[nodiscard]] virtual std::shared_ptr<Condition> copy(const Domain& d) const = 0;
};

inline std::ostream & operator<<( std::ostream & stream, const Condition& c )
{
	c.print(stream);
	return stream;
}

typedef std::vector<std::shared_ptr<Condition>> CondVec;

} } // namespaces
