
#pragma once

#include "Condition.h"

namespace parser { namespace pddl {

class Oneof : public Condition
{
public:

	std::vector<std::shared_ptr<Condition>> conds;

	Oneof() = default;

	Oneof(const Oneof& o, Domain& d)
	{
		for (const auto& cond : o.conds)
			conds.emplace_back(cond->copy(d));
	}

	~Oneof() override = default;

	void print(std::ostream& s) const override
	{
		for (const auto& cond : conds)
			cond->print(s);
	}

	void PDDLPrint(std::ostream& s, unsigned indent, const TokenStruct<std::string>& ts, const Domain& d) const override;

	void parse(Filereader& f, TokenStruct<std::string>& ts, Domain& d) override;

	void add(const std::shared_ptr<Condition>& cond)
	{
		conds.emplace_back(cond);
	}

	void addParams(int m, unsigned n) override
	{
		for (const auto& cond : conds)
			cond->addParams(m, n);
	}

	std::shared_ptr<Condition> copy(Domain& d) override
	{
		return std::make_shared<Oneof>(*this, d);
	}

};

} } // namespaces
