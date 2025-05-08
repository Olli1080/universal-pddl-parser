
#include "Domain.h"

namespace parser { namespace pddl {

void Action::PDDLPrint( std::ostream & s, unsigned indent, const TokenStruct< std::string > & ts, const Domain & d ) const {
	s << "( :ACTION " << name << "\n";

	s << "  :PARAMETERS ";

	TokenStruct< std::string > astruct;

	printParams( 0, s, astruct, d );

	s << "  :PRECONDITION\n";
	if ( pre ) pre->PDDLPrint( s, 1, astruct, d );
	else s << "\t()";
	s << "\n";

	s << "  :EFFECT\n";
	if ( eff ) eff->PDDLPrint( s, 1, astruct, d );
	else s << "\t()";
	s << "\n";

	s << ")\n";
}

void Action::parseConditions( Filereader & f, TokenStruct< std::string > & ts, Domain & d ) {
	f.next();
	f.assert_token( ":" );
	std::string s = f.getToken();
	if ( s == "PRECONDITION" ) {
		f.next();
		f.assert_token( "(" );
		if ( f.getChar() != ')' ) {
			pre = d.createCondition( f );
			pre->parse( f, ts, d );
		}
		else ++f.c;

		f.next();
		f.assert_token( ":" );
		s = f.getToken();
	}
	if ( s != "EFFECT" ) f.tokenExit( s );

	f.next();
	f.assert_token( "(" );
	if ( f.getChar() != ')' ) {
		eff = d.createCondition( f );
		eff->parse( f, ts, d );
	}
	else ++f.c;
	f.next();
	f.assert_token( ")" );
}

void Action::parse( Filereader & f, TokenStruct< std::string > & ts, Domain & d ) {
	f.next();
	f.assert_token( ":PARAMETERS" );
	f.assert_token( "(" );

	TokenStruct< std::string > astruct = f.parseTypedList( true, d.types );
	params = d.convertTypes( astruct.types );

	parseConditions( f, astruct, d );
}

std::vector<std::shared_ptr<Condition>> Action::precons()
{
	return getSubconditionsFromCondition(pre);
}

std::vector<std::shared_ptr<Condition>> Action::effects()
{
	return getSubconditionsFromCondition(eff);
}

std::vector<std::shared_ptr<Ground>> Action::addEffects()
{
	return getGroundsFromCondition(eff, false);
}

std::vector<std::shared_ptr<Ground>> Action::deleteEffects()
{
	return getGroundsFromCondition(eff, true);
}

std::vector<std::shared_ptr<Condition>> Action::getSubconditionsFromCondition(const std::shared_ptr<Condition>& c)
{
	auto a = std::dynamic_pointer_cast<And>(c);
	if (a) return a->conds;

	std::vector<std::shared_ptr<Condition>> subconds;
	if (c) subconds.emplace_back(c);
	return subconds;
}

std::vector<std::shared_ptr<Ground>> Action::getGroundsFromCondition(const std::shared_ptr<Condition>& c, bool neg ) {
	std::vector<std::shared_ptr<Ground>> grounds;
	auto a = std::dynamic_pointer_cast<And>(c);
	for ( unsigned i = 0; a && i < a->conds.size(); ++i ) {
		if ( neg ) {
			auto n = std::dynamic_pointer_cast<Not>(a->conds[i]);
			if (n) grounds.emplace_back(n->cond);
		}
		else {
			auto g = std::dynamic_pointer_cast<Ground>(a->conds[i]);
			if (g) grounds.emplace_back(g);
		}
	}

	if (neg) 
	{
		auto n = std::dynamic_pointer_cast<Not>(c);
		if (n) grounds.emplace_back(n->cond);
	}
	else 
	{
		auto g = std::dynamic_pointer_cast<Ground>(c);
		if (g) grounds.emplace_back(g);
	}

	return grounds;
}

} } // namespaces
