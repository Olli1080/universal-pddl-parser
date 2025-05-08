
#pragma once

#include "Type.h"

namespace parser { namespace pddl {

class EitherType : public Type
{
public:

	EitherType(const std::string& s)
		: Type(s) {}

	EitherType(const EitherType& t)
		: Type(t) {}
	
	[[nodiscard]] std::string getName() const override
	{
		std::string out = "EITHER";
		for (auto subtype : subtypes)
			out += "_" + subtype.lock()->getName();
		return out;
	}

	void PDDLPrint( std::ostream & s ) const override {}

	std::shared_ptr<Type> copy() override {
		return std::make_shared<EitherType>(*this);
	}

};

} } // namespaces
