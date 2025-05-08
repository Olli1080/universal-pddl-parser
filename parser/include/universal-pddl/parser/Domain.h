
#pragma once

#include "Task.h"
#include "TemporalAction.h"
#include "And.h"
#include "Derived.h"
#include "Equals.h"
#include "Exists.h"
#include "Forall.h"
#include "Function.h"
#include "GroundFunc.h"
#include "FunctionModifier.h"

#include "Not.h"
#include "Oneof.h"
#include "Or.h"
#include "EitherType.h"
#include "When.h"

#define DOMAIN_DEBUG false

namespace parser { namespace pddl {

class Domain {
public:

	std::string name;                   // name of domain

	bool equality;                      // whether domain supports equality
	bool strips, adl, condeffects;      // whether domain is STRIPS, ADL and/or has conditional effects
	bool typed, cons, costs;            // whether domain is typed, has constants, has costs
	bool temp, nondet, neg, disj;       // whether domain is temporal, is non-deterministic, has negative precons, has disjunctive preconditions
	bool universal;                     // whether domain has universal precons
	bool fluents;                       // whether domains contains fluents
	bool derivedpred;                   // whether domain contains derived predicates

	TokenStruct<std::shared_ptr<Type>> types;        // types
	TokenStruct<std::shared_ptr<Lifted>> preds;      // predicates
	TokenStruct<std::shared_ptr<Function>> funcs;    // functions
	TokenStruct<std::shared_ptr<Action>> actions;    // actions
	TokenStruct<std::shared_ptr<Derived>> derived;   // derived predicates
	TokenStruct<std::shared_ptr<Task>> tasks;        // tasks

	Domain()
		: equality( false ), strips( false ), adl( false ), condeffects( false )
		, typed( false ), cons( false ), costs( false ), temp( false )
		, nondet( false ), neg( false ), disj( false ), universal( false )
		, fluents( false ), derivedpred( false )
	{
		types.insert(std::make_shared<Type>("OBJECT")); // Type 0 is always "OBJECT", whether the domain is typed or not
	}

	Domain( const std::string & s ) : Domain()
	{
		parse( s );
	}

	virtual ~Domain() = default;

	virtual void parse( const std::string & s ) {
		Filereader f( s );
		name = f.parseName( "DOMAIN" );

		if constexpr ( DOMAIN_DEBUG ) std::cout << name << "\n";

		for ( ; f.getChar() != ')'; f.next() ) {
			f.assert_token( "(" );
			f.assert_token( ":" );
			std::string t = f.getToken();

			if constexpr ( DOMAIN_DEBUG ) std::cout << t << "\n";

			if (!parseBlock(t, f)) {
				f.tokenExit( t );
			}
		}
	}

	//! Returns a boolean indicating whether the block was correctly parsed
	virtual bool parseBlock(const std::string& t, Filereader& f) {
		if ( t == "REQUIREMENTS" ) parseRequirements( f );
		else if ( t == "TYPES" ) parseTypes( f );
		else if ( t == "CONSTANTS" ) parseConstants( f );
		else if ( t == "PREDICATES" ) parsePredicates( f );
		else if ( t == "FUNCTIONS" ) parseFunctions( f );
		else if ( t == "ACTION" ) parseAction( f );
		else if ( t == "DURATIVE-ACTION" ) parseDurativeAction( f );
		else if ( t == "DERIVED" ) parseDerived( f );
//		else if ( t == "AXIOM" ) parseAxiom( f );
		else return false; // Unknown block type

		return true;
	}


	void parseRequirements( Filereader & f ) {
		for ( f.next(); f.getChar() != ')'; f.next() ) {
			f.assert_token( ":" );
			std::string s = f.getToken();

			if constexpr ( DOMAIN_DEBUG ) std::cout << "  " << s << "\n";

			if (!parseRequirement(s)) {
				f.tokenExit( s );
			}
		}

		++f.c;
	}

