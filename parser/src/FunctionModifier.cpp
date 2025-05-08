
#include "Domain.h"

namespace parser { namespace pddl {

FunctionModifier::FunctionModifier(std::string name, int val)
	: name(std::move(name)), modifierExpr(std::make_shared<ValueExpression>(val)) {}

FunctionModifier::FunctionModifier(std::string name, const std::shared_ptr<Function>& f, const IntVec& p)
	: name(std::move(name)), modifierExpr(std::make_shared<FunctionExpression>(std::make_shared<Ground>(f, p))) {}

FunctionModifier::FunctionModifier(std::string name, const FunctionModifier& i, Domain& d)
	: name(std::move(name))
{
	if (i.modifiedGround) 
	{
		modifiedGround = std::dynamic_pointer_cast<Ground>(i.modifiedGround->copy(d));
	}
	else 
		modifiedGround.reset();

	if (i.modifierExpr) 
	{
		modifierExpr = std::dynamic_pointer_cast<Expression>(i.modifierExpr->copy(d));
	}
	else 
		modifierExpr.reset();
}

void FunctionModifier::PDDLPrint( std::ostream & s, unsigned indent, const TokenStruct< std::string > & ts, const Domain & d ) const {
	tabindent( s, indent );
	s << "( " << name << " ";

	if ( modifiedGround ) {
		modifiedGround->PDDLPrint( s, 0, ts, d );
	}
	else {
		s << "( TOTAL-COST )";
	}

	s << " ";
	modifierExpr->PDDLPrint( s, 0, ts, d );

	s << " )";
}

void FunctionModifier::parse(Filereader & f, TokenStruct<std::string>& ts, Domain& d)
{
	f.next();

	f.assert_token( "(" );

	std::string increasedFunction = f.getToken();
	if (increasedFunction == "TOTAL-COST") 
	{
 		f.next();
		f.assert_token( ")" );
	}
	else 
	{
		modifiedGround = std::make_shared<Ground>(d.funcs.get(increasedFunction));
		modifiedGround->parse(f, ts, d);
	}

	f.next();

	modifierExpr = createExpression(f, ts, d);

	f.next();
	f.assert_token(")");
}

} } // namespaces
