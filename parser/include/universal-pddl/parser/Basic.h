
#pragma once

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <numbers>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

constexpr auto PI = std::numbers::pi;

typedef std::set< int > IntSet;
typedef std::vector< int > IntVec;
typedef std::set< double > DoubleSet;
typedef std::vector< IntSet > SetVec;
typedef std::pair< int, int > IntPair;
typedef std::vector< IntPair > PairVec;
typedef std::map< int, IntSet > SetMap;
typedef std::vector< double > DoubleVec;
typedef std::vector< size_t > UnsignedVec;
typedef std::vector< std::string > StringVec;
typedef std::pair< double, double > DoublePair;

// create a vector of integers from lo to (hi-1)
template<typename T0, typename T1>
inline IntVec incvec(T0 lo, T1 hi)
{
	IntVec out;
	out.reserve(hi - lo);
	for (int i = static_cast<int>(lo); i < static_cast<int>(hi); ++i)
		out.emplace_back(i);
	return out;
}

// insert all elements of a collection into a stream
template < typename T >
std::ostream & insertAll( std::ostream & stream, const T & begin, const T & end ) {
	T i = begin;
	stream << "[";
	if ( i != end ) stream << *( i++ );
	while ( i != end ) stream << "," << *( i++ );
	return stream << "]";
}

// insert a pair into a stream
template < typename T, typename U >
std::ostream & operator<<( std::ostream & stream, const std::pair< T, U > & p ) {
	return stream << "(" << p.first << "," << p.second << ")";
}

// insert a list into a stream
template < typename T >
std::ostream & operator<<( std::ostream & stream, const std::list< T > & l ) {
	return insertAll( stream, l.begin(), l.end() );
}

// insert a map into a stream
template < typename T, typename U >
std::ostream & operator<<( std::ostream & stream, const std::map< T, U > & m ) {
	return insertAll( stream, m.begin(), m.end() );
}

// insert a multiset into a stream
template < typename T >
std::ostream & operator<<( std::ostream & stream, const std::multiset< T > & s ) {
	return insertAll( stream, s.begin(), s.end() );
}

// insert a set into a stream
template < typename T >
std::ostream & operator<<( std::ostream & stream, const std::set< T > & s ) {
	return insertAll( stream, s.begin(), s.end() );
}

// insert a vector into a stream
template < typename T >
std::ostream & operator<<( std::ostream & stream, const std::vector< T > &v ) {
	return insertAll( stream, v.begin(), v.end() );
}


inline void tabindent( std::ostream & stream, unsigned indent ) {
	for ( unsigned i = 0; i < indent; ++i )
		stream << "\t";
}
