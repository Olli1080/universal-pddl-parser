
#pragma once

#include "Ground.h"
#include "Condition.h"
#include "Function.h"
#include "Expression.h"


namespace parser { namespace pddl {

class FunctionModifier : public Condition {

public:

	std::string name;

	std::shared_ptr<Ground> modifiedGround;  // if null -> total-cost
	std::shared_ptr<Expression> modifierExpr;  // the expression by which we increment/decrement

	FunctionModifier(std::string name, int val = 1);

	FunctionModifier(std::string name, const std::shared_ptr<Function>& f, const IntVec& p = IntVec());

	FunctionModifier(std::string name, const FunctionModifier& i, Domain& d);

	~FunctionModifier() override = default;

	void print( std::ostream & s ) const override
	{
		s << name << " ";
		if (modifiedGround) modifiedGround->print(s);
		if (modifierExpr) modifierExpr->print(s);
		s << "\n";
	}

	void PDDLPrint(std::ostream& s, unsigned indent, const TokenStruct<std::string>& ts, const Domain& d) const override;

	void parse(Filereader& f, TokenStruct<std::string>& ts, Domain& d) override;

	void addParams(int m, unsigned n) override {}
};

class Decrease : public FunctionModifier {

public:

	Decrease(int val = 1) : FunctionModifier( "DECREASE", val ) {}

	Decrease(const std::shared_ptr<Function>& f, const IntVec& p = IntVec())
		: FunctionModifier("DECREASE", f, p) {}

	Decrease(const FunctionModifier& i, Domain& d)
		: FunctionModifier("DECREASE", i, d) {}

	std::shared_ptr<Condition> copy(Domain& d) override
	{
		return std::make_shared<Decrease>(*this, d);
	}
};

class Increase : public FunctionModifier {

public:

	Increase( int val = 1 ) : FunctionModifier( "INCREASE", val ) { }

	Increase(const std::shared_ptr<Function>& f, const IntVec & p = IntVec() ) : FunctionModifier( "INCREASE", f, p ) { }

	Increase( const FunctionModifier& i, Domain & d ) : FunctionModifier( "INCREASE", i, d ) { }

	std::shared_ptr<Condition> copy(Domain& d) override
	{
		return std::make_shared<Increase>(*this, d);
	}
};

} } // namespaces
