
#pragma once

#include "Condition.h"

namespace parser { namespace pddl {

class Or : public Condition
{
public:
	std::shared_ptr<Condition> first, second;

	Or() = default;

	Or(const Or& o, const Domain& d)
	{
		if (o.first) first = o.first->copy(d);
		if (o.second) second = o.second->copy(d);
	}

	~Or() override = default;

	void print(std::ostream& s) const override
	{
		s << "OR:\n";
		if (first) first->print(s);
		if (second) second->print(s);
	}

	void PDDLPrint(std::ostream& s, unsigned indent, const TokenStruct<std::string>& ts, const Domain& d) const override;

	void parse(Filereader& f, TokenStruct<std::string>& ts, Domain& d) override;

	void addParams(int m, unsigned n) override
	{
		first->addParams(m, n);
		second->addParams(m, n);
	}

	[[nodiscard]] std::shared_ptr<Condition> copy(const Domain& d) const override
	{
		return std::make_shared<Or>(*this, d);
	}
};

} } // namespaces