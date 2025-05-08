
#pragma once

#include "Ground.h"

namespace parser { namespace pddl {

class Action : public ParamCond
{
public:

	std::shared_ptr<Condition> pre, eff;

	Action(const std::string& s)
		: ParamCond(s) {}

	Action(ParamCond& c)
		: ParamCond(c) {}

	Action(const Action& a, const Domain& d)
		: ParamCond(a)
	{
		if (a.pre) 
			pre = a.pre->copy(d);
		if (a.eff) 
			eff = a.eff->copy(d);
	}

	virtual ~Action() override = default;

	void print(std::ostream& s) const override
	{
		s << name << params << "\n";
		s << "Pre: " << pre;
		if (eff) 
			s << "Eff: " << eff;
	}

	virtual double duration()
	{
		return 1;
	}

	void PDDLPrint( std::ostream & s, unsigned indent, const TokenStruct< std::string > & ts, const Domain & d ) const override;

	void parseConditions( Filereader & f, TokenStruct< std::string > & ts, Domain & d );

	void parse( Filereader & f, TokenStruct< std::string > & ts, Domain & d ) override;

	void addParams( int m, unsigned n ) override {}

	void addParams( const IntVec & v ) {
		if (pre) pre->addParams(params.size(), v.size());
		if (eff) eff->addParams(params.size(), v.size());
		params.insert(params.end(), v.begin(), v.end());
	}

	[[nodiscard]] std::shared_ptr<Condition> copy(const Domain& d) const override
	{
		return std::make_shared<Action>(*this, d);
	}

	std::vector<std::shared_ptr<Condition>> precons();

	std::vector<std::shared_ptr<Condition>> effects();

	std::vector<std::shared_ptr<Ground>> addEffects();

	std::vector<std::shared_ptr<Ground>> deleteEffects();

protected:

	std::vector<std::shared_ptr<Condition>> getSubconditionsFromCondition(const std::shared_ptr<Condition>& c);

	std::vector<std::shared_ptr<Ground>> getGroundsFromCondition(const std::shared_ptr<Condition>& c, bool neg );
};

typedef std::vector<Action*> ActionVec;

} } // namespaces
