#include "imgui.h"
#include "imgui-sfml.h"


#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <iostream>
#include <time.h>
#include <string>
#include <vector>

#include "header.h"
#include "Enviroment.h"
#include "StandardRuin.h"
#include "IndustriRuin.h"
#include "Humanoider.h"
#include "Monster.h"
#include "Fenomen.h"

#define ARRAY_SIZE(array) (sizeof((array))/sizeof((array[0])))

// Show text bools.
bool skapa_sektor = true;
bool sektor_miljo = false;
bool sektor_ruin = false;
bool sektor_rota = false;
bool sektor_hot = false;
bool sektor_random_hot = false;
bool sektor_hot_i_zonen = false;
bool is_humanoider = false;
bool is_monster = false;
bool is_fenomen = false;
bool sammanstallning = false;
bool is_bosattning = false;
bool quit_program = false;
bool slumpa_alla = false;

// Om det skall finnas hot, ruiner och artefakter i zonen
bool ruin_standard = false;
bool ruin_industri = false;
bool hot = false;
bool artefakt = false;
// Hur m�nga hot och artefakter
int randHot = 0;
int randArtefakt = 0;
ImVec2 ButtonSize(100, 30);




// s�tter R�tniv�
std::string rotStr[]{
	u8"R�t-oas. RP tar inga r�tpo�ng."
	, u8"Typisk zonsektor. RP tar en r�tpo�ng varje dygn."
	, u8"Extra r�tdrabbad sektor. En r�tpo�ng per timme."
};
static int rotValue = 0;

// s�tter hotniv�
std::string hotNiva[]{
	u8"1. Zonens ytteromr�den"
	, u8"2. Zonens ytteromr�den"
	, u8"3. Zonens ytteromr�den"
	, u8"4. Zonens ytteromr�den"
	, u8"5. Zonens centrala delar"
	, u8"6. Zonens centrala delar"
	, u8"7. Zonens centrala delar"
	, u8"8. Zonens centrala delar"
	, u8"9 + S�rskilt farliga sektorer"
};
static int hotValue = 0;

// hot i zonen
std::string hotIZonenStr[]{
	u8"Humanoider"
	, u8"Monster"
	, u8"Fenomen"
};
static int hotIZonenValue = -1;

// s�tter up str�ngklasserna
bw::Enviroment miljo;
bw::StandardRuin stdRuin;
bw::IndustriRuin indRuin;
bw::Humanoider humanoider;
bw::Monster monster;
bw::Fenomen fenomen;

int main()
{
	static int HEIGHT = 600;
	static int WIDTH = 800;

	srand((unsigned int)time(NULL));

	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "");
	window.setVerticalSyncEnabled(true);
	ImGui::SFML::Init(window);

	sf::Color bgColor;

	float color[3] = { 0.f, 0.f, 0.f };

	// let's use char array as buffer, see next part
	// for instructions on using std::string with ImGui
	char windowTitle[255] = "Skapa Zonsektorer";

	window.setTitle(windowTitle);
	window.resetGLStates(); // call it if you only draw ImGui. Otherwise not needed.
	sf::Clock deltaClock;


	

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed || quit_program) {
				window.close();
			}
		}

		ImGui::SFML::Update(window, deltaClock.restart());

		// Setting next window to full screen and with a static size and position
		ImGui::SetNextWindowSize(ImVec2(WIDTH, HEIGHT));
		ImGui::SetNextWindowSizeConstraints(ImVec2(WIDTH, HEIGHT), ImVec2(WIDTH, HEIGHT));
		ImGui::SetNextWindowPos(ImVec2(0, 0));

		ImGui::Begin(""); // begin window


	/*	ImGui::ShowTestWindow();
		skapa_sektor = false;*/

		if (skapa_sektor)
			inledning();
		else if (sektor_miljo)
			sektorMiljo();
		else if (sektor_ruin)
			sektorRuin();
		else if (hot && sektor_rota)
			rot();
		else if (hot && sektor_hot)
			hotMeny();
		else if (hot && sektor_random_hot)
			randomHot();
		else if (randHot > 0 && sektor_hot_i_zonen)
			hotIZonen();
		else if (hot && randHot > 0 && is_humanoider)
			hotHumanoid();
		else if (hot && randHot > 0 && is_monster)
			hotMOnster();
		else if (hot && randHot > 0 && is_fenomen)
			hotFenomen();
		else if (sammanstallning)
			sammanstall();


		ImGui::End(); // end window




	/*	static bool selected[4] = { false, true, false, false };
		ImGui::Selectable("1. I am selectable", &selected[0]);
		ImGui::Selectable("2. I am selectable", &selected[1]);
		ImGui::Text("3. I am not selectable");
		ImGui::Selectable("4. I am selectable", &selected[2]);*/





		
		//ImGui::ShowTestWindow();



		window.clear(bgColor); // fill background with color
		ImGui::Render();
		window.display();
	}

	ImGui::SFML::Shutdown();
}

int diceRoll(int dice)
{
	if(dice == 1)
	{
		int temp = rand() % 6 + 1;
		return temp;
	}
	else if (dice == 2)
	{
		int temp = rand() % 6 + 1;
		temp *= 10;
		temp += rand() % 6 + 1;
		return temp;
	}
	else if (dice == 3)
	{
		int temp = rand() % 6 + 1;
		temp *= 10;
		temp += rand() % 6 + 1;
		temp *= 10;
		temp += rand() % 6 + 1;
		return temp;
	}
	else return -1;
}
///////////////////////////////////	 INLEDNING		 /////////////////////////////
void inledning()
{
	ImGui::TextWrapped(u8"SKAPA SEKTOR"
		"\nResten av kapitlet beskriver hur du steg f�r steg skapar en sektor i Zonen."
		"Du kan v�lja fritt ur tabellerna eller rulla T66."
		" F�lj dessa steg n�r RP g�r in i en ny sektor :"
		"\n1. Best�m allm�n sektormilj�."
		"\n2. Best�m en dominerande ruin i sektorn."
		"\n3. Avg�r r�tniv�n."
		"\n4. Avg�r hotniv� och sl� hotslaget."
		"\n5. Om det finns ett hot i sektorn, avg�r vilket."
		"\n6. Om det finns en artefakt i sektorn, avg�r vilken."
		"\n7. Om det finns en zonstrykare i gruppen, l�t honom sl� f�r att leda v�gen (se kapitel 3 i Regelboken)."
		"\n8. Om du vill, sl� eller v�lj en detalj att anv�nda i din beskrivning.");

	if (ImGui::Button(u8"Slumpa alla!", ButtonSize))
	{
		slumpa_alla = true;
		skapa_sektor = false;
		sektor_miljo = true;

	}; ImGui::SameLine();
	if (ImGui::Button(u8"Forts�tt", ButtonSize))
	{
		skapa_sektor = false;
		sektor_miljo = true;
	}
	// QUIT!!!
	if (ImGui::Button(u8"Avsluta", ButtonSize))
	{
		quit_program = true;
	}
}
///////////////////////////////////	 MILJ�	/////////////////////////////
void sektorMiljo()
{
	ImGui::TextWrapped(u8"SEKTORMILJ�\n"
		"B�rja med att etablera den allm�nna milj�n\n"
		"i sektorn.Sl� T66 eller v�lj : ");

	int size = ARRAY_SIZE(miljo.text);
	for (int i = 0; i < size; i++)
		ImGui::RadioButton(&miljo.text[i][0], &miljo.value, i);

	ImGui::TextWrapped(u8"\nV�lj en av ovan eller slumpa med hj�lp av knappen."
		"\nTryck p� forts�tt f�r att g� vidare");
	// SLUMPA KNAPP
	


	if (ImGui::Button(u8"Slumpa v�rde", ButtonSize) || slumpa_alla)
	{
		int temp = diceRoll(2);
		std::cout << temp << std::endl;
		for (int i = 0; i < size; i++)
		{
			if (miljo.range[0][i] <= temp && miljo.range[1][i] >= temp)
				miljo.value = i;
		}

	}; ImGui::SameLine();

	// kollar om det skall finnas hot, ruiner och artefakter
	if (miljo.value == 12)
	{
		is_bosattning = true;
		ruin_industri = false;
		ruin_standard = false;
		hot = false;
		artefakt = false;
		sammanstallning = true;
		return;

	}
	if (miljo.value >= 0 && miljo.value <= 6 || miljo.value == 12)
	{
		ruin_industri = false;
		ruin_standard = false;
	}
	else if (miljo.value <= 11 && miljo.value != 12)
	{
		if (miljo.value >= 7 && miljo.value <= 10)
		{
			ruin_industri = false;
			ruin_standard = true;
		}
		else if (miljo.value == 11)
		{
			ruin_industri = true;
			ruin_standard = false;
		}
	}
	if (miljo.value < 12)
		hot = true;
	else
		hot = false;
	if (miljo.value <= 6 || miljo.value == 12)
		artefakt = false;
	else
		artefakt = true;

	// N�STA KNAPP
	if (ImGui::Button(u8"Forts�tt", ButtonSize) || slumpa_alla)
	{
		sektor_miljo = false;
		if (ruin_industri || ruin_standard)
		{
			sektor_ruin = true;
			//sektorRuin();
		}
		else
		{
			sektor_rota = true;
			//rot();
		}

	}
	ImGui::Text(u8"Vald milj� �r: ");
	ImGui::SameLine(); ImGui::Text(&miljo.text[miljo.value][0]);
	// g� tillbaka till b�rjan
	if (ImGui::Button(u8"G� tillbaka", ButtonSize))
	{
		startOver();
	}
	// QUIT!!!
	if (ImGui::Button(u8"Avsluta", ButtonSize))
	{
		quit_program = true;
	}

	
	

}
///////////////////////////////////	 RUIN	 /////////////////////////////
void sektorRuin()
{
	ImGui::TextWrapped(u8"RUINER\n"
		"Om sektormilj�n �r delvis raserade ruiner, v�lbevarade ruiner eller industrilandskap "
		"�r n�sta steg att avg�ra vilken ruin som dominerar sektorn.Det finns "
		"f�rst�s mer �n en ruin i varje sektor, men	den utvalda ruinen �r den som v�cker "
		"RP : s intresse.");
	if (ruin_standard)
	{
		int size = ARRAY_SIZE(stdRuin.text);

		for (int i = 0; i < size; i++)
			ImGui::RadioButton(&stdRuin.text[i][0], &stdRuin.value, i);

		ImGui::TextWrapped(u8"\nV�lj en av ovan eller slumpa med hj�lp av knappen."
			"\nTryck p� forts�tt f�r att g� vidare");
		// SLUMPA KNAPP
		if (ImGui::Button(u8"Slumpa v�rde", ButtonSize) || slumpa_alla)
		{
			stdRuin.value = rand() % size;
		}; ImGui::SameLine();
		// N�STA KNAPP
		if (ImGui::Button(u8"Forts�tt", ButtonSize) || slumpa_alla)
		{
			sektor_ruin = false;
			sektor_rota = true;
			//rot();
		}
		ImGui::Text(u8"Vald ruin �r: ");
		ImGui::SameLine(); ImGui::Text(&stdRuin.text[stdRuin.value][0]);
		// g� tillbaka till b�rjan
		if (ImGui::Button(u8"G� tillbaka", ButtonSize))
		{
			startOver();
		}
		// QUIT!!!
		if (ImGui::Button(u8"Avsluta", ButtonSize))
		{
			quit_program = true;
		}
	}
	else if (ruin_industri)
	{
		int size = ARRAY_SIZE(indRuin.text);

		for (int i = 0; i < size; i++)
			ImGui::RadioButton(&indRuin.text[i][0], &indRuin.value, i);
		// SLUMPA KNAPP
		if (ImGui::Button(u8"Slumpa v�rde", ButtonSize) || slumpa_alla)
		{
			int temp = diceRoll(2);
			std::cout << temp << std::endl;
			for (int i = 0; i < size; i++)
			{
				if (indRuin.range[0][i] <= temp && indRuin.range[1][i] >= temp)
					indRuin.value = i;
			}

		}; ImGui::SameLine();
		// N�STA KNAPP
		if (ImGui::Button(u8"Forts�tt", ButtonSize) || slumpa_alla)
		{
			sektor_ruin = false;
			sektor_rota = true;
			//rot();
		}
		ImGui::Text(u8"Vald ruin �r: ");
		ImGui::SameLine(); ImGui::Text(&indRuin.text[indRuin.value][0]);
		// g� tillbaka till b�rjan
		if (ImGui::Button(u8"G� tillbaka", ButtonSize))
		{
			startOver();
		}
		// QUIT!!!
		if (ImGui::Button(u8"Avsluta", ButtonSize))
		{
			quit_program = true;
		}
	}
	
}
///////////////////////////////////	 R�TA	 /////////////////////////////
void rot()
{
	ImGui::TextWrapped(u8"R�TNIV�\n"
		"V�lj eller sl� fram r�tniv�n i sektorn, en siffra fr�n 0 till 3. 1 eller 2 �r det vanliga "
		"i Zonen.L�s mer om zonr�ta i kapitel 8 i Regelboken. "
		"Om du har en dold sl - karta kan du rita in p� f�rhand vilken r�tniv� Zonens sektorer har.");

	int size = ARRAY_SIZE(rotStr);

	for (int i = 0; i < size; i++)
		ImGui::RadioButton(&rotStr[i][0], &rotValue, i);

	// SLUMPA KNAPP
	if (ImGui::Button(u8"Slumpa v�rde", ButtonSize) || slumpa_alla)
	{
			int randomValue = rand() % 6 + 1;
			if (randomValue == 1)
				rotValue = 0;
			else if (randomValue == 6)
				rotValue = 2;
			else
				rotValue = 1;
	}; ImGui::SameLine();
	// N�STA KNAPP
	if (ImGui::Button(u8"Forts�tt", ButtonSize) || slumpa_alla)
	{
		sektor_rota = false;
		sektor_hot = true;
		//hotMeny();
	}

	ImGui::Text(u8"Vald r�ta �r: ");
	ImGui::SameLine(); ImGui::Text(&rotStr[rotValue][0]);
	// g� tillbaka till b�rjan
	if (ImGui::Button(u8"G� tillbaka", ButtonSize))
	{
		startOver();
	}
	// QUIT!!!
	if (ImGui::Button(u8"Avsluta", ButtonSize))
	{
		quit_program = true;
	}
}
///////////////////////////////////	 HOT		 /////////////////////////////
void hotMeny()
{
	ImGui::TextWrapped(u8"N�sta steg �r att best�mma hotniv�n.\n"
		"Hotniv�n �r ett v�rde fr�n noll och upp�t. "
		"Ett h�gt v�rde betyder en farligare sektor, men ocks� st�rre chans att "
		"det finns or�rda artefakter d�r.\n"
		"Om sektormilj�n (ovan) �r bos�ttning har sektorn "
		"ingen hotniv�.G� direkt till steg 5 "
		"om bos�ttningar, nedan.");

	int size = ARRAY_SIZE(hotNiva);
	for (int i = 0; i < size; i++)
		ImGui::RadioButton(&hotNiva[i][0], &hotValue, i);

	// SLUMPA KNAPP
	if (ImGui::Button(u8"Slumpa v�rde", ButtonSize) || slumpa_alla)
	{
		hotValue = rand() % size;

	}; ImGui::SameLine();
	// N�STA KNAPP
	if (ImGui::Button(u8"Forts�tt", ButtonSize) || slumpa_alla)
	{
		sektor_hot = false;
		sektor_random_hot = true;
		//randomHot();
	}
	ImGui::Text(u8"Vald hotniv� �r: ");
	ImGui::SameLine(); ImGui::Text(&hotNiva[hotValue][0]);
	// g� tillbaka till b�rjan
	if (ImGui::Button(u8"G� tillbaka", ButtonSize))
	{
		startOver();
	}
	// QUIT!!!
	if (ImGui::Button(u8"Avsluta", ButtonSize))
	{
		quit_program = true;
	}
}
///////////////////////////////////	 SLUMPA HOT 	 /////////////////////////////
void randomHot()
{
	ImGui::TextWrapped(u8"N�r rollpersonerna g�r in i sektorn rullar du ett antal grundt�rningar lika "
		"med hotniv�n.Varje RADIAK betyder att det finns en artefakt i sektorn.Sl�r du minst "
		"en BIOHAZARD betyder det att det finns ett hot i sektorn, och ju fler BIOHAZARD du sl�r, desto "
		"v�rre �r hotet.Se steg 5 nedan.\n"
		"Om sektormilj�n �r skog eller totalt raserade ruiner(ovan) finns det inga artefakter i sektorn, oavsett vad t�rningarna visar. "
		"RADIAK har allts� ingen betydelse.\n"
		"Nattetid �r det farligt att r�ra sig i Zonen.R�kna hotniv�n som tre steg h�gre �n normalt.");

	static int e = 0;

	ImGui::RadioButton("Dagtid", &e, 0);
	ImGui::RadioButton("Nattetid", &e, 1);

	int tempHot = hotValue + (e * 3) + 1;
	

	// SLUMPA KNAPP
	if (ImGui::Button(u8"Slumpa v�rde", ButtonSize) || slumpa_alla)
	{
		randArtefakt = 0;
		randHot = 0;
		for (int i = 0; i < tempHot; i++)
		{
			int tempRand = rand() % 6 + 1;
			if (tempRand == 1)
				randHot++;
			else if (tempRand == 6)
				randArtefakt++;
			std::cout << tempRand << std::endl;

		}
		

	}; ImGui::SameLine();
	// N�STA KNAPP
	if (ImGui::Button(u8"Forts�tt", ButtonSize) || slumpa_alla)
	{
		if(randHot > 0)
		{
			hotValue = tempHot;
			sektor_random_hot = false;
			sektor_hot_i_zonen = true;

		}
		else
		{
			hotValue = tempHot;
			sektor_random_hot = false;
			sektor_hot_i_zonen = false;
			sammanstallning = true;
		}

	}
	ImGui::Text(u8"Hotniv�n �r: %d", tempHot);
	if (!hot)
		ImGui::Text(u8"Inga hot p�tr�ffas p� grund av zonvalen");
	else
		ImGui::Text(u8"Styrka p� hotet : %d", randHot);
	if (!artefakt)
		ImGui::Text(u8"Inga artefakter p�tr�ffas p� grund av zonvalen");
	else
		ImGui::Text(u8"Antal artefakter �r: %d", randArtefakt);

	// g� tillbaka till b�rjan
	if (ImGui::Button(u8"G� tillbaka", ButtonSize))
	{
		startOver();
	}
	// QUIT!!!
	if (ImGui::Button(u8"Avsluta", ButtonSize))
	{
		quit_program = true;
	}
}
///////////////////////////////////	 HOT I ZONEN    	 /////////////////////////////
void hotIZonen()
{
	if (hotIZonenValue == -1)
		hotIZonenValue = 0;
	ImGui::TextWrapped(u8"Hot i Zonen tar sig tre olika former : "
		"Humanoider, monster och fenomen.L�s mer om dem i kapitel 5. Sl� en T6 eller v�lj : ");

	for (int i = 0; i < ARRAY_SIZE(hotIZonenStr); i++)
		ImGui::RadioButton(&hotIZonenStr[i][0], &hotIZonenValue, i);

	// SLUMPA KNAPP
	if (ImGui::Button(u8"Slumpa v�rde", ButtonSize) || slumpa_alla)
	{
		int temp = rand() % 6 + 1;
		if (temp == 1 || temp == 2)
			hotIZonenValue = 0;
		else if (temp == 6)
			hotIZonenValue = 2;
		else
			hotIZonenValue = 1;

	}; ImGui::SameLine();
	// R�kna ut vilken typ av hot det g�ller
	if (hotIZonenValue == 0)
	{
		is_humanoider = true;
		is_monster = false;
		is_fenomen = false;
	}
	else if (hotIZonenValue == 1)
	{
		is_humanoider = false;
		is_monster = true;
		is_fenomen = false;
	}
	else if (hotIZonenValue == 2)
	{
		is_humanoider = false;
		is_monster = false;
		is_fenomen = true;
	}
	// N�STA KNAPP
	if (ImGui::Button(u8"Forts�tt", ButtonSize) || slumpa_alla)
	{

		sektor_hot_i_zonen = false;
		//hotIZonen();

	}
	// g� tillbaka till b�rjan
	if (ImGui::Button(u8"G� tillbaka", ButtonSize))
	{
		startOver();
	}
	// QUIT!!!
	if (ImGui::Button(u8"Avsluta", ButtonSize))
	{
		quit_program = true;
	}
	
}
///////////////////////////////////	HUMANOIDA HOT    	 /////////////////////////////
void hotHumanoid()
{
	ImGui::TextWrapped(u8"humanoider �r individer eller grupper av t�nkande varelser.Sl� T66 eller v�lj : ");

	int size = ARRAY_SIZE(humanoider.text);
	for (int i = 0; i < size; i++)
		ImGui::RadioButton(&humanoider.text[i][0], &humanoider.value, i);

	// SLUMPA KNAPP
	if (ImGui::Button(u8"Slumpa v�rde", ButtonSize) || slumpa_alla)
	{
		int temp = diceRoll(2);
		std::cout << temp << std::endl;
		for (int i = 0; i < size; i++)
		{
			if (humanoider.range[0][i] <= temp && humanoider.range[1][i] >= temp)
				humanoider.value = i;
		}

	}; ImGui::SameLine();

	// N�STA KNAPP
	if (ImGui::Button(u8"Forts�tt", ButtonSize) || slumpa_alla)
	{
		hot = false;
		sammanstallning = true;
	}
	ImGui::Text(u8"Vald humanoid �r: ");
	ImGui::SameLine(); ImGui::Text(&humanoider.text[humanoider.value][0]);
	// g� tillbaka till b�rjan
	if (ImGui::Button(u8"G� tillbaka", ButtonSize))
	{
		startOver();
	}
	// QUIT!!!
	if (ImGui::Button(u8"Avsluta", ButtonSize))
	{
		quit_program = true;
	}
}
void hotMOnster()
{
	ImGui::TextWrapped(u8"MONSTER �r sinnessl�a bestar som h�rjar i Zonens vidder.Vissa �r muterade "
		"versioner av nutidens djur, andra n�got helt fr�mmande.Sl� T66 eller v�lj : ");

	int size = ARRAY_SIZE(monster.text);
	for (int i = 0; i < size; i++)
		ImGui::RadioButton(&monster.text[i][0], &monster.value, i);


	// SLUMPA KNAPP
	if (ImGui::Button(u8"Slumpa v�rde", ButtonSize) || slumpa_alla)
	{
		int temp = diceRoll(2);
		std::cout << temp << std::endl;
		for (int i = 0; i < size; i++)
		{
			if (monster.range[0][i] <= temp && monster.range[1][i] >= temp)
				monster.value = i;
		}
		

	}; ImGui::SameLine();
	// N�STA KNAPP
	if (ImGui::Button(u8"Forts�tt", ButtonSize) || slumpa_alla)
	{
		hot = false;
		sammanstallning = true;
	}
	ImGui::Text(u8"Vald humanoid �r: ");
	ImGui::SameLine(); ImGui::Text(&monster.text[monster.value][0]);

	// g� tillbaka till b�rjan
	if (ImGui::Button(u8"G� tillbaka", ButtonSize))
	{
		startOver();
	}
	// QUIT!!!
	if (ImGui::Button(u8"Avsluta", ButtonSize))
	{
		quit_program = true;
	}
}
void hotFenomen()
{
	ImGui::TextWrapped(u8"fenomen �r annat �n levande varelser. Sl� T66 eller v�lj : ");

	int size = ARRAY_SIZE(fenomen.text);
	for (int i = 0; i < size; i++)
		ImGui::RadioButton(&fenomen.text[i][0], &fenomen.value, i);

	// SLUMPA KNAPP
	if (ImGui::Button(u8"Slumpa v�rde", ButtonSize) || slumpa_alla)
	{
		int temp = diceRoll(2);
		std::cout << temp << std::endl;
		for (int i = 0; i < size; i++)
		{
			if (fenomen.range[0][i] <= temp && fenomen.range[1][i] >= temp)
				fenomen.value = i;
		}

	}; ImGui::SameLine();
	// N�STA KNAPP
	if (ImGui::Button(u8"Forts�tt", ButtonSize) || slumpa_alla)
	{
		hot = false;
		sammanstallning = true;
	}
	ImGui::Text(u8"Vald humanoid �r: ");
	ImGui::SameLine(); ImGui::Text(&fenomen.text[fenomen.value][0]);

	// g� tillbaka till b�rjan
	if (ImGui::Button(u8"G� tillbaka", ButtonSize))
	{
		startOver();
	}
	// QUIT!!!
	if (ImGui::Button(u8"Avsluta", ButtonSize))
	{
		quit_program = true;
	}
}

