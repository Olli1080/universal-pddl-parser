
#pragma once

#include "TypeGround.h"

namespace parser { namespace pddl {

template<typename T>
class GroundFunc : public TypeGround
{
public:

	T value;

	GroundFunc()
		: value(0) {}

	GroundFunc(const std::shared_ptr<Lifted>& l, const T& val = T(0))
		: TypeGround(l), value(val) {}

	void PDDLPrint(std::ostream& s, unsigned indent, const TokenStruct<std::string>& ts, const Domain& d) const override;

	void parse(Filereader& f, TokenStruct<std::string>& ts, Domain& d) override;

};

} } // namespaces
