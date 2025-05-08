
#pragma once

/*
	T is either a pointer or a string
	A token structure stores pointers but does not delete them!
*/

namespace parser { namespace pddl {

typedef std::map<std::string, size_t> TokenMap;

template <typename T>
inline std::string getName(const T& t)
{
	return t->name;
}

template <>
inline std::string getName<std::string>(const std::string& t)
{
	return t;
}

template <typename T>
class TokenStruct {

private:

	using Alloc = std::allocator<T>;
	using Alty = std::_Rebind_alloc_t<Alloc, T>;
	using Alty_traits = std::allocator_traits<Alty>;

public:

	using value_type = T;
	using allocator_type = Alloc;
	using pointer = typename Alty_traits::pointer;
	using const_pointer = typename Alty_traits::const_pointer;
	using reference = T&;
	using const_reference = const T&;
	using size_type = typename Alty_traits::size_type;
	using difference_type = typename Alty_traits::difference_type;

	std::vector<T, Alloc> tokens;
	TokenMap tokenMap;

	// represents the types of a typed list
	StringVec types;

	TokenStruct() = default;

	TokenStruct(const TokenStruct& ts)
		: tokens(ts.tokens), tokenMap(ts.tokenMap), types(ts.types) {}

	void append(const TokenStruct& ts)
	{
		for (unsigned i = 0; i < ts.size(); ++i)
			insert(ts[i]);
		types.insert(types.end(), ts.types.begin(), ts.types.end());
	}

	[[nodiscard]] size_type size() const
	{
		return tokens.size();
	}

	T& operator[](size_type i)
	{
		return tokens[i];
	}

	const T& operator[](size_type i) const
	{
		return tokens[i];
	}

	void clear()
	{
		//for (unsigned i = 0; i < size(); ++i)
			//delete tokens[i];

		tokens.clear();
		tokenMap.clear();
	}

	size_type insert(const T& t)
	{
		auto i = tokenMap.insert(tokenMap.begin(), std::make_pair(getName(t), size()));
		tokens.emplace_back(t);
		return i->second;
	}

	[[nodiscard]] int index(const std::string & s ) const
	{
		const auto i = tokenMap.find( s );
		return i == tokenMap.end() ? -1 : i->second;
	}

	[[nodiscard]] T get(const std::string & s) const
	{
		return tokens[index( s )];
	}


	[[nodiscard]] constexpr T* data() noexcept
	{
		return tokens.data();
	}

	[[nodiscard]] constexpr const T* data() const noexcept
	{
		return tokens.data();
	}

	[[nodiscard]] constexpr auto begin() noexcept
	{
		return tokens.begin();
	}

	[[nodiscard]] constexpr auto begin() const noexcept
	{
		return tokens.begin();
	}

	[[nodiscard]] constexpr auto end() noexcept
	{
		return tokens.end();
	}

	[[nodiscard]] constexpr auto end() const noexcept
	{
		return tokens.end();
	}

	[[nodiscard]] constexpr auto rbegin() noexcept
	{
		return tokens.rbegin();
	}

	[[nodiscard]] constexpr auto rbegin() const noexcept
	{
		return tokens.rbegin();
	}

	[[nodiscard]] constexpr auto rend() noexcept
	{
		return tokens.rend();
	}

	[[nodiscard]] constexpr auto rend() const noexcept
	{
		return tokens.rend();
	}

	[[nodiscard]] constexpr auto cbegin() const noexcept
	{
		return tokens.cbegin();
	}

	[[nodiscard]] constexpr auto cend() const noexcept
	{
		return tokens.cend();
	}

	[[nodiscard]] constexpr auto crbegin() const noexcept
	{
		return tokens.crbegin();
	}

	[[nodiscard]] constexpr auto crend() const noexcept
	{
		return tokens.crend();
	}

	[[nodiscard]] constexpr bool empty() const noexcept
	{
		return tokens.empty();
	}
};

} } // namespaces
