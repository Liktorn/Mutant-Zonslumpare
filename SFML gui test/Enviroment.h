#pragma once
#include <string>
namespace bw{

class Enviroment
{
public:
	Enviroment() : value(0) {};

	std::string text[13]{
		u8"Tät skog"
		, u8"Busklandskap"
		, u8"Träsk"
		, u8"Död skog"
		, u8"Asköken"
		, u8"Enorm krater"
		, u8"Fält av glas"
		, u8"Övervuxna ruiner"
		, u8"Söndervittrade ruiner"
		, u8"Förfallna ruiner"
		, u8"Välbevarade ruiner"
		, u8"Öde industrilandskap"
		, u8"Bosättning" } ;
	std::string prefix[13]{
		u8"11-12"
		, u8"13-15"
		, u8"16-21"
		, u8"22-24"
		, u8"25-26"
		, u8"31"
		, u8"32"
		, u8"33-35"
		, u8"36-42"
		, u8"43-51"
		, u8"52-56"
		, u8"61-64"
		, u8"65-66"
	};
	int range[2][13]{
		 { 11, 13, 16, 22, 25, 31, 32, 33, 36, 43, 52, 61, 65 }
		,{ 12, 15, 21, 24, 26, 31, 32, 35, 42, 51, 56, 64, 66}	
	};

	int value;
};

} //namespace