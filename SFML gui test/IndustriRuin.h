#pragma once
#include <string>
namespace bw {

	class IndustriRuin
	{
	public:
		IndustriRuin() : value(0) {};

		std::string text[12]{
			u8"Fabrik"
			, u8"Fartygsvrak"
			, u8"Kraftledning"
			, u8"Militärbas"
			, u8"Pipeline"
			, u8"Radiomast"
			, u8"Raffinaderi"
			, u8"Reningsverk"
			, u8"Skjutbana"
			, u8"Soptipp"
			, u8"Vindkraftverk"
			, u8"Oljecistern"
		};
		std::string prefix[12]{
			u8"11-13.\tFabrik"
			, u8"14-16.\tFartygsvrak"
			, u8"21-23.\tKraftledning"
			, u8"24-26.\tMilitärbas"
			, u8"31-33.\tPipeline"
			, u8"34-36.\tRadiomast"
			, u8"41-43.\tRaffinaderi"
			, u8"44-46.\tReningsverk"
			, u8"51-53.\tSkjutbana"
			, u8"54-56.\tSoptipp"
			, u8"61-63.\tVindkraftverk"
			, u8"64-66.\tOljecistern"
		};
		int range[2][12]{
			{ 11,14,21,24,31,34,41,44,51,54,61,64 }
			,{13,16,23,26,33,36,43,46,53,56,63,66 }
		};

		int value;
	};

} //namespace