void sammanstall()
{
	ImVec4 YELLOW(255,255,0,255);
	ImVec4 RED(255, 0, 0, 255);
	ImVec4 GREEN(0, 255, 0, 255);
	ImVec4 BLUE(0, 0, 255, 255);
	ImVec4 PURPLE(255, 0, 255, 255);
	ImVec4 CYAN(0, 255, 255, 255);
	slumpa_alla = false;
	ImGui::Text(u8"Detta �r en sammanst�llning av alla valen : ");
	ImGui::Text("");
	if (randHot == 0)
	{
		ImGui::TextColored(YELLOW, u8"Hotniv�n �r : %d", hotValue);
		ImGui::SameLine(); ImGui::TextColored(YELLOW, u8", men inga hot p�tr�ffades!");
	}
	if(miljo.value != 12)
	{
		ImGui::TextColored(YELLOW, u8"R�TNIV� : %d", rotValue);
		ImGui::SameLine(); ImGui::TextColored(YELLOW, &rotStr[rotValue][0]);
	}
	
	
	ImGui::Text("");
	ImGui::TextColored(GREEN, u8"MILJ� : ");
	ImGui::SameLine(); ImGui::TextColored(GREEN, &miljo.text[miljo.value][0]);
	


	if (ruin_standard)
	{
		ImGui::TextColored(GREEN, u8"Den dominerande ruinen �r : ");
		ImGui::SameLine(); ImGui::TextColored(GREEN, &stdRuin.text[stdRuin.value][0]);
		if (!artefakt)
			ImGui::TextColored(GREEN, u8"Inga artefakter hittade p� grund av zonvalen");
		else
			ImGui::TextColored(GREEN, u8"Antal artefakter �r: %d", randArtefakt);
		stdRuinBeskrivning(stdRuin.value);
	}
	else if (ruin_industri)
	{
		ImGui::TextColored(GREEN, u8"Den dominerande ruinen �r : ");
		ImGui::SameLine(); ImGui::TextColored(GREEN, &indRuin.text[indRuin.value][0]);
		if (!artefakt)
			ImGui::TextColored(GREEN, u8"Inga artefakter hittade p� grund av zonvalen");
		else
			ImGui::TextColored(GREEN, u8"Antal artefakter �r: %d", randArtefakt);
		indRuinBeskrivning(indRuin.value);
	}
	if (is_humanoider && randHot > 0)
	{
		ImGui::TextColored(PURPLE, u8"Hotet i zonen �r humanoider : ");
		ImGui::SameLine(); ImGui::TextColored(PURPLE, &humanoider.text[humanoider.value][0]);
		ImGui::TextColored(PURPLE, u8"Styrka p� hotet : %d", randHot);
		humanoidBeskrivning(humanoider.value);///////////////////////////////////////
	}
	else if (is_monster && randHot > 0)
	{
		ImGui::TextColored(PURPLE, u8"Hotet i zonen �r monster : ");
		ImGui::SameLine(); ImGui::TextColored(PURPLE, &monster.text[monster.value][0]);
		ImGui::TextColored(PURPLE, u8"Styrka p� hotet : %d", randHot);
		monsterBeskrivning(monster.value);///////////////////////////////////////
	}
	else if (is_fenomen && randHot > 0)
	{
		ImGui::TextColored(PURPLE, u8"Hotet i zonen �r fenomen : ");
		ImGui::SameLine(); ImGui::TextColored(PURPLE, &fenomen.text[fenomen.value][0]);
		ImGui::TextColored(PURPLE, u8"Styrka p� hotet : %d", randHot);
		fenomenBeskrivning(fenomen.value);
	}

	ImGui::Text("");

	if (ImGui::Button(u8"G� tillbaka", ButtonSize))
	{
		startOver();
	}
	// QUIT!!!
	if (ImGui::Button(u8"Avsluta", ButtonSize))
	{
		quit_program = true;
	}

}

void startOver()
{
	skapa_sektor = true;
	sektor_miljo = false;
	sektor_ruin = false;
	sektor_rota = false;
	sektor_hot = false;
	sektor_random_hot = false;
	sektor_hot_i_zonen = false;
	is_humanoider = false;
	is_monster = false;
	is_fenomen = false;
	is_bosattning = false;
	sammanstallning = false;
	slumpa_alla = false;

	// Om det skall finnas hot, ruiner och artefakter i zonen
	ruin_standard = false;
	ruin_industri = false;
	hot = false;
	artefakt = false;
	// Hur m�nga hot och artefakter
	randHot = 0;
	randArtefakt = 0;
	hotValue = 0;
	rotValue = 0;
	hotIZonenValue = -1;


	miljo.value = 0;
	stdRuin.value = 0;
	indRuin.value = 0;
	humanoider.value = 0;
	monster.value = 0;
	fenomen.value = 0;
}

