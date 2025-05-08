
#include "Instance.h"

namespace parser { namespace pddl {

std::shared_ptr<Expression> TemporalAction::parseDuration(Filereader& f, TokenStruct<std::string>& ts, Domain& d)
{
	return createExpression(f, ts, d);
}

void TemporalAction::printCondition(std::ostream& s, const TokenStruct<std::string>& ts, const Domain& d,
									 const std::string& t, const And& a) const
{
	for (const auto& cond : a.conds)
	{
		s << "\t\t( " << t << " ";
		cond->PDDLPrint(s, 0, ts, d);
		s << " )\n";
	}
}

void TemporalAction::PDDLPrint(std::ostream& s, unsigned indent, const TokenStruct<std::string>& ts, const Domain& d) const
	{
	s << "( :DURATIVE-ACTION " << name << "\n";

	s << "  :PARAMETERS ";

	TokenStruct<std::string> astruct;

	printParams(0, s, astruct, d);

	s << "  :DURATION ( = ?DURATION ";
	if (durationExpr) durationExpr->PDDLPrint(s, 0, astruct, d);
	else s << "1";
	s << " )\n";

	s << "  :CONDITION\n";
	s << "\t( AND\n";
	if (pre) printCondition( s, astruct, d, "AT START", *std::static_pointer_cast<And>(pre));
	if (pre_o) printCondition( s, astruct, d, "OVER ALL", *pre_o );
	if (pre_e) printCondition( s, astruct, d, "AT END", *pre_e );
	s << "\t)\n";

	s << "  :EFFECT\n";
	s << "\t( AND\n";
	if (eff) printCondition( s, astruct, d, "AT START", *std::static_pointer_cast<And>(eff));
	if (eff_e) printCondition( s, astruct, d, "AT END", *eff_e );
	s << "\t)\n";

	s << ")\n";
}

void TemporalAction::parseCondition(Filereader& f, TokenStruct<std::string>& ts, Domain& d, And& a)
{
	f.next();
	f.assert_token("(");
	auto c = d.createCondition(f);
	c->parse( f, ts, d );
	a.conds.emplace_back( c );
}

void TemporalAction::parse( Filereader & f, TokenStruct< std::string > & ts, Domain & d ) {
	f.next();
	f.assert_token( ":PARAMETERS" );
	f.assert_token( "(" );

	TokenStruct< std::string > astruct = f.parseTypedList( true, d.types );
	params = d.convertTypes( astruct.types );

	f.next();
	f.assert_token( ":DURATION" );
	f.assert_token( "(" );
	f.assert_token( "=" );
	f.assert_token( "?DURATION" );
	durationExpr = parseDuration( f, astruct, d );
	f.next();
	f.assert_token( ")" );

	f.next();
	f.assert_token( ":" );
	std::string s = f.getToken();
	if ( s == "CONDITION" ) {
		pre = std::make_shared<And>();
		pre_o = std::make_shared<And>();
		pre_e = std::make_shared<And>();
		f.next();
		f.assert_token( "(" );
		if ( f.getChar() != ')' ) {
			s = f.getToken();
			if ( s == "AND" ) {
				for ( f.next(); f.getChar() != ')'; f.next() ) {
					f.assert_token( "(" );
					s = f.getToken();
					f.next();
					std::string t = f.getToken();

					if ( s == "AT" && t == "START" )
						parseCondition( f, astruct, d, *std::dynamic_pointer_cast<And>(pre));
					else if ( s == "OVER" && t == "ALL" )
						parseCondition( f, astruct, d, *pre_o);
					else if ( s == "AT" && t == "END" )
						parseCondition( f, astruct, d, *pre_e);
					else f.tokenExit( s + " " + t );

					f.next();
					f.assert_token( ")" );
				}
				++f.c;
			}
			else {
				f.next();
				std::string t = f.getToken();

				if ( s == "AT" && t == "START" )
					parseCondition( f, astruct, d, *std::dynamic_pointer_cast<And>(pre));
				else if ( s == "OVER" && t == "ALL" )
					parseCondition( f, astruct, d, *pre_o );
				else if ( s == "AT" && t == "END" )
					parseCondition( f, astruct, d, *pre_e );
				else f.tokenExit( s + " " + t );

				f.next();
				f.assert_token( ")" );
			}
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
		eff = std::make_shared<And>();
		eff_e = std::make_shared<And>();

		s = f.getToken();
		if ( s == "AND" ) {
			for ( f.next(); f.getChar() != ')'; f.next() ) {
				f.assert_token( "(" );
				s = f.getToken();
				f.next();
				std::string t = f.getToken();

				if ( s == "AT" && t == "START" )
					parseCondition( f, astruct, d, *std::dynamic_pointer_cast<And>(eff));
				else if ( s == "AT" && t == "END" )
					parseCondition( f, astruct, d, *eff_e );
				else f.tokenExit( s + " " + t );

				f.next();
				f.assert_token( ")" );
			}
			++f.c;
		}
		else {
			f.next();
			std::string t = f.getToken();

			if ( s == "AT" && t == "START" )
				parseCondition( f, astruct, d, *std::dynamic_pointer_cast<And>(eff));
			else if ( s == "AT" && t == "END" )
				parseCondition( f, astruct, d, *eff_e );
			else f.tokenExit( s + " " + t );

			f.next();
			f.assert_token( ")" );
		}
	}
	else ++f.c;

	f.next();
	f.assert_token( ")" );
}

std::vector<std::shared_ptr<Ground>> TemporalAction::preconsStart()
{
	return getGroundsFromCondition( pre, false );
}

std::vector<std::shared_ptr<Ground>> TemporalAction::preconsOverall()
{
	return getGroundsFromCondition( pre_o, false );
}

std::vector<std::shared_ptr<Ground>> TemporalAction::preconsEnd()
{
	return getGroundsFromCondition( pre_e, false );
}

std::vector<std::shared_ptr<Condition>> TemporalAction::endEffects()
{
	return getSubconditionsFromCondition( eff_e );
}

std::vector<std::shared_ptr<Ground>> TemporalAction::addEndEffects()
{
	return getGroundsFromCondition( eff_e, false );
}

std::vector<std::shared_ptr<Ground>> TemporalAction::deleteEndEffects()
{
	return getGroundsFromCondition( eff_e, true );
}

} } // namespaces