	//! Returns a boolean indicating whether the requirement was correctly parsed
	virtual bool parseRequirement( const std::string& s ) {
		if ( s == "STRIPS" ) strips = true;
		else if ( s == "ADL" ) adl = true;
		else if ( s == "NEGATIVE-PRECONDITIONS" ) neg = true;
		else if ( s == "CONDITIONAL-EFFECTS" ) condeffects = true;
		else if ( s == "TYPING" ) typed = true;
		else if ( s == "ACTION-COSTS" ) costs = true;
		else if ( s == "EQUALITY" ) equality = true;
		else if ( s == "DURATIVE-ACTIONS" ) temp = true;
		else if ( s == "NON-DETERMINISTIC" ) nondet = true;
		else if ( s == "UNIVERSAL-PRECONDITIONS" ) universal = true;
		else if ( s == "FLUENTS" ) fluents = true;
		else if ( s == "DISJUNCTIVE-PRECONDITIONS" ) disj = true;
		else if ( s == "DERIVED-PREDICATES" ) derivedpred = true;
		else return false; // Unknown requirement

		return true;
	}

	// get the type corresponding to a string
	std::shared_ptr<Type> getType(const std::string& s)
	{
		int i = types.index( s );
		if ( i < 0 ) {
			if ( s[0] == '(' ) {
				i = types.insert(std::make_shared<EitherType>(s));
				for ( unsigned k = 9; s[k] != ')'; ) {
					unsigned e = s.find( ' ', k );
					types[i]->subtypes.emplace_back(getType(s.substr(k, e - k )));
					k = e + 1;
				}
			}
			else i = types.insert(std::make_shared<Type>(s));
		}
		return types[i];
	}

	// convert a vector of type names to integers
	IntVec convertTypes(const StringVec& v)
	{
		IntVec out;
		for (const auto& i : v)
			out.emplace_back(types.index(getType(i)->name));
		return out;
	}

	void parseTypes(Filereader& f)
	{
		if (!typed)
		{
			std::cout << "Requirement :TYPING needed to define types\n";
			exit( 1 );
		}

//		if this makes it in, probably need to define new subclass of Type
//		if ( costs ) insert( new Type( "NUMBER" ), tmap, types );

		// Parse the typed list
		TokenStruct< std::string > ts = f.parseTypedList( false );

		// bit of a hack to avoid OBJECT being the supertype
		if ( ts.index( "OBJECT" ) >= 0 ) 
		{
			types[0]->name = "SUPERTYPE";
			types.tokenMap.clear();
			types.tokenMap["SUPERTYPE"] = 0;
		}

		// Relate subtypes and supertypes
		for ( unsigned i = 0; i < ts.size(); ++i ) 
		{
			if (!ts.types[i].empty())
			{
				auto t1 = getType(ts.types[i]);
				auto type = getType(ts[i]);
				connect_types(t1, type);
				//t1->insertSubtype(type);
			}
			else getType( ts[i] );
		}

		// By default, the supertype of a type is "OBJECT"
		for (unsigned i = 1; i < types.size(); ++i)
			if (!types[i]->supertype.lock())
				connect_types(types[0], types[i]);

		for ( unsigned i = 0; DOMAIN_DEBUG && i < types.size(); ++i )
			std::cout << "  " << types[i];
	}

	void parseConstants( Filereader & f ) {
		if ( typed && types.empty()) {
			std::cout << "Types needed before defining constants\n";
			exit( 1 );
		}

		cons = true;

		TokenStruct< std::string > ts = f.parseTypedList( true, types );

		for ( unsigned i = 0; i < ts.size(); ++i )
			getType( ts.types[i] )->constants.insert( ts[i] );

		for ( unsigned i = 0; DOMAIN_DEBUG && i < types.size(); ++i ) {
			std::cout << " ";
			if ( typed ) std::cout << " " << types[i] << ":";
			for (const auto& constant : types[i]->constants)
				std::cout << " " << constant;
			std::cout << "\n";
		}
	}

	void parsePredicates(Filereader& f)
	{
		if (typed && types.empty()) 
		{
			std::cout << "Types needed before defining predicates\n";
			exit(1);
		}

		for ( f.next(); f.getChar() != ')'; f.next() ) 
		{
			f.assert_token( "(" );
			if ( f.getChar() == ':' ) {
				// Needed to support MA-PDDL
				f.assert_token( ":PRIVATE" );
				f.parseTypedList( true, types, "(" );

				// CURRENT HACK: TOTALLY IGNORE PRIVATE !!!
				--f.c;
				parsePredicates( f );
			}
			else {
				auto c = std::make_shared<Lifted>(f.getToken());
				c->parse(f, types[0]->constants, *this);

				if constexpr ( DOMAIN_DEBUG ) std::cout << "  " << c;
				preds.insert(c);
			}
		}
		++f.c;
	}

