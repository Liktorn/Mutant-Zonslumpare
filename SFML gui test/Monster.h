#pragma once
#include <string>
namespace bw {

	class Monster
	{
	public:
		Monster() : value(0) {};

		std::string text[24]{
			u8"Asgnagare"
			, u8"Betesdjur"
			, u8"Dråparträd"
			, u8"Drömblomster"
			, u8"Etterbestar"
			, u8"Frätargräs"
			, u8"Jordslukare"
			, u8"Magtorsk(smittat vatten)"
			, u8"Maskinvarelse"
			, u8"Masksvärm"
			, u8"Parasitsvamp(smittad varelse)"
			, u8"Psi - mygg"
			, u8"Raggbest"
			, u8"Ruinspindel"
			, u8"Rötmyror"
			, u8"Skrädesvråk"
			, u8"Skövlare"
			, u8"Skrotkråkor"
			, u8"Strykarhundar"
			, u8"Tjärnmal"
			, u8"Zongetingar"
			, u8"Zoniglar"
			, u8"Zontörne"
			, u8"Ärgflyn"
		};
		int range[2][24]{
			{ 11,13,15,16,21,23,25,31,32,33,34,35,36,42,44,46,51,52,54,56,61,63,65,66 }
			,{ 12,14,15,16,22,24,26,31,32,33,34,35,41,43,45,56,51,53,55,56,62,64,65,66}
		};
		int value;
	};

} //namespace