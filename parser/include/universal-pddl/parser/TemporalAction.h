
#pragma once

#include "Action.h"
#include "And.h"
#include "Expression.h"

namespace parser { namespace pddl {

class Instance;

class TemporalAction : public Action
{
public:

	std::shared_ptr<Expression> durationExpr;
	// pre_s and eff_s inherited from Action
	std::shared_ptr<And> pre_o, pre_e, eff_e;

	TemporalAction(const std::string& s)
		: Action(s)
	{}

	~TemporalAction() override = default;

	void print(std::ostream& s) const override
	{
		s << name << params << "\n";
		s << "Duration: " << durationExpr->info() << "\n";
		s << "Pre_s: " << pre;
		s << "Eff_s: " << eff;
		s << "Pre_o: " << pre_o;
		s << "Pre_e: " << pre_e;
		s << "Eff_e: " << eff_e;
	}

	double duration() override
	{
		if (durationExpr) 
			return durationExpr->evaluate();
		else 
			return 0;
	}

	std::shared_ptr<Expression> parseDuration(Filereader& f, TokenStruct<std::string>& ts, Domain& d);

	void printCondition(std::ostream& s, const TokenStruct<std::string>& ts, const Domain& d,
	                     const std::string& t, const And& a ) const;

	void PDDLPrint( std::ostream & s, unsigned indent, const TokenStruct< std::string > & ts, const Domain & d ) const override;

	void parseCondition( Filereader & f, TokenStruct< std::string > & ts, Domain & d, And& a);

	void parse( Filereader & f, TokenStruct< std::string > & ts, Domain & d ) override;

	std::vector<std::shared_ptr<Ground>> preconsStart();

	std::vector<std::shared_ptr<Ground>> preconsOverall();

	std::vector<std::shared_ptr<Ground>> preconsEnd();

	std::vector<std::shared_ptr<Condition>> endEffects();

	std::vector<std::shared_ptr<Ground>> addEndEffects();

	std::vector<std::shared_ptr<Ground>> deleteEndEffects();
};

} } // namespaces