	void parseFunctions(Filereader& f)
	{
		if ( typed && types.empty()) {
			std::cout << "Types needed before defining functions\n";
			exit(1);
		}

		for ( f.next(); f.getChar() != ')'; f.next() ) {
			f.assert_token( "(" );
			auto c = std::make_shared<Function>(f.getToken());
			c->parse( f, types[0]->constants, *this );

			if constexpr ( DOMAIN_DEBUG ) std::cout << "  " << c;
			funcs.insert( c );
		}
		++f.c;
	}

	virtual void parseAction( Filereader & f ) {
		if (preds.empty()) {
			std::cout << "Predicates needed before defining actions\n";
			exit(1);
		}

		f.next();
		auto a = std::make_shared<Action>(f.getToken());
		a->parse( f, types[0]->constants, *this );

		if constexpr ( DOMAIN_DEBUG ) std::cout << a << "\n";
		actions.insert( a );
	}

	void parseDerived( Filereader & f )
	{
		if (preds.empty()) 
		{
			std::cout << "Predicates needed before defining derived predicates\n";
			exit(1);
		}

		f.next();
		auto d = std::make_shared<Derived>();
		d->parse( f, types[0]->constants, *this );

		if constexpr ( DOMAIN_DEBUG ) std::cout << d << "\n";
		derived.insert( d );
	}

	void parseDurativeAction( Filereader & f )
	{
		if (preds.empty()) 
		{
			std::cout << "Predicates needed before defining actions\n";
			exit(1);
		}

		f.next();
		auto a = std::make_shared<TemporalAction>(f.getToken());
		a->parse( f, types[0]->constants, *this );

		if constexpr ( DOMAIN_DEBUG ) std::cout << a << "\n";
		actions.insert( a );
	}


	// Return a copy of the type structure, with newly allocated types
	// This will also copy all constants and objects!
	TokenStruct<std::shared_ptr<Type>> copyTypes()
	{
		TokenStruct<std::shared_ptr<Type>> out;
		for (const auto& type : types)
			out.insert(type->copy() );

		for ( unsigned i = 1; i < types.size(); ++i ) {
			if (auto lock = types[i]->supertype.lock(); lock)
				connect_types(out[out.index(lock->name)], out[i]);
			else
				out[i]->copySubtypes( *types[i], out );
		}

		return out;
	}

	// Set the types to "otherTypes"
	void setTypes( const TokenStruct<std::shared_ptr<Type>> & otherTypes )
	{
		types = otherTypes;
	}

	// Create a type with a given supertype (default is "OBJECT")
	void createType(const std::string& name, const std::string& parent = "OBJECT")
	{
		auto type = std::make_shared<Type>(name);
		types.insert( type );
		connect_types(types.get(parent), type);
	}

	// Create a constant of a given type
	void createConstant( const std::string & name, const std::string & type )
	{
		types.get( type )->constants.insert( name );
	}

	// Create a predicate with the given name and parameter types
	std::shared_ptr<Lifted> createPredicate(const std::string& name, const StringVec& params = StringVec())
	{
		auto pred = std::make_shared<Lifted>(name);
		for (const auto& param : params)
			pred->params.push_back( types.index(param) );
		preds.insert( pred );
		return pred;
	}

	// Create a function with the given name and parameter types
	std::shared_ptr<Lifted> createFunction( const std::string & name, int type, const StringVec & params = StringVec() )
	{
		auto func = std::make_shared<Function>(name, type);
		for (const auto& param : params)
			func->params.push_back( types.index(param) );
		funcs.insert( func );
		return func;
	}

	// Create an action with the given name and parameter types
	std::shared_ptr<Action> createAction( const std::string & name, const StringVec & params = StringVec() ) {
		auto action = std::make_shared<Action>(name);
		for (const auto& param : params)
			action->params.push_back( types.index(param) );
		action->pre = std::make_shared<And>();
		action->eff = std::make_shared<And>();
		actions.insert( action );
		return action;
	}