void stdRuinBeskrivning(int i)
{
	switch (i+1)
	{
	case 1:
		ImGui::TextWrapped(u8"AFF�RSGALLERIA\n"
			"En stad i staden �ppnar sig, en enorm sal\n"
			"med forntida aff�rer och restauranger i\n"
			"flera v�ningar l�ngs v�ggarna. Allt �r\n"
			"illa h�rjat av l�gorna och t�ckt av tjocka\n"
			"lager sot, men kanske g�mmer sig n�got\n"
			"v�rdefullt under askan ? H�r och var st�r\n"
			"svarta figurer i f�rbr�nd plast i m�rkliga\n"
			"st�llningar, som om de talade om den\n"
			"gamla v�rldens viktiga kunskaper.");
		break;
	case 2:
		ImGui::TextWrapped(u8"BADHUS\n"
			"Inne i denna stora ruin finns stora bass�nger\n"
			"fyllda av gyttja och slemmiga\n"
			"gr�na alger. Motbjudande och osunda\n"
			"�ngor m�ter bes�kare som f�rs�ker ta\n"
			"sig in. Det �r sv�rt att t�nka sig att fornfolket\n"
			"frivilligt gick hit och doppade sig\n"
			"i bass�ngerna.");
		break;
	case 3:
		ImGui::TextWrapped(u8"BENSINSTATION\n"
			"Under ett �ppet tak vid en l�g byggnad\n"
			"st�r n�gra motork�rror och rostar. Under\n"
			"taket finns maskiner med slangar och\n"
			"kranar. Den zonkloke vet att fornfolket\n"
			"reparerade sina fordon och fyllde dem\n"
			"med br�nsle p� platser som denna.");
		break;
	case 4:
		ImGui::TextWrapped(u8"BIOGRAF\n"
			"Intill en stor entr� i en husfasad h�nger\n"
			"en m�ngd gulnade planscher i ramar,\n"
			"alla visar dramatiska motiv fr�n tiden\n"
			"f�re underg�ngen. H�r inne tittade den\n"
			"gamla v�rldens m�nniskor p� r�rliga bilder\n"
			"p� en duk.");
		break;
	case 5:
		ImGui::TextWrapped(u8"VILLAOMR�DE\n"
			"En smal gata med sm�, identiska hus i\n"
			"j�mna rader p� varje sida.Tr�dg�rdarnas\n"
			"gr�s och buskar har vittrat bort, allt\n"
			"�r t�ckt av damm och aska. Inne i husen\n"
			"ligger multnande kl�der och prylar kvar,\n"
			"nv�narna verkar ha flytt i en hast. Fordonsvrak\n"
			"och m�ngder av spruckna och\n"
			"f�rst�rda l�mningar fr�n dem som en\n"
			"�ng bott i omr�det finns �verallt. F�rfallet\n"
			"r markant och m�nga av husen har\n"
			"aft sv�rt att motst� tidens tand.");
		break;
	case 6:
		ImGui::TextWrapped(u8"BUSSTATION\n"
			"Bland flagnade skyltar med ol�sliga siffror\n"
			"och bokst�ver p� st�r flera avl�nga\n"
			"fornk�rror och rostar. Vissa �r r�da, \n"
			"andra �r bl�, och flera har stora dragspel\n"
			"p� mitten. �nnu ett av den gamla v�rldens\n"
			"mysterier.");
		break;
	case 7:
		ImGui::TextWrapped(u8"FLYGPLANSVRAK\n"
			"En enorm metalltub ligger kn�ckt och\n"
			"s�nderbruten p� marken. Den �r svedd\n"
			"av eld och inuti syns m�ngder av forntida\n"
			"f�t�ljer, svartbr�nda av l�gorna. I\n"
			"ett avl�ngt f�lt som str�cker sig bort\n"
			"fr�n tuben �r marken sargad och alla\n"
			"hus �delagda, som om tuben st�rtat fr�n\n"
			"himlen i fruktansv�rd hastighet.");
		break;
	case 8:
		ImGui::TextWrapped(u8"F�RVILDAD PARK\n"
			"Spretiga tr�d och buskar v�xer vilt och\n"
			"ohejdat h�r. B�nkar, g�ngv�gar och\n"
			"font�ner g�ms under tjockt sly och\n"
			"gr�smattorna har f�rvandlats till ett\n"
			"vattensjukt tr�sk. Det g�r m�nga rykten\n"
			"om farliga varelser som d�ljer sig i\n"
			"Zonens forna parker.");
		break;
	case 9:
		ImGui::TextWrapped(u8"H�GHUSRUIN\n"
			"Ett h�gt torn reser sig mot Zonens guldisiga\n"
			"himmel. Betongen �r sargad och\n"
			"sprucken, och alla f�nster har f�r l�nge\n"
			"sedan fallit ur sina ramar. Zonvinden viner\n"
			"genom v�ningsplanen, som gapar �ppna\n"
			"och m�rka. Varje v�ning �r fylld av sm�\n"
			"och likformade bostadsenheter eller kontor.\n"
			"Bara de �versta v�ningarna skiljer sig\n"
			"med markant st�rre och lyxigare bost�der.");
		break;
	case 10:
		ImGui::TextWrapped(u8"JAKTAFF�R\n"
			"Bland v�lta hyllor g�r det att hitta b�de\n"
			"fiskesp�n, krokar och linor. En zonstrykare\n"
			"kan ta reda p� i vilka av Zonens vatten\n"
			"man v�gar fiska. P� v�ggarna sitter\n"
			"halvt s�nderrivna bilder p� gev�r med\n"
			"kikarsikten. I ett h�rn st�r ett tungt l�st\n"
			"metallsk�p h�gt som en mutant, som\n"
			"kan inneh�lla n�got v�rdefullt.");
		break;
	case 11:
		ImGui::TextWrapped(u8"KONTORSKOMPLEX\n"
			"I denna m�rka ruin finns sal efter sal\n"
			"med or�kneliga m�ngder identiska\n"
			"bord i varsitt litet b�s. P� golvet ligger\n"
			"gulnade papper utspridda. F�re underg�ngen\n"
			"p�gick s�kert n�got viktigt\n"
			"arbete h�r. Nu virvlar zondammet in\n"
			"genom de krossade f�nstren.");
		break;
	case 12:
		ImGui::TextWrapped(u8"KRATER\n"
			"Ett gigantiskt h�l gapar i zonmarken, \n"
			"minst 50 meter tv�rs �ver. H�r m�ste\n"
			"n�got fornvapen av horribel styrka slagit\n"
			"ned och f�rintat allt i n�rheten. I botten\n"
			"av kratern har en liten sj� bildats av\n"
			"gr�naktigt zonsjukt vatten.");
		break;
	case 13:
		ImGui::TextWrapped(u8"KR�KSLOTT\n"
			"Detta m�rka och fallf�rdiga hus var\n"
			"gammalt redan f�re underg�ngen. Det\n"
			"�r byggt i tr� och sten och har p� n�got\n"
			"s�tt �verlevt katastrofens h�rjningar.");
		break;
	case 14:
		ImGui::TextWrapped(u8"KYRKA\n"
			"Det ur�ldriga stentornet p� denna byggnad\n"
			"n�r h�gt i skyn, dess vassa spets\n"
			"g�r f�rlorad i Zonens dis. Inne i byggnaden\n"
			"st�r rader av murkna tr�b�nkar\n"
			"v�nda mot en mansfigur som h�nger p�\n"
			"ett kors. Hit gick fornfolket f�r att prata\n"
			"med sin h�vding i himlen. Nu �r det tyst.\n"
			"Om det funnits en gud har den f�r l�nge\n"
			"sedan �vergivit denna plats.");
		break;
	case 15:
		ImGui::TextWrapped(u8"LEKPLATS\n"
			"Bild�ck h�nger i rostiga k�ttingar fr�n\n"
			"en st�llning.En tr�l�da med sand i har\n"
			"blivit en osund lerp�l. Syraregn har f�r\n"
			"l�nge sedan fr�tt s�nder rutschkanan.\n"
			"H�r lekte den gamla v�rldens m�nniskor\n"
			"med sina barn. N�gra spruckna plastleksaker\n"
			"har �verlevt tidens tand och\n"
			"sticker upp ur gruset.");
		break;
	case 16:
		ImGui::TextWrapped(u8"MILJONPROGRAM\n"
			"Flera likformade byggnader i gr� och\n"
			"skadad betong. Mellan husen finns\n"
			"mansh�ga st�llningar av f�rvridet skrot, \n"
			"halvt raserade staket och trasiga b�nkar.\n"
			"Otaliga vrak pryder ocks� omr�det.\n"
			"I husen har tydligt n�gon sorts bost�der\n"
			"legat. Varje enhet tycks lik de andra och\n"
			"varierar minimalt. Det tycks enbart vara\n"
			"antalet rum som skiljer dem �t. Rester\n"
			"av forntida m�bler, husger�d och annat\n"
			"finns i de flesta av utrymmena.");
		break;
	case 17:
		ImGui::TextWrapped(u8"MOTORV�G\n"
			"Asfalten �r sprucken och de vita\n"
			"strecken har n�stan flagnat bort. F�re\n"
			"underg�ngen susade den gamla v�rldens\n"
			"folk fram h�r i hisnande hastighet\n"
			"i sina bl�nkande motork�rror. Nu st�r\n"
			"fordonen utbr�nda och s�nderrostade\n"
			"l�ngs v�gkanten.");
		break;
	case 18:
		ImGui::TextWrapped(u8"MUSEUM\n"
			"En m�ktig trappa i sprucken sten leder\n"
			"upp till en pampig byggnad. D�r inne\n"
			"finns m�rka salar med m�rkliga f�rem�l\n"
			"fr�n den gamla v�rlden och �nnu tidigare\n"
			"v�rldar, uppst�llda i montrar f�r\n"
			"fornfolket att besk�da.");
		break;
	case 19:
		ImGui::TextWrapped(u8"N�JESF�LT\n"
			"En bisarr samling maskiner och stora\n"
			"t�lt i f�rger som en g�ng varit bj�rta st�r\n"
			"och vittrar s�nder. Resterna av t�ltduken\n"
			"sl�r i vinden. P� en bild syns en leende\n"
			"man med vitm�lat ansikte och r�d n�sa.\n"
			"En maskin har formen av en stor rund\n"
			"tr�platta med plasth�star p�. Plattan �r\n"
			"monterad p� en motor och verkar ha\n"
			"kunnat rotera.");
		break;
	case 20:
		ImGui::TextWrapped(u8"PARKERINGSHUS\n"
			"Betongpelarna har vikt sig och hela den\n"
			"runda byggnaden har s�ckat ihop som\n"
			"ett sammanpressat dragspel. Men i rasmassorna\n"
			"mellan v�ningsplanen d�ljer\n"
			"sig m�ngder av motork�rror fr�n den\n"
			"gamla v�rlden. Bilar, kallades de f�re\n"
			"underg�ngen.");
		break;
	case 21:
		ImGui::TextWrapped(u8"POLISSTATION\n"
			"Utanf�r den stora byggnaden vilar vrak\n"
			"som en g�ng var identiska fordon. D�cken\n"
			"�r sedan l�nge borta, men takens bl�\n"
			"plastk�por finns kvar. Rummen i byggnadens\n"
			"k�llare g�r inte att �ppna inifr�n.");
		break;
	case 22:
		ImGui::TextWrapped(u8"RADIOSTATION\n"
			"En skog av rostiga antenner och buckliga\n"
			"pl�tfat str�cker sig mot zonhimlen\n"
			"fr�n taket p� denna massiva byggnad.\n"
			"En skylt p� fasaden visar en antenn med\n"
			"ringar omkring.");
		break;
	case 23:
		ImGui::TextWrapped(u8"RASERAD BRO\n"
			"H�gt i Zonens gula dis reser sig resterna\n"
			"av en majest�tisk bro. Den har rasat �t\n"
			"b�de v�ster och �ster och bara tv� tjocka\n"
			"pelare st�r kvar, men den som v�gar\n"
			"kl�ttra upp har hittat en perfekt utkiksplats\n"
			"�ver Zonens ruiner.");
		break;
	case 24:
		ImGui::TextWrapped(u8"SKYDDSRUM\n"
			"En massiv rostig st�lport i bergv�ggen\n"
			"leder ned i djupet. D�r nere s�kte den\n"
			"gamla v�rldens folk skydd mot fasorna i\n"
			"ytv�rlden. Ett perfekt st�lle att leta efter\n"
			"artefakter. Men vem vet vad som g�mmer\n"
			"sig d�r nere, och vem mer som kan\n"
			"ha hittat hit ? ");
		break;
	case 25:
		ImGui::TextWrapped(u8"SLAGF�LT\n"
			"Omr�det �r fullkomligt �delagt. Rostiga\n"
			"armeringsj�rn spretar ur kn�ckta\n"
			"betongblock som en g�ng varit husv�ggar.\n"
			"Ruinerna �r svedda av explosioner\n"
			"och genomborrade av kulh�l, marken\n"
			"sargad av kratrar.");
		break;
	case 26:
		ImGui::TextWrapped(u8"SNABBMATSRESTAURANG\n"
			"Ett stort M i form av tv� smutsgula b�gar\n"
			"pryder fasaden p� denna ruin. Liknande\n"
			"byggnader finns p� massor av st�llen i\n"
			"Zonen, minst en i varje sektor. Vad som\n"
			"fanns d�r f�rr vet ingen l�ngre, men\n"
			"M-husen undviks av de flesta zonstrykare\n"
			"p� grund av den starka r�tan i dem.");
		break;
	case 27:
		ImGui::TextWrapped(u8"SPORTHALL\n"
			"En stor och rymlig byggnad i �rgad\n"
			"korrugerad pl�t. D�r inne finns m�rkliga\n"
			"saker som tjocka madrasser i r�d\n"
			"plast, stora uppsp�nda n�t, klubbor\n"
			"med h�l i och bollar i olika storlekar\n"
			"och f�rger. V�ggarna �r t�ckta av tr�stegar\n"
			"som nu har murknat och fallit\n"
			"loss. Det s�gs att folk f�re katastrofen\n"
			"gick hit f�r att tr�na.");
		break;
	case 28:
		ImGui::TextWrapped(u8"SJUKHUS\n"
			"Till detta betongkomplex s�kte sig den\n"
			"gamla v�rldens folk i horder n�r apokalypsen\n"
			"kom. Nu �r huset en fallf�rdig ruin.\n"
			"�verallt i salar och korridorer, p� s�ngar, \n"
			"britsar och golv, ligger f�rtorkade lik.");
		break;
	case 29:
		ImGui::TextWrapped(u8"STRIDSVAGN\n"
			"En skr�ckinjagande metalltingest blockerar\n"
			"v�gen. Dess enorma larvf�tter har f�r\n"
			"l�nge sedan rostat fast och ingen d�d och\n"
			"f�rintelse har skjutits ur kanonr�ret p�\n"
			"otaliga �r, men att n�rma sig pansarbesten\n"
			"v�cker �nd� oro och smygande fasa.");
		break;
	case 30:
		ImGui::TextWrapped(u8"TEATER\n"
			"Inne i denna m�rka ruin finns en stor sal, \n"
			"fylld med l�nga rader av f�t�ljer framf�r\n"
			"en stor scen med en m�ktig r�d rid�. Hit\n"
			"gick den gamla v�rldens folk f�r att se\n"
			"s�ng, dans och sk�despel.");
		break;
	case 31:
		ImGui::TextWrapped(u8"TUNNELBANA\n"
			"P� en sargad husfasad h�nger en\n"
			"flagnad skylt med ett bl�tt T i en vit\n"
			"cirkel. Alla zonstrykare vet vad symbolen\n"
			"betyder. H�r finns en nedg�ng\n"
			"till tunnlarna som l�per under Zonen.\n"
			"I djupet ruvar stora faror � men kanske\n"
			"ocks� or�rda artefakter.");
		break;
	case 32:
		ImGui::TextWrapped(u8"T�GSTATION\n"
			"P� rostiga och �vervuxna sp�r st�r flera\n"
			"forntida r�lsvagnar och vittrar sakta s�nder.\n"
			"I dessa tr�ngde den gamla v�rldens\n"
			"folk in sig f�r att snabbt och effektivt\n"
			"resa tillsammans kors och tv�rs i den\n"
			"stora staden. Nu sveper vinden genom\n"
			"vagnarnas krossade f�nster.");
		break;
	case 33:
		ImGui::TextWrapped(u8"SNABBK�P\n"
			"En storslagen byggnad reser sig bland\n"
			"ruinerna.Dess stora skyltf�nster �r krossade\n"
			"f�r l�nge sedan och zondammet\n"
			"driver in obehindrat. F�re underg�ngen\n"
			"rymde byggnaden en o�ndlig m�ngd\n"
			"kl�der, skor, b�cker, k�ksartiklar, m�bler\n"
			"och andra prylar som folk konsumerade\n"
			"i den gamla v�rlden. Nu �r det mesta\n"
			"br�nt och s�ndervittrat.");
		break;
	case 34:
		ImGui::TextWrapped(u8"V�GTUNNEL\n"
			"Mynningen till en bred tunnel gapar\n"
			"svart och hotfull. Utanf�r tunneln tr�ngs\n"
			"en or�knelig m�ngd motork�rror, s�nderrostade\n"
			"och t�tt sammanpressade.\n"
			"Vilken fasa fordonens passagerare �n\n"
			"flytt ifr�n �r det uppenbart att f� lyckades\n"
			"komma undan.");
		break;
	case 35:
		ImGui::TextWrapped(u8"�VERGIVEN SKOLA\n"
			"P� det fallf�rdiga tegelhusets fasad\n"
			"h�nger en vit klocka som stannat f�r\n"
			"l�nge sedan. L�ngre ned p� v�ggen har\n"
			"n�gon m�lat text, vad det st�r g�r inte\n"
			"l�ngre att l�sa. P� asfaltplanen framf�r\n"
			"byggnaden st�r tv� rostiga metallstavar\n"
			"nedk�rda i marken, drygt tv� meter h�ga\n"
			"och med varsin ring h�gst upp.");
		break;
	case 36:
		ImGui::TextWrapped(u8"�DELAGD MARINA\n"
			"Vid strandkanten ligger otaliga b�tvrak\n"
			"fr�n den gamla tiden, halvt nedsjunkna i\n"
			"det osunda vattnet. Segelb�tar, motorb�tar\n"
			"och kanoter som folk roade sig med i\n"
			"den gamla v�rlden har f�rvandlats till ett\n"
			"rev av sprucken plast.");
		break;
	}
}

