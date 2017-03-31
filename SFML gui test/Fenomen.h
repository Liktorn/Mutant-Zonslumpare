#pragma once
#include <string>
namespace bw {

	class Fenomen
	{
	public:
		Fenomen() : value(0) {};

		std::string text[21]{
			u8"Askvind"
			, u8"Atomk�ld / Extremhetta"
			, u8"Blindg�ngare"
			, u8"Dammtornado"
			, u8"Elektrisk storm"
			, u8"Ljuspelare"
			, u8"Magnetf�lt"
			, u8"Mis�rvind"
			, u8"Nattsken"
			, u8"Obelisk"
			, u8"R�th�rd"
			, u8"Slukh�l"
			, u8"Splittervind"
			, u8"Sp�ksken"
			, u8"Synvilla"
			, u8"Syraregn"
			, u8"Tr�ghetsf�lt"
			, u8"Vakuum"
			, u8"Zong�lar"
			, u8"Zonsmog"
			, u8"Het k�lla"
		};
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
		int range[2][21]{
			 { 11,13,15,21,23,25,31,33,35,41,42,44,46,51,52,53,56,61,62,64,66 }
			,{ 12,14,16,22,24,26,32,34,36,41,43,45,46,51,52,55,56,61,63,65,66 }
		};

		int value;
	};

} //namespace