	// Set the precondition of an action to "cond", converting to "And"
	void setPre(const std::string& act, const std::shared_ptr<Condition>& cond) 
	{
		auto action = actions.get(act);

		auto old = std::dynamic_pointer_cast<And>(cond);
		if (!old) 
		{
			action->pre = std::make_shared<And>();
			if (cond) std::dynamic_pointer_cast<And>(action->pre)->add( cond->copy( *this ) );
		}
		else action->pre = old->copy( *this );
	}

	// Add a precondition to the action with name "act"
	void addPre( bool neg, const std::string & act, const std::string & pred, const IntVec & params = IntVec() ) {
		auto action = actions.get( act );
		if (!action->pre) action->pre = std::make_shared<And>();
		auto a = std::dynamic_pointer_cast<And>(action->pre);
		if ( neg ) a->add( std::make_shared<Not>( ground( pred, params ) ) );
		else a->add( ground( pred, params ) );
	}

	// Add an "OR" precondition to the action with name "act"
	void addOrPre( const std::string & act, const std::string & pred1, const std::string & pred2,
				   const IntVec & params1 = IntVec(), const IntVec & params2 = IntVec() )
	{
		auto o = std::make_shared<Or>();
		o->first = ground( pred1, params1 );
		o->second = ground( pred2, params2 );
		auto action = actions.get( act );
		auto a = std::dynamic_pointer_cast<And>(action->pre);
		a->add(o);
	}

	// Set the precondition of an action to "cond", converting to "And"
	void setEff( const std::string & act, const std::shared_ptr<Condition>& cond )
	{
		auto action = actions.get( act );

		auto old = std::dynamic_pointer_cast<And>(cond);
		if (!old) 
		{
			action->eff = std::make_shared<And>();
			if ( cond ) std::dynamic_pointer_cast<And>(action->eff)->add( cond->copy( *this ) );
		}
		else action->eff = old->copy( *this );
	}

	// Add an effect to the action with name "act"
	void addEff( bool neg, const std::string & act, const std::string & pred, const IntVec & params = IntVec() )
	{
		auto action = actions.get( act );
		if (!action->eff) action->eff = std::make_shared<And>();
		auto a = std::dynamic_pointer_cast<And>(action->eff);
		if ( neg ) a->add( std::make_shared<Not>( ground( pred, params ) ) );
		else a->add( ground( pred, params ) );
	}

	// Add a cost to the action with name "act", in the form of an integer
	void addCost( const std::string & act, int cost )
	{
		auto action = actions.get( act );
		if (!action->eff) action->eff = std::make_shared<And>();
		auto a = std::dynamic_pointer_cast<And>(action->eff);
		a->add(std::make_shared<Increase>(cost));
	}

	// Add a cost to the action with name "act", in the form of a function
	void addCost( const std::string & act, const std::string & func, const IntVec & params = IntVec() )
	{
		auto action = actions.get( act );
		if (!action->eff) action->eff = std::make_shared<And>();
		auto a = std::dynamic_pointer_cast<And>(action->eff);
		a->add( std::make_shared<Increase>( funcs.get( func ), params ) );
	}

	void addFunctionModifier( const std::string & act, const std::shared_ptr<FunctionModifier>& fm)
	{
		auto action = actions.get( act );
		if (!action->eff) action->eff = std::make_shared<And>();
		auto a = std::dynamic_pointer_cast<And>(action->eff);
		a->add(fm);
	}

	// Create a ground condition with the given name
	std::shared_ptr<Ground> ground(const std::string& name, const IntVec& params = IntVec())
	{
		if ( preds.index( name ) < 0 ) 
		{
			std::cout << "Creating a ground condition " << name << params;
			std::cout << " failed since the predicate " << name << " does not exist!\n";
			std::exit( 1 );
		}
		return std::make_shared<Ground>( preds[preds.index( name )], params );
	}

	// Return the list of type names corresponding to a parameter list
	[[nodiscard]] StringVec typeList(const ParamCond& c) const
	{
		StringVec out;
		for (int param : c.params)
			out.emplace_back( types[param]->name );
		return out;
	}

	// Return the list of object names corresponding to a ground fluent
	[[nodiscard]] StringVec objectList(const Ground& g) const
	{
		StringVec out;
		for ( unsigned i = 0; i < g.params.size(); ++i )
			out.emplace_back( types[g.lifted.lock()->params[i]]->object( g.params[i] ).first );
		return out;
	}

	// Add parameters to an action
	void addParams(const std::string& name, const StringVec& v)
	{
		actions.get(name)->addParams( convertTypes(v));
	}

