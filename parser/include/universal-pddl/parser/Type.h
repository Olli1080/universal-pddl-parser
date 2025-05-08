
#pragma once

#include "TokenStruct.h"

namespace parser { namespace pddl {

class Type;

typedef std::vector<Type*> TypeVec;

class Type
{
public:

	std::string name;
	std::vector<std::weak_ptr<Type>> subtypes;
	std::weak_ptr<Type> supertype;

	TokenStruct< std::string > constants;
	TokenStruct< std::string > objects;

	Type() = default;

	Type(std::string s)
		: name(std::move(s)) {}

	Type(const Type& t)
		: name(t.name), constants(t.constants), objects(t.objects) {}

	virtual ~Type() = default;

	[[nodiscard]] virtual std::string getName() const
	{
		return name;
	}

	/*void insertSubtype(const std::shared_ptr<Type>& t) {
		subtypes.emplace_back(t);
		t->supertype = this->shared_from_this();
	}*/

	void copySubtypes(Type& t, const TokenStruct<std::shared_ptr<Type>>& ts)
	{
		for (auto& subtype : t.subtypes)
			subtypes.emplace_back(ts.get(subtype.lock()->name));
	}

	void print( std::ostream & stream ) const
	{
		stream << name;
		if (const auto lock = supertype.lock(); lock) 
			stream << "[" << lock->name << "]";
		stream << "\n";
	}

	virtual void PDDLPrint( std::ostream & s ) const
	{
		s << "\t" << name;
		if (const auto lock = supertype.lock(); lock) 
			s << " - " << lock->name;
		s << "\n";
	}

	std::pair<bool, int> parseConstant(const std::string& object)
	{
		int k = 0;

		int i = constants.index( object );
		if ( i < 0 ) k += constants.size();
		else return std::make_pair( true, -1 - i );

		for (auto& subtype : subtypes)
		{
			std::pair<bool, int> p = subtype.lock()->parseConstant(object);
			if ( p.first ) return std::make_pair(true, - k + p.second);
			else k += p.second;
		}

		return std::make_pair( false, k );
	}

	std::pair<bool, unsigned> parseObject(const std::string& object)
	{
		unsigned k = 0;

		int i = objects.index( object );
		if (i < 0) k += objects.size();
		else return std::make_pair( true, i );

		for (auto& subtype : subtypes)
		{
			std::pair<bool, unsigned> p = subtype.lock()->parseObject( object );
			if ( p.first ) return std::make_pair( true, k + p.second );
			else k += p.second;
		}

		return std::make_pair( false, k );
	}

	std::pair<std::string, int> object(int index)
	{
		if ( index < 0 ) {
			if ( -index <= (int)constants.size() ) return std::make_pair( constants[-1 - index], 0 );
			else index += constants.size();
		}
		else {
			if ( index < (int)objects.size() ) return std::make_pair( objects[index], 0 );
			else index -= objects.size();
		}

		for (auto& subtype : subtypes)
		{
			std::pair<std::string, int> p = subtype.lock()->object( index );
			if (!p.first.empty()) return p;
			else index = p.second;
		}

		return std::make_pair( "", index );
	}

	unsigned noObjects() const
	{
		unsigned total = objects.size() + constants.size();
		for (auto& subtype : subtypes)
			total += subtype.lock()->noObjects();
		return total;
	}

	unsigned noConstants() const
	{
		unsigned total = constants.size();
		for (auto& subtype : subtypes)
			total += subtype.lock()->noConstants();
		return total;
	}

	virtual std::shared_ptr<Type> copy()
	{
		return std::make_shared<Type>(*this);
	}

};

static void connect_types(const std::shared_ptr<Type>& super, const std::shared_ptr<Type>& sub)
{
	super->subtypes.emplace_back(sub);
	sub->supertype = super;
}

inline std::ostream & operator<<(std::ostream& stream, const Type& t)
{
	t.print(stream);
	return stream;
}

} } // namespaces
