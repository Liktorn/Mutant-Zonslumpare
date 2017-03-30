#pragma once
#include <string>
namespace bw {

	class Humanoider
	{
	public:
		Humanoider() : value(0) {};

		std::string text[16]{
			u8"Djurmutanter"
			, u8"Exilmutanter"
			, u8"Expedition(från annan Ark)"
			, u8"Kannibaler"
			, u8"Minneslös"
			, u8"Morlocker"
			, u8"Motorskallar"
			, u8"Patrull(från Folkets Ark)"
			, u8"Sekten Nova"
			, u8"Skrotorakel"
			, u8"Smittade"
			, u8"Undergångskult"
			, u8"Vandrare"
			, u8"Vattenhandlare"
			, u8"Vrakrövare"
			, u8"Zongastar"
		};
		int range[2][16]{
			{ 11,13,15,22,25,26,33,35,41,43,45,51,53,55,61,63 }
			,{ 12,14,21,24,25,32,34,36,42,44,46,52,54,56,62,66}
		};
		int value;
	};

} //namespace