	// Assert that one type is a subtype of another
	bool assertSubtype(int t1, int t2)
	{
		for (auto type = types[t1]; type; type = type->supertype.lock())
			if (type->name == types[t2]->name) return true;
		return false;
	}

	// return the index of a constant for a given type
	[[nodiscard]] int constantIndex(const std::string& name, const std::string& type) const
	{
		return types.get(type)->parseConstant(name).second;
	}

	//! Prints a PDDL representation of the object to the given stream.
	friend std::ostream& operator<<(std::ostream &os, const Domain& o) { return o.print(os); }
	virtual std::ostream& print(std::ostream& os) const
	{
		os << "( DEFINE ( DOMAIN " << name << " )\n";
		print_requirements(os);

		if ( typed ) 
		{
			os << "( :TYPES\n";
			for ( unsigned i = 1; i < types.size(); ++i )
				types[i]->PDDLPrint( os );
			os << ")\n";
		}

		if ( cons ) 
		{
			os << "( :CONSTANTS\n";
			for (const auto& type : types)
			{
				if (!type->constants.empty()) {
					os << "\t";
					for (const auto& constant : type->constants)
						os << constant << " ";
					if (typed)
						os << "- " << type->name;
					os << "\n";
				}
			}
			os << ")\n";
		}

		os << "( :PREDICATES\n";
		for (const auto& pred : preds)
		{
			pred->PDDLPrint( os, 1, TokenStruct< std::string >(), *this );
			os << "\n";
		}
		os << ")\n";

		if (!funcs.empty()) {
			os << "( :FUNCTIONS\n";
			for (const auto& func : funcs)
			{
				func->PDDLPrint( os, 1, TokenStruct< std::string >(), *this );
				os << "\n";
			}
			os << ")\n";
		}

		for (const auto& action : actions)
			action->PDDLPrint( os, 0, TokenStruct< std::string >(), *this );

		for (const auto& i : derived)
			i->PDDLPrint( os, 0, TokenStruct< std::string >(), *this );

		print_addtional_blocks(os);

		os << ")\n";
		return os;
	}

	virtual std::ostream& print_requirements(std::ostream& os) const {
		os << "( :REQUIREMENTS";
		if ( equality ) os << " :EQUALITY";
		if ( strips ) os << " :STRIPS";
		if ( costs ) os << " :ACTION-COSTS";
		if ( adl ) os << " :ADL";
		if ( neg ) os << " :NEGATIVE-PRECONDITIONS";
		if ( condeffects ) os << " :CONDITIONAL-EFFECTS";
		if ( typed ) os << " :TYPING";
		if ( temp ) os << " :DURATIVE-ACTIONS";
		if ( nondet ) os << " :NON-DETERMINISTIC";
		if ( universal ) os << " :UNIVERSAL-PRECONDITIONS";
		if ( fluents ) os << " :FLUENTS";
		if ( disj ) os << " :DISJUNCTIVE-PRECONDITIONS";
		if ( derivedpred ) os << " :DERIVED-PREDICATES";
		os << " )\n";
		return os;
	}

	virtual std::ostream& print_addtional_blocks(std::ostream& os) const { return os; }

	virtual std::shared_ptr<Condition> createCondition(Filereader& f)
	{
		std::string s = f.getToken();

		if ( s == "=" ) return std::make_shared<Equals>();
		if ( s == "AND" ) return std::make_shared<And>();
		if ( s == "EXISTS" ) return std::make_shared<Exists>();
		if ( s == "FORALL" ) return std::make_shared<Forall>();
		if ( s == "INCREASE" ) return std::make_shared<Increase>();
		if ( s == "DECREASE" ) return std::make_shared<Decrease>();
		if ( s == "NOT" ) return std::make_shared<Not>();
		if ( s == "ONEOF" ) return std::make_shared<Oneof>();
		if ( s == "OR" ) return std::make_shared<Or>();
		if ( s == "WHEN" ) return std::make_shared<When>();
		if ( s == ">=" || s == ">" || s == "<=" || s == "<" ) return std::make_shared<CompositeExpression>(s);

		int i = preds.index(s);
		if (i >= 0) return std::make_shared<Ground>(preds[i]);

		f.tokenExit(s);

		return nullptr;
	}
};

} } // namespaces
