
#pragma once

#include "ParamCond.h"

namespace parser { namespace pddl {

class Task : public ParamCond
{
public:

	Task() = default;

	Task(const std::string& s)
		: ParamCond(s) {}

	Task(const ParamCond& c)
		: ParamCond(c) {}

    void PDDLPrint(std::ostream& s, unsigned indent, const TokenStruct<std::string>& ts, const Domain& d) const override {}

	void parse(Filereader& f, TokenStruct<std::string>& ts, Domain& d) override {}

	void addParams(int m, unsigned n) override {}

	std::shared_ptr<Condition> copy(Domain& d) override
	{
		return std::make_shared<Task>(*this);
	}

};

typedef std::vector<Task*> TaskVec;

} } // namespaces