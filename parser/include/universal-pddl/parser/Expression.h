
#pragma once

#include "Condition.h"

namespace parser { namespace pddl {

class Instance;

class Expression : public Condition
{
public:

	~Expression() override = default;
	[[nodiscard]] virtual std::string info() const = 0;
	virtual double evaluate() = 0;
	virtual double evaluate(Instance& ins, const StringVec& par) = 0;
	virtual IntSet params() = 0;

	// inherit
	void print(std::ostream& stream) const override
	{
		stream << info();
	}

	void parse(Filereader& f, TokenStruct<std::string>& ts, Domain& d) override {}
	void addParams(int m, unsigned n) override {}
};

std::shared_ptr<Expression> createExpression(Filereader& f, TokenStruct<std::string>& ts, Domain& d);

class CompositeExpression : public Expression
{
public:

	std::string op;
	std::shared_ptr<Expression> left;
	std::shared_ptr<Expression> right;

	CompositeExpression(std::string c) : op(std::move(c))
	{}

	CompositeExpression(std::string c, std::shared_ptr<Expression> l, std::shared_ptr<Expression> r )
	: op(std::move(c)), left(std::move(l)), right(std::move(r))
	{}

	~CompositeExpression() override = default;

	void parse(Filereader& f, TokenStruct<std::string>& ts, Domain& d) override
	{
		f.next();
		left = createExpression( f, ts, d );
		right = createExpression( f, ts, d );
		f.next();
		f.assert_token( ")" );
	}

	[[nodiscard]] std::string info() const override
	{
		std::ostringstream os;
		os << "(" << op << " " << left->info() << " " << right->info() << ")";
		return os.str();
	}

	void PDDLPrint( std::ostream & s, unsigned indent, const TokenStruct< std::string > & ts, const Domain & d ) const override {
		s << "( " << op << " ";
		left->PDDLPrint( s, indent, ts, d );
		s << " ";
		right->PDDLPrint( s, indent, ts, d );
		s << " )";
	}

	double compute( double x, double y ) {
		double res = 0;

		if ( op == "+" ) res = x + y;
		else if ( op == "-" ) res = x - y;
		else if ( op == "*" ) res = x * y;
		else if ( op == "/" ) res = ( y == 0 ? 0 : x / y );

		return res;
	}

	double evaluate() override
	{
		return compute(left->evaluate(), right->evaluate());
	}

	double evaluate(Instance& ins, const StringVec& par) override
	{
		return compute(left->evaluate(ins, par), right->evaluate(ins, par));
	}

	IntSet params() override
	{
		IntSet lpars = left->params();
		IntSet rpars = right->params();
		lpars.insert(rpars.begin(), rpars.end());
		return lpars;
	}

	[[nodiscard]] std::shared_ptr<Condition> copy(const Domain& d) const override
	{
		auto cleft = std::dynamic_pointer_cast<Expression>(left->copy(d));
		auto cright = std::dynamic_pointer_cast<Expression>(right->copy(d));
		return std::make_shared<CompositeExpression>(op, cleft, cright);
	}
};

class FunctionExpression : public Expression
{
public:

	std::shared_ptr<ParamCond> fun;

	FunctionExpression(const std::shared_ptr<ParamCond>& c) : fun(c) {}

	~FunctionExpression() override = default;

	[[nodiscard]] std::string info() const override
	{
		std::ostringstream os;
		os << "(" << fun->name << fun->params << ")";
		return os.str();
	}

	void PDDLPrint( std::ostream & s, unsigned indent, const TokenStruct< std::string > & ts, const Domain & d ) const override;

	double evaluate() override { return 1; }

	double evaluate( Instance & ins, const StringVec & par ) override;

	IntSet params() override
	{
		return {fun->params.begin(), fun->params.end()};
	}

	[[nodiscard]] std::shared_ptr<Condition> copy(const Domain& d) const override
	{
		auto pc = std::dynamic_pointer_cast<ParamCond>(fun->copy(d));
		return std::make_shared<FunctionExpression>(pc);
	}
};

class ValueExpression : public Expression
{
public:

	double value;

	ValueExpression(double v) : value(v) {}

	[[nodiscard]] std::string info() const override
	{
		std::ostringstream os;
		os << value;
		return os.str();
	}

	void PDDLPrint(std::ostream& s, unsigned indent, const TokenStruct<std::string>& ts, const Domain& d) const override
	{
		s << value;
	}

	double evaluate() override { return value; }

	double evaluate(Instance& ins, const StringVec& par) override
	{
		return value;
	}

	IntSet params() override
	{
		return {};
	}

	[[nodiscard]] std::shared_ptr<Condition> copy(const Domain& d) const override
	{
		return std::make_shared<ValueExpression>(value);
	}
};

class DurationExpression : public Expression
{
	void PDDLPrint( std::ostream & s, unsigned indent, const TokenStruct< std::string > & ts, const Domain & d ) const override {
		s << "?DURATION";
	}

	[[nodiscard]] std::string info() const override
	{
		return "?DURATION";
	}

	double evaluate() override
	{
		return -1;
	}

	double evaluate( Instance & ins, const StringVec & par ) override
	{
		return evaluate();
	}

	IntSet params() override
	{
		return {};
	}

	[[nodiscard]] std::shared_ptr<Condition> copy(const Domain& d) const override
	{
		return std::make_shared<DurationExpression>();
	}
};

} } // namespaces