void indRuinBeskrivning(int i)
{
	switch (i+1)
	{
	case 1:
		ImGui::TextWrapped(u8"FABRIK\n"
			"Pl�ttaket har rasat in p� denna stora\n"
			"barackliknande byggnad. Under taket\n"
			"finns maskiner och m�rkliga fordon med\n"
			"stora gafflar l�ngst fram. Det mesta �r\n"
			"sargat av v�der och vind. N�gra rostiga\n"
			"tunnor har v�lt och en gr�naktig s�rja\n"
			"har runnit ut p� marken...");
		break;
	case 2:
		ImGui::TextWrapped(u8"FARTYGSVRAK\n"
			"Ett stort rostigt fartyg ligger f�rlist vid\n"
			"strandkanten, halvt nedsjunket under\n"
			"vattenytan. De runda gluggarna i skrovet\n"
			"f�r b�ten att se ut som ett stort strandat\n"
			"valkadaver. Det kan finnas skrot kvar\n"
			"i maskinrummet.");
		break;
	case 3:
		ImGui::TextWrapped(u8"KRAFTLEDNING\n"
			"Ett h�gt torn byggt av rostiga st�lbalkar\n"
			"reser sig mellan ruinerna. Det �r kn�ckt\n"
			"p� mitten, och den spretiga toppen �r\n"
			"vikt ned mot marken. D�rifr�n l�per\n"
			"tjocka ledningar, som en g�ng f�rs�g\n"
			"hela Zonen med elektricitet.");
		break;
	case 4:
		ImGui::TextWrapped(u8"MILIT�RBAS\n"
			"En stor �ppen grusplan �r omgiven av\n"
			"tegelbyggnader p� alla fyra sidor. I husen\n"
			"finns ett stort antal likadana rum med\n"
			"v�ningss�ngar i metall. Bortanf�r byggnaderna\n"
			"finns en j�ttelik garagebyggnad\n"
			"och utanf�r st�r ett utbr�nt skelett av en\n"
			"stridsvagn. Stridsvagnen kommer aldrig\n"
			"mer att r�ra sig, men kan utg�ra ett bra\n"
			"skydd vid ett hastigt �verfall.");
		break;
	case 5:
		ImGui::TextWrapped(u8"PIPELINE\n"
			"Ett grovt och rostigt metallr�r l�per genom\n"
			"Zonen, stort nog att krypa i. Det har f�r\n"
			"l�nge sedan slutat fl�da gas eller olja i\n"
			"r�ret, nu duger det bara till metallskrot.");
		break;
	case 6:
		ImGui::TextWrapped(u8"RADIOMAST\n"
			"Ett h�gt metalltorn reser sig �ver ruinlandskapet.\n"
			"Det �r uppstagat av m�ktiga vajrar\n"
			"och i toppen spretar stora rostiga antenner.\n"
			"D�rifr�n s�ndes eters�ndningar ut till\n"
			"nytta och n�je f�r den gamla v�rldens folk, \n"
			"s�gs det. Nu h�rs bara zonvinden.");
		break;
	case 7:
		ImGui::TextWrapped(u8"RAFFINADERI\n"
			"Ett helt landskap av rostiga och s�nderfallande\n"
			"maskiner breder ut sig h�r.\n"
			"Enorma cisterner binds samman av\n"
			"metallr�r, vissa tjocka nog f�r en fullvuxen\n"
			"mutant att krypa in i. Den som\n"
			"v�gar kan kl�ttra omkring i det metalliska\n"
			"mardr�mslandskapet p� rostiga\n"
			"stegar och lejdare.");
		break;
	case 8:
		ImGui::TextWrapped(u8"RENINGSVERK\n"
			"Stanken tilltar f�r varje steg n�rmare\n"
			"denna anl�ggning, men alla zonstrykare\n"
			"vet att lukten av f�rruttnelse kan d�lja en\n"
			"ov�rderlig skatt. De stora maskinerna h�r\n"
			"renade fornm�nniskans avloppsvatten\n"
			"och gjorde det drickbart igen. Rent vatten\n"
			"kanske �nnu finns i n�gon av cisternerna? ");
		break;
	case 9:
		ImGui::TextWrapped(u8"SKJUTBANA\n"
			"Under taket som str�cker sig l�ngs ena\n"
			"sidan av den �ppna ytan g�r det fortfarande\n"
			"att hitta tomhylsor. I andra �nden\n"
			"av planen finns en h�g vall och i ett rangligt\n"
			"skjul ligger m�ltavlor som f�rst�ller\n"
			"b�de m�nniskor och djur. I den gamla\n"
			"v�rlden hade man r�d att anv�nda patroner\n"
			"till att skjuta p� pappfigurer.");
		break;
	case 10:
		ImGui::TextWrapped(u8"SOPTIPP\n"
			"Ett berg av avskr�de skymmer sikten.\n"
			"H�r dumpade den gamla v�rldens folk\n"
			"allt de inte fann v�rt att spara. Mycket\n"
			"har gnagts till stoft av tidens tand, men\n"
			"plastprylar och stora stycken rostig\n"
			"metall sticker upp ur br�ten. Trots �ren\n"
			"som g�tt h�nger en fadd stank i luften.");
		break;
	case 11:
		ImGui::TextWrapped(u8"VINDKRAFTVERK\n"
			"En runt vitt torn i nu rostig metall reser\n"
			"sig ur ruinerna. H�gst upp sitter en\n"
			"enorm propeller. Det s�gs att den gamla\n"
			"v�rldens m�nniskor b�rjade bygga vindsnurror\n"
			"som den h�r f�r att f� energi n�r\n"
			"oljan i marken sinade. Det var f�rst�s alldeles\n"
			"f�r sent.");
		break;
	case 12:
		ImGui::TextWrapped(u8"OLJECISTERN\n"
			"Den stora cylinderformade byggnaden\n"
			"har en trappa som slingrar sig runt utsidan\n"
			"upp till taket. Uppe p� taket kan\n"
			"man uppt�cka faror p� stort avst�nd.\n"
			"Byggnaden inneh�ller en svart klibbig\n"
			"v�tska som en skrotskalle skulle kunna\n"
			"anv�nda som drivmedel. Det g�ller bara\n"
			"att f� ut det...");
		break;
	}
}

