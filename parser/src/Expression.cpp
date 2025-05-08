
#include "Instance.h"

namespace parser { namespace pddl {

void FunctionExpression::PDDLPrint( std::ostream & s, unsigned indent, const TokenStruct< std::string > & ts, const Domain & d ) const {
	std::shared_ptr<ParamCond> c = d.funcs[d.funcs.index( fun->name )];

	s << "( " << fun->name;
	IntVec v( c->params.size() );
	for ( unsigned i = 0; i < v.size(); ++i ) {
		if (!ts.empty() && fun->params[i] >= 0 ) s << " " << ts[fun->params[i]];
		else s << " " << d.types[c->params[i]]->object( fun->params[i] ).first;
	}
	s << " )";
}

double FunctionExpression::evaluate( Instance & ins, const StringVec & par ) {
	std::shared_ptr<ParamCond> c = ins.d.funcs[ins.d.funcs.index( fun->name )];

	IntVec v( c->params.size() );
	for ( unsigned i = 0; i < v.size(); ++i ) 
	{
		auto p = ins.d.types[c->params[i]]->parseObject( par[fun->params[i]] );
		if ( p.first ) v[i] = static_cast<int>(p.second);
		else {
			std::pair< bool, int > q = ins.d.types[c->params[i]]->parseConstant( par[fun->params[i]] );
			if ( q.first ) v[i] = q.second;
			else return 1;
		}
	}

	for (const auto& i : ins.init)
		if (i->name == c->name && i->params == v )
			return std::static_pointer_cast<GroundFunc<double>>(i)->value;
	return 1;
}

std::shared_ptr<Expression> createExpression(Filereader& f, TokenStruct<std::string>& ts, Domain& d)
{
	f.next();

	if ( f.getChar() == '(' ) {
		++f.c;
		f.next();
		std::string s = f.getToken();
		if ( s == "+" || s == "-" || s == "*" || s == "/" ) {
			auto ce = std::make_shared<CompositeExpression>(s);
			ce->parse( f, ts, d );
			return ce;
		}
		else {
			f.c -= s.size();
			auto fun = d.funcs.get( f.getToken( d.funcs ) );
			std::shared_ptr<ParamCond> c = std::make_shared<Lifted>(*fun);
			for ( unsigned i = 0; i < fun->params.size(); ++i ) {
				f.next();
				c->params[i] = ts.index( f.getToken( ts ) );
			}
			f.next();
			f.assert_token( ")" );
			return std::make_shared<FunctionExpression>(c);
		}
	}
	else if ( f.getChar() == '?' ) {  // just support DURATION if starts with ?
		f.assert_token( "?DURATION" );
		return std::make_shared<DurationExpression>();
	}
	else {
		double d;
		std::string s = f.getToken();
		std::istringstream is( s );
		is >> d;
		return std::make_shared<ValueExpression>(d);
	}
}

} }