void humanoidBeskrivning(int i)
{
	switch (i + 1)
	{
	case 1:
		ImGui::TextWrapped(u8"DJURMUTANTER -1\n"
			"I Zonen finns en stam av varelser som inte liknar n�gra andra. De �r "
			"inte m�nniskor, de �r inte bestar - de �r h�lften av varje. De �r djur, men g�r "
			"uppr�tt p� bakbenen, anv�nder verktyg och tycks ha utvecklat ett primitivt, "
			"gr�tigt talspr�k. De �r nykomlingar i Zonen, och �r misst�nksamma mot "
			"alla de m�ter. Trots sitt ringa antal verkar djurmutanterna tillh�ra flera olika "
			"arter - n�gra liknar raggiga hundar p� tv� ben, andra �r kortvuxna och liknar "
			"gnagare och en tredje art �r apliknande kl�ttrare.\n"
			"Sanningen �r att djurmutanterna - likt Folket sj�lvt - har sitt ursprung i "
			"den gamla v�rldens genlab, framexperimenterade f�r att �verleva katastrofens "
			"h�rjningar.\n"
			"De �r djur som har beg�vats med delar av m�nniskans intellekt och finmotorik. Den "
			"lilla stam som nu r�r sig i Zonen har rymt fr�n en forskningsanl�ggning i de fj�rran "
			"bergen, d�r djurmutanter varit insp�rrade i generationer och d�r m�nga artfr�nder "
			"fortfarande h�lls f�ngna av de mystiska V�ktarna.Rymlingarna har s�kt sig till "
			"Zonen f�r att finna en ny hemvist.\n"
			"SPR�K: Djurmutanternas spr�k �r besl�ktat med Folkets, men sv�rt att f�rst� efter "
			"som det �r blandat med skall, gl�fsanden, morranden och kroppsspr�k. "
			"En rollperson m�ste lyckas med att k�nna zonen f�r att begripa spr�ket.\n"
			"\n"
			"HUND: STY 4, KYL 5, SKP 3, KNS 2,\n"
			"Sl�ss FV 1, Fly FV 1.\n"
			"\n"
			"BJ�RN: STY 5, KYL 4, SKP 2, KNS 3,\n"
			"Sl�ss FV 2.\n"
			"\n"
			"GNAGARE : STY 2, KYL 5, SKP 5, KNS 2,\n"
			"Smyga FV 2.\n"
			"\n"
			"APA : STY 3, KYL 5, SKP 4, KNS 2,\n"
			"Fly FV 2.\n"
			"\n"
			"MUTATIONER : Djuren �r en annan slags mutanter �n Folket.De har inte\n"
			"mutationer och anv�nder inte mutationspo�ng.\n"
			"VAPEN : Skrotspjut, slunga, huggt�nder (vapenskada 2, prylbonus noll).");
		break;
	case 2:
		ImGui::TextWrapped(u8"EXILMUTANTER �0\n"
			"Ett tiotal mutanter i el�ndigt skick. Deras kl�der �r i trasor, "
			"n�gra haltar eller har d�ligt l�kta s�r. De �r alla utm�rglade och "
			"de flesta �r smittade av R�tan, och kanske �ven parasitsvamp(sid 72) "
			"och magtorsk(sid 70).\n"
			"Dessa mutanter kommer fr�n en annan Ark, men har blivit utst�tta. "
			"Bem�ts exilmutanterna v�nligt tigger de om vatten och mat. �r "
			"rollpersonerna fientliga kan deras utsatthet g� �ver till vrede och "
			"trots att de �r svaga och illa rustade kan de g� till anfall. "
			"Hj�lp fr�n RP g�r gruppen �vervallande tacksam.\n"
			"\n"
			"Exilmutanterna kan ber�tta om sin forna Ark - och kan ha en ledtr�d om Eden(kapitel 8).\n"
			"GRUNDEGENSKAPER: STY 2, KYL 2, SKP 2, KNS 2.\n"
			"F�RDIGHETER : Fly FV 1, Sl�ss FV 1.\n"
			"MUTATIONER : En slumpm�ssig mutation.\n"
			"VAPEN : Tillhygge eller slunga.");
		break;
	case 3:
		ImGui::TextWrapped(u8"EXPEDITION FR�N ANNAN ARK +1\n"
			"En expedition, inte olika rollpersonernas egen, men fr�n en annan Ark.\n"
			"Fr�mlingarna �r ungef�r lika m�nga som rp.\n"
			"Om rollpersonerna inte har st�tt p� folk fr�n en fr�mmande Ark "
			"tidigare(till exempel via hotkortet Det andra Folket, sid 30) "
			"kan detta bli ett omv�lvande m�te, f�r b�da parter."
			"L�s mer om andra Arker och andra mutanter ovan.\n"
			"\n"
			"GRUNDEGENSKAPER: STY 3, KYL 4, SKP 3, KNS 2\n"
			"(eller enligt syssla\n"
			"F�RDIGHETER : Som SLP i Arken(sid 19).\n"
			"MUTATIONER : En slumpm�ssig mutation.\n"
			"VAPEN : Slagtr� med spikar eller skrotgev�r.\n"
			"SPECIAL : Minst en i gruppen �r en zonstrykare, med fv 3 i leda v�gen. De kan ha en ledtr�d om Eden(kapitel 8).");
		break;
	case 4:
		ImGui::TextWrapped(u8"KANNIBALER �0\n"
			"En skara utm�rglade mutanter kl�dda i trasor och med ansikten "
			"t�ckta av vita markeringar.De �r bev�pnade med skrotspjut och andra primitiva vapen.\n"
			"Gruppen �r mutanter fr�n en Ark d�r forskarna dog tidigt(kapitel 8) "
			"och brutala vanor blev vardag. De �r sluga och blodt�rstiga och hungrar "
			"efter f�rskt mutantk�tt. De g�r inte att resonera med och eventuell "
			"frist inneb�r enbart att de s�ker ett st�rre �vertag mot sina offer.\n"
			"\n"
			"Enbart �verv�ldigande vapenmakt och kraftig decimering av den egna "
			"gruppen kan avskr�cka kannibalerna. Deras egna som faller blir senare "
			"offer f�r deras egen hunger.\n"
			"\n"
			"GRUNDEGENSKAPER: STY 5, KYL 4, SKP 3, KNS 2\n"
			"F�RDIGHETER : Sl�ss FV 2, Fly FV 1.\n"
			"MUTATIONER : En slumpm�ssig mutation.\n"
			"VAPEN : Skrotspjut eller slunga.");
		break;
	case 5:
		ImGui::TextWrapped(u8"MINNESL�S -2\n"
			"En kvinna som stapplar fram genom Zonen.Hennes blick �r "
			"blank och riktad mot n�gon inre fasa, och hon rabblar "
			"n�got knappt h�rbart. Kvinnans kl�dsel �r m�rklig, en lortig "
			"overall med m�rkliga tecken p�. Hon �r okontaktbar och kollapsar "
			"om RP stoppar henne. Hon m�ste V�RDAS f�r att inte avlida. Om hon "
			"�verlever kan RP prata med henne, men hennes dialekt �r fr�mmande "
			"och hon verkar har mist allt minne av vem hon �r och var hon kommer "
			"fr�n. Hon kan dock ha en ledtr�d om Eden(se kapitel 8).\n"
			"\n"
			"Om ni har spelat hotkortet Mannen p� stranden(sid 32) k�nner RP "
			"igen b�de kl�dseln och dialekten - kvinnan har samma. Hon reagerar "
			"ocks� likartat om hon f�rs till Arken. Om de tv� m�rkliga fr�mlingarna "
			"f�rs samman kanske de b�rjar minnas n�got?\n"
			"\n"
			"GRUNDEGENSKAPER : STY 2 (nu 1), KYL 2 (nu 1), SKP 4, KNS 2\n"
			"F�RDIGHETER : F�rst� sig p� FV 2, V�rda FV 1.\n"
			"MUTATION : Inga.\n"
			"VAPEN : Inga.");
		break;
	case 6:
		ImGui::TextWrapped(u8"MORLOCKER �0\n"
			"Degenererade vildmutanter som tagit sin tillflykt under jord, "
			"i tunnlar, k�llare och skyddsrum. Fr�n b�rjan var morlockerna som "
			"Folket sj�lva och levde i en annan Ark, men deras kroppar och sinnen "
			"har f�rfallit och deras civilisation g�tt f�rlorad. Morlockerna har "
			"gl�mt den gamla v�rldens spr�k och talar genom grymtningar och tecken. "
			"De �r inte fientliga, snarare avvaktande, och kan hj�lpa rollpersonerna "
			"om de f�r n�got i utbyte.\n"
			"De f�rsvarar sig med djuriskt ursinne om de blir angripna.\n"
			"\n"
			"De kan ha en ledtr�d om Eden(kapitel 8).\n"
			"\n"
			"GRUNDEGENSKAPER: STY 2, KYL 3, SKP 5, KNS 2\n"
			"F�RDIGHETER : Fly FV 2, Sl�ss FV 1.\n"
			"MUTATIONER : En slumpm�ssig mutation.\n"
			"VAPEN : Skrotspjut.");
		break;
	case 7:
		ImGui::TextWrapped(u8"MOTORFOLK �0\n"
			"Ett d�n som av tilltagande �ska h�rs �ver Zonen. "
			"Snart mullrar en hord av bisarra fordon fram mellan ruinerna.\n"
			"Bilar, bussar, motorcyklar och lastbilar; alla prydda av vassa spikar, "
			"d�dskallar och fornskrot. Det fruktade motorfolket �r mutanter "
			"som l�mnat sin Ark f�r l�nge sedan och hade turen att komma �ver "
			"en stor samling fungerande fordon fr�n den gamla v�rlden.\n"
			"\n"
			"GRUNDEGENSKAPER: STY 4, KYL 5, SKP 2, KNS 3\n"
			"F�RDIGHETER : Skjuta FV 3, Sl�ss FV 2, Mecka FV 1.\n"
			"MUTATIONER : En slumpm�ssig mutation.\n"
			"VAPEN : Motorcykelkedja, skrotgev�r, skrotrustning(skyddsv�rde 3)."
			"SPECIAL: Motorfolket har bilar, mopeder och andra fordon (se sidan 97 f�r speldata), ofta bestyckade med eldkastare eller till och med skrotkanoner. L�s mer om motorfolket p� sidan 119.");
		break;
	case 8:
		ImGui::TextWrapped(u8"PATRULL FR�N ARKEN\n"
			"En patrull fr�n Arken, som har hittat en v�rdefull artefakt "
			"eller s�ger sig veta var en finns. Ledaren vill antingen f� "
			"hj�lp med att f� fyndet till Arken eller hitta det. En av "
			"deltagarna i patrullen kan g�rna vara en SLP som en eller "
			"flera rollpersoner har en relation till - kanske �r det till "
			"och med ledaren. Patrullen har splittrad lojalitet d� h�lften "
			"�r underhuggare till en boss, resten till en annan.\n"
			"\n"
			"GRUNDEGENSKAPER: Beroende p� syssla, som slp i Arken(sid 19).\n"
			"F�RDIGHETER : Som slp i Arken(sid 19).\n"
			"MUTATIONER : En slumpm�ssig mutation var.\n"
			"VAPEN : Slagtr� med spikar, skrotkniv, skrotgev�r.\n"
			"SPECIAL : Minst en i gruppen �r en zonstrykare, med fv 3 i V�gvisare.");
		break;
	case 9:
		ImGui::TextWrapped(u8"SEKTEN NOVA -1\n"
			"En procession av m�n och kvinnor med rakade skallar och kl�dda i l�nga sl�jor."
			"Gruppen l�mnar n�gra bylten p� ett betongblock och r�r sig sedan "
			"hastigt d�rifr�n.G�r RP fram hittar de en handfull sp�dbarn. "
			"Alla �r nedkylda och svaga och risken �r stor att alla d�r. "
			"�verlever n�gon kommer denne uppvisa kraftiga mentala mutationer."
			"De besl�jade tillh�r den mytomspunna sekten Nova(sid 35). "
			"Om RP stoppar dem behandlas de som luft, eller tilltalas "
			"som dumma barn. Tar RP till v�ld tvekar sektmedlemmarna inte "
			"att anv�nda sina mentala mutationer. RP kan ocks� skugga dem "
			"och hitta v�gen till deras bunker(sid 113)."
			"\n"
			"GRUNDEGENSKAPER: STY 2, KYL 3, SKP 5, KNS 4"
			"F�RDIGHETER : Fly FV 2, V�rda FV 1."
			"MUTATIONER : En mutation, men enbart av mentalt slag(Mardr�mmare, Mental dominans, Parasit, Patokinesi, Pyrokinesi eller Telepati)."
			"Novas medlemmar �r m�ktiga mentala mutanter och har normalt 5 MP fr�n start i st�llet f�r 3."
			"VAPEN : -");
		break;
	case 10:
		ImGui::TextWrapped(u8"SKROTORAKEL -1\n"
			"En kort och satt mutant kl�dd i en grotesk utstyrsel prydd av "
			"otaliga f�rem�l fr�n forntiden. Han �r en s�rling med kraftiga "
			"mentala mutationer. Mutanten �r telepat och kan f� starka visioner "
			"om RP:s m�jliga �de. Skrotoraklets syner kan hj�lpa RP med att "
			"tolka en ledtr�d om Eden, ge dem en ledtr�d om de saknar en, "
			"guida dem till l�sningen f�r hur en �nnu ok�nd artefakt fungerar "
			"eller kanske leda dem till var en m�ktig artefakt g�r att hitta.\n"
			"\n"
			"GRUNDEGENSKAPER : STY 2, KYL 3, SKP 4, KNS 3\n"
			"F�RDIGHETER : Manipulera FV 2.\n"
			"MUTATIONER : Telepati, Patokinesi, R�t - �tare\n"
			"VAPEN : -\n"
			"SPECIAL : Skrotoraklet �r en m�ktig mental mutant, och har 5 mp.");
		break;
	case 11:
		ImGui::TextWrapped(u8"SMITTADE �0\n"
			"En grupp degenererade vildmutanter fr�n en Ark som f�rfallit och g�tt under.\n"
			"De �r sorgliga trashankar, t�ckta av b�lder och utv�xter. "
			"Alla har deformerade kroppsdelar eller sjukliga lyten. "
			"En del har �ppna s�r ur vilket tjockt var flyter. Andra uts�ndrar "
			"stinkande sekret ur svulster och kropps�ppningar. De �r en tragisk "
			"samling mutanter i djupt stadium av s�nderfall och sjukdom som "
			"varken har ork eller vilja till annat �n att sakta vandra tills de stupar.\n"
			"\n"
			"GRUNDEGENSKAPER: STY 1, KYL 1, SKP 1, KNS 1\n"
			"F�RDIGHETER : -\n"
			"MUTATIONER : En slumpm�ssig mutation.\n"
			"VAPEN : -\n"
			"SPECIAL : Bara att vistas en stund i n�rheten av de smittade r�knas "
			"som en r�tattack(kapitel 7 i Regelboken). Varje direktkontakt med "
			"deras kroppar r�knas som ytterligare en r�tattack. De smittade b�r "
			"sannolikt �ven p� parasitsvamp(sid 72) och magtorsk(sid 70).");
		break;
	case 12:
		ImGui::TextWrapped(u8"UNDERG�NGSKULT �0\n"
			"En skara mutanter, sedan �ratal utst�tta fr�n sin Ark, som dyrkar "
			"en s�rskild ruin eller artefakt i Zonen. De har mist all sans och "
			"lever bara f�r att tillbedja sitt kultf�rem�l, som de tror ska locka "
			"den gamla v�rldens m�nniskor tillbaka till jorden. Kultisterna kan "
			"f�rs�ka v�rva rollpersoner och SLP, eller f�nga in dem f�r att offra "
			"dem till sin gud. Sekten kan ha en ledtr�d om Eden(kapitel 8).\n"
			"\n"
			"GRUNDEGENSKAPER: STY 3, KYL 2, SKP 3, KNS 4\n"
			"F�RDIGHETER : Sl�ss FV 1, Manipulera FV 2.\n"
			"MUTATIONER : En slumpm�ssig mutation.\n"
			"VAPEN : Tillhygge.");
		break;
	case 13:
		ImGui::TextWrapped(u8"VANDRARE -1\n"
			"En mager och bister skara m�nniskor i lappade och trasiga kl�der. "
			"De �r p� sin vakt men inte direkt fientliga - om RP �r fredliga "
			"kan fr�mlingarna ber�tta, p� en m�rklig dialekt, att de har vandrat "
			"i �ratal fr�n bergen i v�ster. De ber om krubb och vatten. De vill "
			"veta var RP:s bos�ttning finns, och kan bli hotfulla om RP v�grar "
			"svara. Om ni redan har spelat hotkortet Vandrarna fr�n bergen(sid 36) "
			"kan detta vara en grupp utbrytare, som har drivits bort fr�n "
			"huvudgruppen efter en maktkamp.\n"
			"Slutligen kan dessa vandrare ha en ledtr�d om Eden(kapitel 8).\n"
			"\n"
			"GRUNDEGENSKAPER : STY 4, KYL 3, SKP 4, KNS 3\n"
			"F�RDIGHETER : Skjuta FV 3, Sl�ss FV 2, F�rst� sig p� FV 1.\n"
			"MUTATIONER : Inga.\n"
			"VAPEN : Skrotkniv, vedyxa, en artefakt.");
		break;
	case 14:
		ImGui::TextWrapped(u8"VATTENHANDLARE �0\n"
			"En ensam resen�r som trotsar Zonens faror f�r att handla med rent vatten. "
			"Det �r en storv�xt mutant som sl�par p� en skrotk�rra, f�ljd av tv� "
			"strykarhundar. P� k�rran finns plastflaskor, pl�tdunkar och en underlig "
			"konstruktion av tuber och r�r.\n"
			"Det senare �r ett hemmabyggt reningsverk drivet av ett sotigt kokk�rl. "
			"Vattenhandlaren har ett �ppet och trevligt s�tt och �r alltid redo f�r aff�rer. "
			"Han �r inte dumdristig och kapabel till att f�rsvara sig. "
			"Mutanten vet mycket om Zonen och kan ber�tta var andra bos�ttningar finns.\n"
			"Han kan ocks� ha en ledtr�d om Eden.\n"
			"Handlaren tar glatt emot information om var Folkets Ark finns.\n"
			"\n"
			"GRUNDEGENSKAPER: STY 2, KYL 3, SKP 4, KNS 5\n"
			"F�RDIGHETER : Manipulera FV 2, Skjuta FV 1.\n"
			"MUTATIONER : En slumpm�ssig mutation.\n"
			"VAPEN : Tillhygge, skrotpistol(5 patroner)\n"
			"SPECIAL : Vattenhandlaren har r�tfritt vatten att s�lja. "
			"Det kan dock inneh�lla magtorsk(sid 70).Priset beror, som alltid, p� efterfr�gan.");
		break;
	case 15:
		ImGui::TextWrapped(u8"VRAKR�VARE �0\n"
			"Ett ov�ntat intakt fordonsvrak st�r or�rt i en gl�nta eller p� ett ruintorg. "
			"Vrakets rutor �r smutsiga, luckorna �r l�sta och skrovet �r av tjock pansarpl�t. "
			"Vraket �r en f�lla. En skock vildmutanter eller zongastar rustade med skrotpansar "
			"och enkla vapen anv�nder fordonet som lockbete f�r nomader och vandrare.\n"
			"Deras vanliga plan �r simpel: de rusar fram och s�ker genom numer�rt "
			"�verl�ge �vervinna sina offer. De plundrar sina byten p� allt och sl�par "
			"iv�g dem som slavar till sin stam. Mutanterna har sj�lva inte kunnat "
			"�ppna vraket. Lyckas n�gon ta sig in i det hittar man tv� artefakter i "
			"mycket gott skick.\n"
			"\n"
			"GRUNDEGENSKAPER : STY 3, KYL 4, SKP 3, KNS 2\n"
			"F�RDIGHETER : Sl�ss FV 3, Skjuta FV 2, Smyga FV 1.\n"
			"MUTATIONER : En slumpm�ssig mutation om vrakr�varna �r vildmutanter, "
			"ingen mutation om de �r zongastar(nedan).\n"
			"VAPEN : Slagtr� med spikar, slunga.");
		break;
	case 16:
		ImGui::TextWrapped(u8"ZONGASTAR �0\n"
			"Zongastar �r vad Folket ber�ttar skr�ckhistorier vid l�gerelden om. "
			"Detta vildsinta folk, som lever som nomader ute i Zonen, r�r sig som "
			"skuggor i ruinerna och anfaller andra zonvandrare utan f�rvarning. "
			"De �r mest aktiva om natten, och �r s�llan ute i dagsljus. Ryktena om "
			"zongastarna �r m�nga, det s�gs att de fl�r och �ter mutanter som de f�ngat.\n"
			"De �r som regel kortv�xta och r�r sig ofta i stora grupper. Zongastar "
			"d�ljer sina ansikten bakom skr�ckinjagande masker fr�n den gamla tiden och g�r oftast kl�dda i plastrockar som d�ljer deras huvuden.De sl�ss med grova tillhyggen, slungor och stenar.De anv�nder s�llan artefakter.\n"
			"\n"
			"Beskriv zongastarna som skr�ckinjagade f�rsta g�ngen rollpersonerna m�ter dem. "
			"De �r smygande fasor i ruinerna, dolda hot, st�ndigt p� jakt. Ber�tta ryktena "
			"som g�r om zongastarna i Arken. I strid �r de fasansfulla vildar utan n�d "
			"och besinning.\n"
			"Senare, om RP m�ter zongastarna igen, ge dem en chans att se en annan "
			"sida av de fruktade fienderna.Kanske st�ter de p� en s�rad zongast.\n"
			"\n"
			"SPR�K: Zongastar har ett primitivt gutturalt spr�k som Folket har mycket "
			"sv�rt att f�rst�. En rollperson m�ste lyckas K�NNA ZONEN f�r att f�rst� det.\n"
			"GRUNDEGENSKAPER : STY 3, KYL 4, SKP 3, KNS 2\n"
			"F�RDIGHETER : Fly FV 3, Skjuta FV 2, Sl�ss FV 1.\n"
			"MUTATIONER : Inga.\n"
			"VAPEN : Cykelkedja, slunga.\n"
			"SPECIAL : I direkt dagsljus tar zongastar en po�ng skada per timme, "
			"trots sina skyddsdr�kter.\n"
			"\n"
			"SANNINGEN OM ZONGASTARNA\n"
			"Vad Folket inte vet �r att zongastarna �r �ttlingar till m�nniskor "
			"som �verlevde underg�ngen. Mot alla odds har de �verlevt R�tan och "
			"hungern.Men de blir inte gamla, de flesta �r ton�ringar.\n"
			"Under alla paltor �r deras kroppar bleka och krumma, inte s�llan "
			"f�rvridna. Zongastarna har f�rlorat alla minnen av den m�nskliga "
			"civilisationen genom �ren, och de ser Folket som ett d�dligt hot.\n"
			"Om grupperna kunde �vervinna skr�cken f�r den andre kanske de kunde "
			"bli v�rdefulla allierade.\n"
			"Zongastarna kan ocks� bli en m�jlig - om �n oaptitlig - l�sning p� "
			"Folkets of�rm�ga att f�r�ka sig(kapitel 8).");
		break;
		
	}
}
void monsterBeskrivning(int i)
{
	switch (i + 1)
	{
	case 1:
		ImGui::TextWrapped(u8"ASGNAGARE +1\n"
			"Dessa utm�rglade, skabbiga gnagare lever i horder i Zonen. "
			"R�tan tycks bara g�ra dem st�rre och elakare. Zonens r�ttor "
			"�r l�nga som en mansarm och gnager i sig allt de kommer �ver, "
			"levande som d�tt, med sina gulnade och sylvassa huggt�nder.\n"
			"\n"
			"GRUNDEGENSKAPER: STY 8, KYL 5\n"
			"PANSAR : -\n"
			"VAPEN : Sylvassa t�nder(vapenskada 1).\n"
			"SV�RM : Kan endast skadas av eldkastare, explosioner och mutationer.\n"
			"R�TA : Varje attack fr�n asgnagare, oavsett om den lyckas eller "
			"inte, ger offret en r�tpo�ng.");
		break;
	case 2:
		ImGui::TextWrapped(u8"BETESDJUR +1\n"
			"H�r och var i Zonen lever flockar av relativt beskedliga betesdjur, "
			"muterade versioner av den gamla tidens f�r, getter, grisar och kor. "
			"De �r f�rvildade och tvekar inte att anfalla zonvandrare som de "
			"uppfattar som hot mot flocken. Betesdjur �r s�rskilt l�mpliga att "
			"jaga f�r att f� krubb(se talangen Slaktare i Regelboken).\n"
			"\n"
			"GRUNDEGENSKAPER: STY 2 - 5, KYL 2 - 4\n"
			"PANSAR : -\n"
			"VAPEN : Horn, bett eller trynen(vapenskada 1).");
		break;
	case 3:
		ImGui::TextWrapped(u8"DR�PARTR�D -1\n"
			"Ett omr�de som �r obehagligt tyst och stilla. Det �r hemvist "
			"�t Zonens fruktade modergr�nska; ett dr�partr�d. Det best�r av "
			"en m�rk stam som slutar i ormande tentakelgrenar. Knotiga r�tter "
			"t�ckta av illaluktande sekret har spr�ckt ruiner och mark. Rester "
			"av ben, skinn och fj�drar ligger �verallt runt om. "
			"Dr�partr�dets m�rkliga fr�kapslar - humanoida kringvandrande "
			"v�xtmutanter som undertvingade dr�partr�dets vilja jagar �t "
			"rovgr�nskan - finns i omr�det och kommer g�ra allt f�r att "
			"f�nga in levande v�sen som r�r sig n�ra och mata dem �t sin hungrande tr�dmoder.\n"
			"\n"
			"GRUNDEGENSKAPER: STY 10, KYL 6\n"
			"F�RDIGHETER : Sl�ss FV 3.\n"
			"PANSAR : 6\n"
			"VAPEN : Tentakler(vapenskada 1, vid tr�ff blir offret sn�rjt "
			"och m�ste Ta krafttag f�r att komma loss, �ven v�nner intill "
			"kan sl�), ringformat gap(vapenskada 3, kr�ver att offret �r sn�rjt).\n"
			"\n"
			"TANKEV�GOR :\n"
			"Dr�partr�det lockar sina offer till sig med kraftfulla tankev�gor. "
			"Alla inom kort avst�nd m�ste klara att FLY f�r att st� emot - den "
			"som misslyckas tar en po�ng f�rvirring och m�ste avancera fram mot tr�det.\n"
			"\n"
			"TENTAKLER :\n"
			"N�r ett offer v�l �r p� n�ra avst�nd kan dr�partr�det SL�SS med sina "
			"tentakler. Tr�det kan f�rdela sina grund - och f�rdighetst�rningar p� "
			"flera offer och angripa dem i samma runda.\n"
			"\n"
			"FR�KAPSLAR :\n"
			"Offer som lyckas motst� dr�partr�dets lockelser blir som regel angripna "
			"av tr�dets humanoida fr�kapslar.\n"
			"I sitt naturliga tillst�nd har dessa varelser gr�naktig hy och saknar "
			"kroppsbeh�ring och tydliga anletsdrag.\n"
			"\n"
			"En fr�kapsel har STY 3 och KYL 5 och FV 2 i att SL�SS.\n"
			"De kan dock efterapa en humanoids exakta utseende, genom att skicka in "
			"v�xtst�nglar i offrets hj�rna och suga ut information om offrets DNA. "
			"Offret, som m�ste vara fasth�llet eller brutet av skada f�r att processen "
			"ska g� att genomf�ra, d�r p� kuppen. En fr�kapsel som efterapat en "
			"m�nniska eller mutant �r helt identisk med offret och kan �ven tala med "
			"dess r�st, men �r totalt k�nslokall och g�r inte att MANIPULERA eller MUCKA med.");
		break;
	case 4:
		ImGui::TextWrapped(u8"DR�MBLOMSTER -1\n"
			"Ett omr�de fyllt av blommor som kl�nger och v�xer �verallt. Tunga "
			"fuktiga kronblad, st�ndare och pistiller samt intensiva f�rger utm�rker dem.\n"
			"Om n�got r�r sig i omr�det s�nder zonblommorna ut s�vande moln av sporer. "
			"N�r n�got faller i s�mn ringlar taggiga utskott fram, borrar sig in i "
			"offrets mjukdelar och suger i sig n�ring tills offret �r d�tt. Resterna "
			"av tidigare offer �r t�ckta av den snabbt v�xande gr�nskan. En hel del "
			"prylar och kanske till och med en artefakt eller tv� ligger p� marken i "
			"omr�det, dolda av blomstren.\n"
			"\n"
			"EFFEKT:\n"
			"Den som Spejar mot omr�det och lyckas f�r syn p� rester av tidigare offer. "
			"Den som r�r sig in i omr�det m�ste klara att Fly f�r att inte somna p� "
			"st�llet.Den som s�vs tar en po�ng skada varje runda tills han blir bruten "
			"eller tills n�gon V�rdar honom. Rustning ger normalt skydd, men bara "
			"tills offret har tagit sin f�rsta po�ng skada. Dr�mblomstren kan enbart "
			"bek�mpas med eld - r�kna dess STY som 10. Vid noll �r v�xterna d�da och "
			"omr�det kan passeras utan risk.");
		break;
	case 5:
		ImGui::TextWrapped(u8"ETTERBEST +1\n"
			"N�ra Zonens vattendrag lever varelser som zonstrykare kallar "
			"etterbestar - f�rvuxna �dlor, l�nga som tv� mutanter om man r�knar in svansen.\n"
			"Etterbestar �r intelligenta och det s�gs till och med att de "
			"g�r att t�mja som riddjur. Att f�rs�ka med det �r dock en riskabel "
			"aff�r, med tanke p� etterbestarnas vresiga hum�r och den f�rm�ga som "
			"givit dem deras namn : de spottar fr�tande syra med f�r�dande tr�ffs�kerhet.\n"
			"\n"
			"GRUNDEGENSKAPER : STY 5, KYL 4 (l�gre i kallt v�der)\n"
			"F�RDIGHETER : K�mpa p� FV 3, Sl�ss FV 2, Skjuta FV 3.\n"
			"PANSAR : 2\n"
			"VAPEN : Bett(vapenskada 2), syraspott(r�ckvidd n�ra, vapenskada 1, "
			"rustning skyddar normalt).");
		break;
	case 6:
		ImGui::TextWrapped(u8"FR�TARGR�S -1\n"
			"Ett f�lt av midjeh�gt gr�s. En sur doft ligger �ver platsen och h�r "
			"och var skymtar uttorkade kadaver eller vitnande knotor.\n"
			"Gr�set b�jer sig till och fr�n som om en osynlig vind sveper fram. "
			"V�xtligheten �r ett f�lt av l�mskt rovgr�s. Om djur eller mutanter "
			"g�r in i det b�rjar gr�set sno sig runt deras f�tter, ben och f�rem�l. "
			"Om gr�set sn�rjer n�got b�rjar det uts�ndra ett fr�tande sekret. Offret "
			"sm�lts ned till en s�rjig gegga som v�xten kan suga i sig.\n"
			"\n"
			"EFFEKT: En RP som Spejar mot omr�det och lyckas f�r syn p� rester av "
			"tidigare offer. Den som r�r sig in i omr�det blir omg�ende angripen med "
			"sex grundt�rningar.\n"
			"Vid tr�ff blir offret sn�rjt och tar direkt en po�ng skada av fr�tande "
			"sekret. Rustning skyddar normalt. F�r att ta sig loss m�ste offret Ta krafttag. "
			"Till dess tar han en po�ng skada till varje runda. Fr�targr�set kan "
			"enbart bek�mpas med eld � r�kna dess STY som 10. Vid noll �r gr�set "
			"d�tt och omr�det kan passeras utan risk.");
		break;
	case 7:
		ImGui::TextWrapped(u8"JORDSLUKARE �0"
			"En best som jagar sina byten genom att luckra upp marken ovanf�r "
			"sig och v�nta p� att ett of�rsiktigt offer skall g� ned sig. "
			"Jordslukaren �r en avl�ng och skalt�ckt varelse i dubbel mansl�ngd "
			"med otaliga ben och kraftiga mandibler. Den �r entr�gen och enveten i "
			"sina attacker och anfaller till offret �r nedk�mpat eller jordslukaren g�r under."
			"\n"
			"GRUNDEGENSKAPER: STY 6, KYL 4"
			"F�RDIGHETER : Sl�ss FV 3, Smyga FV 5 (under jord)."
			"PANSAR : -"
			"VAPEN : Ringformat gap(vapenskada 2)."
			"Den som blir tr�ffad blir �ven nerdragen i jorden. Offret m�ste TA KRAFTTAG "
			"f�r att komma loss(r�knas som en handling). S� l�nge offret sitter "
			"fast f�r jordslukaren + 3 p� alla attacker mot det. Om ett offer kommer "
			"loss brukar jordslukaren ge upp och Fly.");
		break;
	case 8:
		ImGui::TextWrapped(u8"MAGTORSK �0\n"
			"Magtorsken �r en fruktad parasitvarelse som sprids via dricksvatten. "
			"�ven vatten som i �vrigt �r r�tfritt och drickbart kan vara smittat - "
			"det finns inget s�kert s�tt att skydda sig mot magtorsken utom att koka "
			"vattnet. Parasiten v�xer inne i buken p� v�rdkroppen och livn�r sig p� "
			"maten som v�rden �ter. N�r magtorsken, som snarare liknar en �l, �r "
			"fullvuxen �ter den sig ut genom buken och slingrar sig i v�g till n�rmaste vattendrag.\n"
			"\n"
			"EFFEKT: Efter smittotillf�llet blir offret fruktansv�rt hungrigt och "
			"m�ste �ta minst tv� ransoner krubb per dag. Varje utebliven ranson ger ett i skada.\n"
			"Efter n�gra dagar eller en vecka �r magtorsken fullvuxen och �ter sig ut. "
			"Offret blir automatiskt bruten av skada, oavsett STY, och m�ste bli V�RDAD	 "
			"f�r att inte avlida.");
		break;
	case 9:
		ImGui::TextWrapped(u8"MASKINVARELSE -2\n"
			"I enklavernas m�rker byggde titanmakterna(kapitel 8) varelser av metall, plast och kretsar som var s� avancerade att de kunde m�ta sig med oss m�nniskor i intelligens och initiativf�rm�ga.\n"
			"Vissa forskare experimenterade till och med att sm�lta samman m�nniska och maskin, i hopp om att skapa en varelse b�ttre l�mpad f�r livet efter katastrofen.\n"
			"\n"
			"Robotar, av en m�ngd olika slag, kommer att utforskas grundligt i den kommande modulen Mutant : Maskinarium.Men redan i Mutant : �r Noll kan rollpersonerna st�ta p� enstaka maskinvarelser som av n�gon anledning befinner sig i Zonen.\n"
			"F�r Folket l�r en s�dan robot framst� som en fruktansv�rd styggelse, en vandrande eller sv�vande metallvarelse med d�dliga vapen inbyggda i sj�lva kroppen.\n"
			"\n"
			"GRUNDEGENSKAPER : STY 8, KYL 6\n"
			"F�RDIGHETER : Sl�ss FV 4, Skjuta FV 4.\n"
			"PANSAR : 10\n"
			"VAPEN : S�gklinga(vapenskada 2), energivapen(vapenskada 2, l�ng r�ckvidd).\n"
			"Vapnen �r inbyggda och ger ingen prylbonus.\n"
			"SPECIAL : Roboten kan inte drabbas av stress, dess v�rde i KYL g�r aldrig ner.");
		break;
	case 10:
		ImGui::TextWrapped(u8"MASKSV�RM -1\n"
			"Dessa kluster av decimeterl�nga svarta maskar har utvecklat ett gemensamt medvetande och beter sig mer som en enda varelse �n som separata individer.\n"
			"En typisk sv�rm best�r av miljontals maskar och kan bli flera meter i omf�ng.\n"
			"Det d�ljer sig ofta i m�rka och fuktiga platser och angriper byten som passerar i n�rheten. Angreppet sker genom att sv�rmen hastigt f�rs�ker omsluta offret och f�rs�tta det i panik, f�r att sedan f�rt�ra det.\n"
			"\n"
			"GRUNDEGENSKAPER: STY 8 (normalt startv�rde som kan �ka och minska, se nedan)\n"
			"PANSAR : -\n"
			"VAPEN : Omslutning(vapenskada 1).\n"
			"Den f�rsta attacken ger stress. Forts�tter angreppet tar offret skada fr�n runda tv� och vidare. F�r varje po�ng skada offret tar �kar masksv�rmens STY ett steg.\n"
			"SV�RM : Masksv�rmen kan endast skadas av eldkastare, explosioner och mutationer.");
		break;
	case 11:
		ImGui::TextWrapped(u8"PARASITSVAMP �0\n"
			"Denna d�dliga svampvarelse smittar genom mycel som sprids vid n�rkontakt med en infekterad kropp. Efter smittotillf�llet b�rjar parasiten v�xa inne i den nya v�rdens kropp och hj�rna, som upplever fruktansv�rda sm�rtor. Svamporganismen p�verkar ocks� offrets hj�rna.\n"
			"\n"
			"EFFEKT: Efter smittotillf�llet tar offret en po�ng skada och en po�ng f�rvirring per dygn. Offret m�ste allts� �ta och sova onormalt mycket f�r att inte bli bruten. D� och d� f�rs�ker parasitsvampen tvinga offret att komma n�ra ett nytt offer, f�r att sprida smittan vidare. Offret m�ste klara ett slag f�r Kyla(ingen f�rdighet kan anv�ndas) f�r att st� emot impulsen. Det enda s�ttet att bli fri fr�n smittan �r att bli bruten och sedan bli V�rdad.");
		break;
	case 12:
		ImGui::TextWrapped(u8"PSI-MYGG -1\n"
			"Ett omr�de fyllt av surrande sv�rmar av mygg med psioniska krafter. Alla som r�r sig i deras n�rhet blir deprimerade och uppgivna. Allt tycks hoppl�st f�r de som uts�tts. Offren f�r en allt starkare �nskan att inte g�ra n�got, vilket l�ter myggorna landa och suga friskt. Attacken �r inte fysiskt d�dlig, men uppgivenheten kan i sig kosta en zonfarare livet.\n"
			"\n"
			"EFFEKT: Individer som drabbas av psimyggen f�r varje runda en attack mot sig med ett antal grundt�rningar lika med sv�rmens STY. Varje / ger ett po�ng tvivel (trauma mot KNS). F�r att samla sig och ta sig i skydd kr�vs att man lyckas Fly. Myggsv�rmen kan enbart bek�mpas med eld eller explosioner. N�r sv�rmens STY n�r noll �r den helt bek�mpad. En typisk sv�rm psi-myggor i Zonen har STY 8 fr�n start.");
		break;
	case 13:
		ImGui::TextWrapped(u8"RAGGBEST �0\n"
			"Ett vildsint och storv�xt rovdjur med n�ra mansh�jd �ver manken. Besten har smutsgr� p�ls och utm�rks av ett orimligt stort gap, en outsinlig hunger och f�rm�ga att r�ra sig n�st intill ljudl�st.\n"
			"Raggbesten smyger sig med o�ndligt t�lamod mot sitt offer och det �r s�llan bytet hinner uppfatta vad som h�nder innan monstret halvt slukat sitt byte. K�ften kan vidgas s� att besten utan problem kan hugga en storv�xt mutant runt �verkroppen. Fr�tande saliv sm�lter sedan s�nder offret som inte sl�pps f�rr�n allt �r svalt eller raggbesten tvingas fly.\n"
			"\n"
			"GRUNDEGENSKAPER: STY 6, KYL 5\n"
			"F�RDIGHETER: Sl�ss FV 3, Smyga FV 4.\n"
			"PANSAR: 2\n"
			"VAPEN: Klor (vapenskada 1), bett (vapenskada 3, men kan bara anv�ndas om offret �r ovetande om attacken).\n"
			"Om ett bett lyckas sitter offret fast och tar 1 po�ng skada varje runda. F�r att komma loss m�ste offret (eller en kamrat) Ta krafttag (modifikation -2). Varje slag r�knas som en handling.");
		break;
	case 14:
		ImGui::TextWrapped(u8"RUINSPINDEL �0\n"
			"I m�rka tunnlar och ruiner lever muterade spindlar som har vuxit till grotesk storlek. Of�rsiktiga zonvandrare som trampar igenom gamla murkna tunneltak eller utforskar �dsliga ruiner kan hamna mitt ibland dem. Insekterna livn�r sig p� allt ifr�n lik och avskr�de till storv�xta byten som de tar d�d p� med sitt giftbett.\n"
			"\n"
			"GRUNDEGENSKAPER: STY 5\n"
			"F�RDIGHETER: Sl�ss FV 2, Fly FV 2.\n"
			"PANSAR: 3\n"
			"VAPEN: Giftbett (vapenskada 1 och f�rlamande gift, se nedan).\n"
			"NERVGIFT: Om giftbettet g�r skada tr�nger giftet in och g�r en po�ng stress per runda tills offret v�rdas. Medan giftet verkar l�ker vatten inte kyl.");
		break;
	case 15:
		ImGui::TextWrapped(u8"R�TMYROR �0\n"
			"R�tmyrorna �r en legend bland zonstrykare.\n"
			"Det farliga �r inte insekterna i sig - nog f�r att de �r otrevliga nog, stora som en handflata och med kraftiga k�kar - utan deras �verv�ldigande antal. R�tmyror bygger inga stackar, i st�llet v�ller de fram i m�ngmiljonh�vdade horder genom Zonen och f�rintar allt i sin v�g.\n"
			"De �r blinda och uppfattar allt levande som f�da. Det finns zonstrykare som s�ger sig ha sett en sv�rm r�tmyror sluka en raggbest p� ett par minuter. Insekterna blir inte trevligare av att de sprider stark r�ta omkring sig d�r de drar fram.\n"
			"\n"
			"GRUNDEGENSKAPER: STY 6\n"
			"PANSAR : -\n"
			"VAPEN : Bett(vapenskada 1).\n"
			"R�TA : Varje attack, oavsett om den lyckas eller inte, ger offret en r�tpo�ng.\n"
			"SV�RM : R�tmyrornas sv�rm �r enorm. Den g�r �verhuvudtaget inte att skada med vapen som rollpersonerna kan t�nkas f�rfoga �ver. Det enda s�ttet att undkomma �r att FLY.");
		break;
	case 16:
		ImGui::TextWrapped(u8"SKR�DESVR�K �0\n"
			"Fr�n h�ga ruiner spanar skr�desvr�ken ut �ver Zonen med sina gula �gon. Gr�bruna fj�drar l�ter den sm�lta in omgivningen. Sitt namn har besten f�tt av sina muterade klor som liknar l�nga knivblad. Skr�desvr�ken jagar genom att dyka ner p� sina offer och med klorna knipsa av l�mplig del, eller bara ta tag i offret och flyga iv�g. De st�rsta bestarna har m�tt upp till tio meter mellan vingspetsarna.\n"
			"Yngre eller mindre exemplar n�jer sig oftast med att riva ut �gon eller knipsa av �ron.\n"
			"\n"
			"GRUNDEGENSKAPER: STY 8, KYL 8\n"
			"F�RDIGHETER: Sl�ss FV 3, Fly 4, Ta krafttag FV 2.\n"
			"PANSAR: 1\n"
			"VAPEN: Klor (vapenskada 2, vid tr�ff slits ett rej�lt k�ttstycke loss).\n"
			"ST�RTDYKNING: En skr�desvr�k i luften kan st�rtdyka fr�n l�ngt avst�nd och direkt sl�ss med ett offer i samma runda. Vr�ken tar sj�lv ett i skada av attacken.\n"
			"LYFTA OFFER: N�r skr�desvr�ken lyckats med att sl�ss kan den lyfta upp offret och flyga iv�g med det genom att klara ett slag f�r ta krafttag.");
		break;
	case 17:
		ImGui::TextWrapped(u8"SK�VLARE\n"
			"Ett ofantligt monster som n�r n�ra dubbel mansh�jd �ver manken och dubbelt i l�ngd. Trots besten sm� ben kan den f� upp en skr�mmande fart och under det trubbiga trynet sitter tv� grova betar som sliter s�nder allt ifr�n vrak och tr�d till mutanter och skrotpalissader.\n"
			"Allt tycks reta upp varelsen och att f� stopp p� en rasande sk�vlare �r n�ra om�jligt. Besten verkar inte vara ett utpr�glat rovdjur och jagar inte f�r f�da, d�remot �ter den lika g�rna as och lik som frodig gr�nska\n"
			"\n"
			"GRUNDEGENSKAPER: STY 10, KYL 6\n"
			"F�RDIGHETER: Sl�ss FV 4.\n"
			"PANSAR: 8\n"
			"VAPEN: Tjurrusning (vapenskada 2, m�ste inledas p� kort avst�nd, ger en extra man�ver till f�rflyttning f�re attacken), betar(vapenskada 3).");
		break;
	case 18:
		ImGui::TextWrapped(u8"SKROTKR�KOR +1\n"
			"Stora smutsgr� f�glar som lever i flockar i Zonen. De anfaller g�rna of�rsiktiga zonvandrare och hackar besinningsl�st med sina vassa n�bbar mot ansikte och �gon. Dessa motbjudande fj�derf�n kallas skrotkr�kor av Folket, eftersom de finner allt som bl�nker oemotst�ndligt lockande - s�rskilt dyrbart fornskrot.\n"
			"\n"
			"GRUNDEGENSKAPER: STY 7, KYL 6\n"
			"PANSAR: -\n"
			"VAPEN: N�bbar och klor (vapenskada 1).\n"
			"SV�RM: Kan endast skadas av eldkastare, explosioner och mutationer.");
		break;
	case 19:
		ImGui::TextWrapped(u8"STRYKARHUNDAR +1\n"
			"Hungriga och muterade flockar av byrackor �r vanliga i Zonen. De �r magra, seniga och saknar ofta p�ls helt eller fl�ckvis.\n"
			"\n"
			"GRUNDEGENSKAPER: STY 3, KYL 4\n"
			"F�RDIGHETER: Sl�ss FV 2, Fly FV 3.\n"
			"PANSAR: �\n"
			"VAPEN: Bett (vapenskada 2).");
		break;
	case 20:
		ImGui::TextWrapped(u8"TJ�RNMAL �0\n"
			"I ruing�lar och gamla vattent�kter ruvar tj�rnmalen. Monstret kan ta m�nga skepnader, fr�n platta fj�llbet�ckta fiskar till �l- eller ormliknande bestar med vassa fenor. Tj�rnmalen rycker ofta ner byten som simmar i vattnet eller g�r l�ngs str�nder f�r att sen dr�nka offret genom att h�lla fast det under ytan.\n"
			"\n"
			"GRUNDEGENSKAPER: STY 10, KYL 5\n"
			"F�RDIGHETER: Sl�ss FV 3, Fly FV 4 (under vatten).\n"
			"PANSAR: 5\n"
			"VAPEN: Bett (vapenskada 2)\n"
			"DR�NKA OFFER: Om malen lyckas sl�ss kan den dra ner offret i djupet f�r att dr�nka det. Offret kan ta sig loss genom att klara att fly.");
		break;
	case 21:
		ImGui::TextWrapped(u8"ZONGETINGAR �0\n"
			"Det f�rsta zonvandrare brukar m�rka av dessa odjur �r surrandet, onaturligt dovt och motbjudande. Zongetingarna �r gulsvarta, stora som knytn�var och har h�rda ytterskal. Deras sting injicerar ett kraftigt hallucinogent gift som f�rvirrar offret och g�r det l�ttare f�r insekterna att slita i stycken. Som om det inte vore nog uts�ndrar zongetingar n�r de hamnar i strid ett feromon som direkt lockar till sig fler surrande missfoster.\n"
			"\n"
			"GRUNDEGENSKAPER: STY 5\n"
			"PANSAR: 2\n"
			"VAPEN: Bett (vapenskada 1 och hallucinogent gift, se nedan).\n"
			"SV�RM: Kan endast skadas av eldkastare, explosioner och mutationer.\n"
			"HALLUCINOGENT GIFT: Varje attack som ger 1 eller mer i skada ger offret en po�ng f�rvirring.\n"
			"FEROMONER: Sv�rmens sty �kar ett steg varje runda tills alla offer �r d�da eller har lyckats fly.");
		break;
	case 22:
		ImGui::TextWrapped(u8"ZONIGLAR �0\n"
			"Fingerl�nga kryp som h�ller till i t�ta sn�r eller inne i m�rka ruiner och faller ner �ver sina byten. De slemmiga kr�ken har varken ben eller �gon, bara ett gap fyllt av n�lvassa t�nder. Kommer zoniglarna i kontakt med bar hud hugger de blixtsnabbt och huden domnar. En svag pulserande sm�rta f�rnimms i omr�det och sedan fylls sakta larvkroppen av offrets blod och v�tska.\n" 
			"Ett tiotal kryp som lyckas suga sig fast p� en mutant kommer l�mna ett uttorkat lik efter sig.\n"
			"\n"
			"GRUNDEGENSKAPER: STY 6\n"
			"PANSAR: -\n"
			"VAPEN: Blodsugning. Om iglarnas attack (Sl�ss) lyckas forts�tter offret att ta en po�ng skada per runda tills de tagits bort. Att sk�ra eller rycka bort dem kr�ver att offret, eller en v�n, lyckas Sl�ss. Flera f�rs�k �r till�tna.\n"
			"SV�RM: Zoniglar kan endast skadas av eldkastare, explosioner och mutationer.");
		break;
	case 23:
		ImGui::TextWrapped(u8"ZONT�RNE -1\n"
			"Trakten �r �vervuxen med sn�r och buskar, m�nga t�ckta av grova t�rnen och taggar. Zonbuskarnas rotutv�xter har tryckk�nsliga omr�den. Kliver n�gon p� dem skjuter busken m�ngder av sylvassa taggar i h�g hastighet. Taggarna �r t�ckta med hallucinogent gift. Offret f�rvirras av orimliga syner, desorienteras och riskerar att vandra in bland sn�ren, bara f�r att fastna p� hullingar och t�rnen. Varelser som fastnar blir m�ltid �t de asgnagare och sumpf�glar som lever p� buskarnas offer.\n"
			"\n"
			"EFFEKT: Den som g�r in i omr�det uts�tts f�r en attack med sex grundt�rningar. Rustningar skyddar normalt. En tr�ff ger ingen skada - i st�llet drabbas offret av hallucinogent gift som ger en po�ng f�rvirring f�r varje slagen RADIAK. F�r att ta sig ur sn�ret m�ste offret klara att Fly.\n"
			"Sn�ret angriper en g�ng varje runda.\n"
			"Zont�rnet kan enbart bek�mpas med eld - r�kna dess STY som 10. Vid noll �r sn�ret nedbr�nt och omr�det kan passeras utan risk.");
		break;
	case 24:
		ImGui::TextWrapped(u8"�RGFLYN �0\n"
			"�rgflyn �r udda varelser, stora som en handflata, och p�minner om halvgenomskinliga bl�sor som sv�var fram. Tunna armsl�nga spr�t som gl�nser av sekret h�nger under deras kroppar. Deras r�relser fyller luften med en fr�n stickande doft. Allt spr�ten ber�r f�r br�nnande smet �ver sig. Om de n�r metall vrider spr�ten sig runt f�rem�let och �n mer syra uts�ndras.\n"
			"Varelserna drar sig undan vid anfall. De hungrar bara efter metall, inte levande varelser.\n"
			"\n"
			"EFFEKT: �rgflyna attackerar inte levande offer, utan deras prylar - och bara om de �r gjorda av metall. Sl� f�r en attack mot varje pryl, med sex grundt�rningar. Varje RADIAK s�nker m�lets prylbonus ett steg. Attackerna forts�tter ofta runda efter runda, tills offren lyckas Fly.");
		break;
	}
}
void fenomenBeskrivning(int i)
{
	switch (i + 1)
	{
	case 1:
		ImGui::TextWrapped(u8"ASKVIND +1\n"
			"En kall vind sveper genom Zonen och drar med sig den svarta askan som ligger i drivor mellan ruinerna. Askan skymmer sikten och svider i �gonen och det blir allt sv�rare att andas. Den otrevliga bl�sten varar i n�gra timmar eller �nnu l�ngre. N�r vinden till sist mojnar �r det mesta t�ckt av ett finpudrigt asklager och alla vattenk�llor inom n�gra kilometer �r f�rgiftad s�rja. Vintertid blir sn�n gr�f�rgad och det dr�jer l�nge innan askan f�rsvinner.\n"
			"\n"
			"EFFEKT: Zonfarare som vill forts�tta vandra genom askvinden m�ste klara att K�mpa p�. Den som misslyckas m�ste s�ka skydd tills askvinden �r �ver.");
		break;
	case 2:
		ImGui::TextWrapped(u8"ATOMK�LD �0\n"
			"Stora vita moln sveper in och vinden friskar i rej�lt. Kylan blir sv�r och t�t sn� faller. Pl�tsligt upph�r bl�sten och sn�fallet. En v�gg av vita moln bildar en h�g cylinder ovanf�r omr�det. En bl� himmel skymtar d�r�ver. Stillheten har ett pris: temperaturen faller dramatiskt.\n"
			"Att vistas utomhus �r f�renat med livsfara. Allt levande som inte s�ker skydd d�r. Eldar m�ste matas h�rt eller falna.\n"
			"\n"
			"EFFEKT: Zonfarare som inte b�r tjocka atomvinterst�ll eller forntida skyddsdr�kter uts�tts f�r effekten av k�ld (kapitel 6 i Regelboken). Ett slag i timmen m�ste sl�s tills k�ldkn�ppen �r �ver. Det enda s�ttet att skydda sig �r att t�nda en eld - vilket kr�ver br�nsle, elddon och att n�gon lyckas med ett slag f�r Kyla (h�nderna skakar och br�nslet �r sv�rt att f� fyr p�).");
		ImGui::TextWrapped(u8"\nEXTREMHETTA �0\n"
			"Anv�nd detta fenomen endast p� sommarhalv�ret. Om ni spelar p� vintern, v�lj Atomk�ld i st�llet.\n"
			"\n"
			"EFFEKT: Hettan �r s� sv�r att zonfararna m�ste sl� f�r K�mpa p� efter varje fysiskt anstr�ngande handling. Sl� slaget direkt - det r�knas inte som en handling.\n"
			"Misslyckande inneb�r att offret direkt tar en po�ng stress. �ven den som inte g�r n�got alls m�ste dricka en ranson vatten varje timme i st�llet f�r varje dygn, s� l�nge hettan varar - vilket brukar vara fram till kv�llen.");
		break;
	case 3:
		ImGui::TextWrapped(u8"BLINDG�NGARE +1\n"
			"Hela omr�det �r s�nderskjutet och nerlusat med bomber, minor och granater.\n"
			"M�nga ligger odetonerade, halv dolda i zongruset. Att g� h�r �r f�renat med livsfara.\n"
			"\n"
			"EFFEKT: En person som vill passera omr�det m�ste klara ett slag f�r att Fly. Ett misslyckat slag betyder att en gammal granat detonerar, med spr�ngverkan 10 (se Regelboken).");
		break;
	case 4:
		ImGui::TextWrapped(u8"DAMMTORNADO -1\n"
			"Luften fylls med sand och aska och himlen f�rgas smutsigt gulbrun. En ruin fr�n den gamla v�rlden rister pl�tsligt till och en kilometerh�g tornado sliter den i stycken. Den enorma virveln av sand och damm maler allt i sin v�g till stoft.\n"
			"N�r tornadon dragit f�rbi g�r ett halvkilometer brett sp�r av total �del�ggelse s� l�ngt man kan se. Inget som varit i dess v�g har klarat sig.\n"
			"\n"
			"EFFEKT: Alla som �r i dammtornadons v�g m�ste klara att Fly f�r att hinna ur v�gen i tid. Den som misslyckas uts�tts f�r en attack med hela tolv grundt�rningar. Varje RADIAK ger ett po�ng skada. Rustning skyddar.");
		break;
	case 5:
		ImGui::TextWrapped(u8"ELEKTRISK STORM �0\n"
			"Tunga m�rka moln s�nker sig �ver omr�det. Luften b�rjar smaka metalliskt, h�r st�ller sig p� �nda och spr�da blixtar vandrar l�ngs metallf�rem�l.\n"
			"Pl�tsligt b�rjar blixtar sl� ned �verallt. Om och om igen bombarderas omr�det. �gon t�ras av det skarpa ljuset och d�net fr�n dundrande knallar �r �ronbed�vande. Stormen varar bara n�gra minuter men det k�nns som en evighet.\n"
			"\n"
			"EFFEKT: Alla som uts�tts f�r stormen blir m�l f�r en attack med tre grundt�rningar. Varje RADIAK ger en po�ng stress, rustningar ger inget skydd. F�r den som st�r p� en h�jd eller kl�ttrar i en metallst�llning sl�r du i st�llet sex grundt�rningar.\n"
			"Sl� g�ng p� g�ng tills stormen har bedarrat eller tills zonfararna har lyckats s�ka skydd - det kr�ver att de klarar att Fly.");
		break;
	case 6:
		ImGui::TextWrapped(u8"LJUSPELARE �0\n"
			"Starka ljuspelare syns vid horisonten, och str�lar upp mot den svarta zonhimlen. Var kommer ljusen ifr�n? Och vad har v�ckt dem till liv just nu?\n"
			"\n"
			"EFFEKT: Ingen. Men l�t g�rna SLP ber�ttar skr�nor om ljusen, att n�gon har h�rt att de visar v�gen till Eden, till en annan Ark eller att de �r ett ont omen.");
		break;
	case 7:
		ImGui::TextWrapped(u8"MAGNETF�LT -1\n"
			"Rester av n�gon anl�ggning fr�n den gamla v�rlden skapar i detta omr�de ett magnetiskt kraftf�lt av fluktuerande styrka. Zonfarare kan m�rka av fenomenet genom ett tilltagande surrande och att deras h�r st�ller sig rakt upp. Stannar de i omr�det m�rker de hur alla metallf�rem�l de b�r dras mot en viss punkt.\n"
			"\n"
			"EFFEKT: Zonfararna m�ste v�lja mellan att sl�ppa alla metallf�rem�l de b�r p� eller Ta krafttag f�r att lyckas ta sig ur kraftf�ltet med sakerna i beh�ll. Den som b�r en skrotrustning m�ste Ta krafttag, det g�r inte att ta av sig rustningen snabbt nog. Den som misslyckas f�r sl� igen i n�sta runda, men f�r d� en modifikation -1. Den negativa modifikationen v�xer ett steg f�r varje runda. Kraftf�ltet upph�r pl�tsligt och of�rklarligt n�r du finner det l�mpligt, men �terkommer senare p� samma plats.");
		break;
	case 8:
		ImGui::TextWrapped(u8"MIS�RVIND �0\n"
			"En vind fylld av pest, f�rruttnelse och r�ta sveper in �ver Zonen. Luften fylls av damm och otrevliga od�rer och blir tung att andas. I sp�ren f�ljer smittor, krubb ruttnar och vatten blir odrickbart.\n"
			"Den el�ndiga vinden h�ller i sig i flera timmar innan den avtar.\n"
			"\n"
			"EFFEKT: Vinden ger en r�tpo�ng direkt. Dessutom blir allt krubb och vatten besudlat. Det g�r fortfarande att �ta/dricka, men ger en r�tpo�ng varje g�ng man g�r s�.");
		break;
	case 9:
		ImGui::TextWrapped(u8"NATTSKEN -1\n"
			"Flammande ljussken i sjukliga f�rger sveper �ver nattskyn. Artefakter som drivs p� batterier och fornkraft fungerar erratiskt eller inte alls. En del ser underg�ngsomen, andra hopp om en ny gryning. �ter andra tycker bara f�rgerna �r fina eller g�mmer sig i r�dsla. Zonskenet varar tills det ljusnar.\n"
			"\n"
			"EFFEKT: Alla elektriska artefakter (Str�lkastare, Kassettradio, Videokamera) slutar fungera s� l�nge nattskenet p�g�r.\n"
			"KOMMENTAR: Om du sl�r fram detta fenomen under dagtid, l�t det intr�ffa f�ljande kv�ll i st�llet.");
		break;
	case 10:
		ImGui::TextWrapped(u8"OBELISK �0\n"
			"Mitt i Zonen st�r en flera meter h�g, svart obelisk. Den verkar vara tillverkad i n�gon sorts mycket t�lig metall. Den finns inga synliga �ppningar eller luckor, och obelisken verkar helt ober�rd av tidens tand. Obelisken ger ett fr�mmande intryck och har m�rkliga psykiska effekter p� personer i n�rheten, psykedeliska hallucinationer eller v�sande r�ster i huvudet.\n"
			"\n"
			"EFFEKT: Varje runda n�ra obelisken m�ste offret sl� f�r att K�nna Zonen. Att misslyckas ger en po�ng f�rvirring.");
		break;
	case 11:
		ImGui::TextWrapped(u8"R�TH�RD -1\n"
			"Kusten verkar klar. Ruinen �r tom p� fiender, och utforskningen kan b�rja obehindrat.\n"
			"Men den osynliga R�tan �r stark h�r och kan d�da of�rsiktiga zonfarare.\n"
			"\n"
			"EFFEKT: Ge RP chansen att ana or�d genom att beskriva n�got avvikande i milj�n; till exempel en samling kadaver, ett r�r som l�cker gr�n s�rja, eller en plats d�r m�rkret �r onaturligt kompakt och p�tr�ngande. Den som trots allt vandrar in i h�rden tar en r�tpo�ng per minut (kapitel 8 i Regelboken).\n"
			"Om du vill st�lla spelarna inf�r ett knivigt val kan den enda v�gen fram�t g� genom r�th�rden. Om RP anar or�d kan de Speja mot omr�det f�r att f� veta att det �r en r�th�rd.");
		break;
	case 12:
		ImGui::TextWrapped(u8"SLUKH�L �0\n"
			"Den s�ndervittrande ruinen ger vika och en eller flera personer dras med i ett moln av grus och zondamm.\n"
			"\n"
			"EFFEKT: Alla som dras med i raset m�ste klara ett slag f�r att Fly f�r att undvika att ta en po�ng skada (rustning skyddar).\n"
			"Men det �r bara b�rjan p� problemet. Offren f�r raset kan ha hamnat i n�gon forntida bunkar eller tunnel, fylld av monster eller r�ta. Att kl�ttra upp kan ocks� visa sig sv�rt.");
		break;
	case 13:
		ImGui::TextWrapped(u8"SPLITTERVIND �0\n"
			"En h�rd bl�st b�rjar vina. Luften fylls av virvlande korn av glas och metallsplitter som river upp oskyddad hud.\n"
			"\n"
			"EFFEKT: Alla som befinner sig ute i splittervinden uts�tts f�r en attack med sex (eller �nnu fler, om vinden �r s�rskilt h�rd) grundt�rningar. Varje RADIAK ger en po�ng skada. Att hitta skydd kr�ver normalt att man lyckas Fly, alternativt anv�nder en mutation med samma effekt.");
		break;
	case 14:
		ImGui::TextWrapped(u8"SP�KSKEN �0\n"
			"Ett blekt gr�nt ljussken spelar mellan ruinrester eller tr�dstammar. Om n�gon n�rmar sig drar skenen sig undan. Ingen f�rklaring st�r att finna men sp�kljusen tycks f�rflytta sig om en betraktare g�r det. Sedan f�rsvinner de lika hastigt i intet som de d�k upp.\n"
			"\n"
			"EFFEKT: Ingen - men l�t g�rna spelarna tro annorlunda. Det finns inget rollpersonerna kan g�ra som p�verkar sp�kskenet.");
		break;
	case 15:
		ImGui::TextWrapped(u8"SYNVILLA -1\n"
			"Omr�det runt zonfararna tycks pl�tsligt hacka till. Det �r som om allt skevar till ett �gonblick. Omr�det byter skepnad och ser i en br�kdels sekund helt annorlunda ut: v�lordnat, st�dat, vackert. Zonfararna har vandrat in i ett hologram, resterna av ett urtida f�rsvarsverk. Det �r ett avancerat kamouflage som i sig �r harml�st men skapar en risk att rp g�r in i saker (ruiner, tr�d, betongblock, vrak) som f�rvr�ngts till att likna n�got annat.\n"
			"Omr�det �r cirka 20x20 meter stort.\n"
			"\n"
			"EFFEKT: Ingen - men effekten kan vara f�rvirrande.");
		break;
	case 16:
		ImGui::TextWrapped(u8"SYRAREGN +1\n"
			"Tunga droppar b�rjar pl�tsligt falla fr�n zonhimlen. Men detta �r inget vanligt regn. Dropparna fr�ser av syra n�r de sl�r ner, och den som befinner sig i syraskuren g�r b�st i att snabbt s�ka skydd. Efter regnet �r ruiner, betongblock, tr�d, blad och vrak pepprade med syrafr�tta h�l eller har f�tt djupa r�nnilar av strilande syravatten graverat. P�lar av fr�tande s�rja och lera t�cker marken. Vintertid blir regnet till t�tt fallande syrasn� som bildar en tjock fr�tande sn�gegga. Sn�n dunstar inte utan f�rsvagas �ver flera dygns tid.\n"
			"\n"
			"EFFEKT: Den som �r ute i syraregnet l�per stor risk att ta skada. Sl� ett antal grundt�rningar beroende p� regnets intensitet - 6 �r normalt, men allt fr�n 4 till 10 �r m�jligt. Varje ger offret en po�ng skada. Rustning skyddar p� vanligt s�tt (kapitel 6 i Regelboken).\n"
			"Offren forts�tter ta skada av regnet p� samma s�tt varje runda tills de lyckas s�ka skydd genom att Fly.");
		break;
	case 17:
		ImGui::TextWrapped(u8"TR�GHETSF�LT -2\n"
			"Zonfararna ser ett monster eller humanoid som tycks helt frusen mitt i en r�relse, totalt or�rlig. Om n�gon n�rmar sig objektet kan andra se hur hans r�relser tycks bromsa in till slow motion. I omr�det finns ett m�rkligt kraftf�lt som f�r sj�lva tiden att bromsa in. Den som g�r in i f�ltet m�rker bara att omgivningen b�rjar r�ra sig snabbare, och effekten tilltar ju l�ngre in personen g�r. Det g�r att ropa till en person i f�ltet, men han uppfattar r�ster utifr�n som h�gfrekventa och pipiga. Hans egen r�st l�ter m�rk och l�ngsam f�r personer utanf�r f�ltet.\n"
			"\n"
			"EFFEKT: Att befinna sig i f�ltets ytterkant medf�r ingen st�rre risk, men den som n�rmar sig dess centrum kommer att bli f�ngen i f�ltet f�r evigt. L�t RP som g�r in i f�ltet (eller bara ser effekten av det p� n�gon annan) sl� f�r att K�nna Zonen. Om slaget lyckas ska du f�rklara risken f�r spelarna.");
		break;
	case 18:
		ImGui::TextWrapped(u8"VAKUUM -2\n"
			"Zonfararna passerar ett omr�de som �r helt tomt p� luft. Det finns ingen uppenbar f�rklaring till varf�r luften inte fyller omr�det - inga tydliga markeringar eller gr�nser syns. Det f�rsta zonfararna m�rker �r att de inte kan andas eller prata.\n"
			"\n"
			"EFFEKT: Alla m�ste sl� f�r att fly. Den som misslyckas f�r panik och blir kvar i vakuumet, vilket ger en po�ng skada varje runda. Rustning skyddar inte. Ett slag per runda �r till�tet f�r att ta sig ut ur vakuumet.\n"
			"Den som tagit sig ut kan hj�lpa (sid 54 i Regelboken) ut en v�n som blivit kvar.");
		break;
	case 19:
		ImGui::TextWrapped(u8"ZONG�LAR �0\n"
			"En ruintrakt d�r varm och tjockflytande lera t�cker alla �ppna ytor. Solid mark finns under geggan och en normall�ng mutant sjunker ned till kn�et. H�r och var finns meterdjupa kratrar, skapade av omr�dets zonfenomen. Under ytan d�ljs l�mska k�llor med hett vatten. Om n�gon r�r sig i n�rheten eller rakt �ver dem kommer de brisera och en kaskad av sk�llande vatten sprutar upp med oerh�rd kraft. Vattenstr�len n�r ett tiotal meter i h�jd och dr�nker allt inom n�gra meter. En person som tr�ffas riskerar att f� sin hud bortsk�llad.\n"
			"\n"
			"EFFEKT: Den som Spejar framg�ngsrikt mot g�larna noterar att det pyser och bubblar p� vissa platser. F�r att r�ra sig genom omr�det och undvika dessa platser kr�vs att man klarar ett slag f�r K�nna Zonen. Ett misslyckat slag betyder att offret blir m�ltavla f�r en sk�llhet kaskad. Samma sak g�ller den som g�r in i omr�det ovetande. Sl� ett antal grundt�rningar beroende p� kaskadens intensitet, sex �r normalt. Sl�r du en eller fler RADIAK tr�ffas offret, och tar skada lika med antalet RADIAK. Den som tar en eller flera po�ng i skada tar ocks� en r�tpo�ng. Den enda f�rdelen med denna plats �r att den alla i n�rheten skyddas mot k�ld.");
		break;
	case 20:
		ImGui::TextWrapped(u8"ZONSMOG +1\n"
			"Str�k av smog b�rjar virvla upp fr�n marken. F�rst i kn�h�jd men sedan stiger dimman allt mer. �gon b�rjar t�ras och det br�nner i n�sa och mun samt sticker otrevligt �ver huden. Man riskerar ingen direkt skada men har mycket sv�rt att se och lukt- och smaksinne �r helt borta i flera timmar fram�t. Bar hud kommer vara r�d och sm�tt svullen n�gon dag efter. Smogmolnet ligger t�tt resten av dagen �ver omr�det. D�rtill har zonbestar med f�rm�ga att jaga obehindrat i smogen l�rt sig utnyttja dimmans effekt. Tar sig zonfararna till h�gre h�jd (ruin, tr�dtopp, kulle) undkommer de smogen.\n"
			"\n"
			"EFFEKT: Det g�r inte att Skjuta p� kort eller l�ngt h�ll i smoggen. Alla som hamnar i smoggen tar ocks� en r�tpo�ng.\n"
			"V�lj eller sl� fram ett zonmonster som jagar i smoggen.");
		break;
	case 21:
		ImGui::TextWrapped(u8"Luften blir allt varmare och p� en del platser v�ller �nga fram ur sprickor i marken. Sm� p�lar med bubblande och illaluktande vatten finns ocks�. Vattnet �r kokande hett men h�r och var blandas det med regnp�lar och b�ckar och tycks behagligt varmt.\n"
			"\n"
			"EFFEKT: Ett ur�ldrigt kraftverk l�ngt under ytan �r fortfarande aktivt. Det l�cker r�ta och vattnet �r smittat. Den som vadar genom vattnet tar en r�tpo�ng.\n"
			"F�rdelen �r att k�llan skyddar mot k�ld (se Regelboken) s� l�nge man h�ller sig i n�rheten.");
		break;
	}
}