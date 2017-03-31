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
// Hur många hot och artefakter
int randHot = 0;
int randArtefakt = 0;
ImVec2 ButtonSize(100, 30);




// sätter Rötnivå
std::string rotStr[]{
	u8"Röt-oas. RP tar inga rötpoäng."
	, u8"Typisk zonsektor. RP tar en rötpoäng varje dygn."
	, u8"Extra rötdrabbad sektor. En rötpoäng per timme."
};
static int rotValue = 0;

// sätter hotnivå
std::string hotNiva[]{
	u8"1. Zonens ytterområden"
	, u8"2. Zonens ytterområden"
	, u8"3. Zonens ytterområden"
	, u8"4. Zonens ytterområden"
	, u8"5. Zonens centrala delar"
	, u8"6. Zonens centrala delar"
	, u8"7. Zonens centrala delar"
	, u8"8. Zonens centrala delar"
	, u8"9 + Särskilt farliga sektorer"
};
static int hotValue = 0;

// hot i zonen
std::string hotIZonenStr[]{
	u8"Humanoider"
	, u8"Monster"
	, u8"Fenomen"
};
static int hotIZonenValue = -1;

// sätter up strängklasserna
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
		"\nResten av kapitlet beskriver hur du steg för steg skapar en sektor i Zonen."
		"Du kan välja fritt ur tabellerna eller rulla T66."
		" Följ dessa steg när RP går in i en ny sektor :"
		"\n1. Bestäm allmän sektormiljö."
		"\n2. Bestäm en dominerande ruin i sektorn."
		"\n3. Avgör rötnivån."
		"\n4. Avgör hotnivå och slå hotslaget."
		"\n5. Om det finns ett hot i sektorn, avgör vilket."
		"\n6. Om det finns en artefakt i sektorn, avgör vilken."
		"\n7. Om det finns en zonstrykare i gruppen, låt honom slå för att leda vägen (se kapitel 3 i Regelboken)."
		"\n8. Om du vill, slå eller välj en detalj att använda i din beskrivning.");

	if (ImGui::Button(u8"Slumpa alla!", ButtonSize))
	{
		slumpa_alla = true;
		skapa_sektor = false;
		sektor_miljo = true;

	}; ImGui::SameLine();
	if (ImGui::Button(u8"Fortsätt", ButtonSize))
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
///////////////////////////////////	 MILJÖ	/////////////////////////////
void sektorMiljo()
{
	ImGui::TextWrapped(u8"SEKTORMILJÖ\n"
		"Börja med att etablera den allmänna miljön\n"
		"i sektorn.Slå T66 eller välj : ");

	int size = ARRAY_SIZE(miljo.text);
	for (int i = 0; i < size; i++)
		ImGui::RadioButton(&miljo.text[i][0], &miljo.value, i);

	ImGui::TextWrapped(u8"\nVälj en av ovan eller slumpa med hjälp av knappen."
		"\nTryck på fortsätt för att gå vidare");
	// SLUMPA KNAPP
	


	if (ImGui::Button(u8"Slumpa värde", ButtonSize) || slumpa_alla)
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

	// NÄSTA KNAPP
	if (ImGui::Button(u8"Fortsätt", ButtonSize) || slumpa_alla)
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
	ImGui::Text(u8"Vald miljö är: ");
	ImGui::SameLine(); ImGui::Text(&miljo.text[miljo.value][0]);
	// gå tillbaka till början
	if (ImGui::Button(u8"Gå tillbaka", ButtonSize))
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
		"Om sektormiljön är delvis raserade ruiner, välbevarade ruiner eller industrilandskap "
		"är nästa steg att avgöra vilken ruin som dominerar sektorn.Det finns "
		"förstås mer än en ruin i varje sektor, men	den utvalda ruinen är den som väcker "
		"RP : s intresse.");
	if (ruin_standard)
	{
		int size = ARRAY_SIZE(stdRuin.text);

		for (int i = 0; i < size; i++)
			ImGui::RadioButton(&stdRuin.text[i][0], &stdRuin.value, i);

		ImGui::TextWrapped(u8"\nVälj en av ovan eller slumpa med hjälp av knappen."
			"\nTryck på fortsätt för att gå vidare");
		// SLUMPA KNAPP
		if (ImGui::Button(u8"Slumpa värde", ButtonSize) || slumpa_alla)
		{
			stdRuin.value = rand() % size;
		}; ImGui::SameLine();
		// NÄSTA KNAPP
		if (ImGui::Button(u8"Fortsätt", ButtonSize) || slumpa_alla)
		{
			sektor_ruin = false;
			sektor_rota = true;
			//rot();
		}
		ImGui::Text(u8"Vald ruin är: ");
		ImGui::SameLine(); ImGui::Text(&stdRuin.text[stdRuin.value][0]);
		// gå tillbaka till början
		if (ImGui::Button(u8"Gå tillbaka", ButtonSize))
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
		if (ImGui::Button(u8"Slumpa värde", ButtonSize) || slumpa_alla)
		{
			int temp = diceRoll(2);
			std::cout << temp << std::endl;
			for (int i = 0; i < size; i++)
			{
				if (indRuin.range[0][i] <= temp && indRuin.range[1][i] >= temp)
					indRuin.value = i;
			}

		}; ImGui::SameLine();
		// NÄSTA KNAPP
		if (ImGui::Button(u8"Fortsätt", ButtonSize) || slumpa_alla)
		{
			sektor_ruin = false;
			sektor_rota = true;
			//rot();
		}
		ImGui::Text(u8"Vald ruin är: ");
		ImGui::SameLine(); ImGui::Text(&indRuin.text[indRuin.value][0]);
		// gå tillbaka till början
		if (ImGui::Button(u8"Gå tillbaka", ButtonSize))
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
///////////////////////////////////	 RÖTA	 /////////////////////////////
void rot()
{
	ImGui::TextWrapped(u8"RÖTNIVÅ\n"
		"Välj eller slå fram rötnivån i sektorn, en siffra från 0 till 3. 1 eller 2 är det vanliga "
		"i Zonen.Läs mer om zonröta i kapitel 8 i Regelboken. "
		"Om du har en dold sl - karta kan du rita in på förhand vilken rötnivå Zonens sektorer har.");

	int size = ARRAY_SIZE(rotStr);

	for (int i = 0; i < size; i++)
		ImGui::RadioButton(&rotStr[i][0], &rotValue, i);

	// SLUMPA KNAPP
	if (ImGui::Button(u8"Slumpa värde", ButtonSize) || slumpa_alla)
	{
			int randomValue = rand() % 6 + 1;
			if (randomValue == 1)
				rotValue = 0;
			else if (randomValue == 6)
				rotValue = 2;
			else
				rotValue = 1;
	}; ImGui::SameLine();
	// NÄSTA KNAPP
	if (ImGui::Button(u8"Fortsätt", ButtonSize) || slumpa_alla)
	{
		sektor_rota = false;
		sektor_hot = true;
		//hotMeny();
	}

	ImGui::Text(u8"Vald röta är: ");
	ImGui::SameLine(); ImGui::Text(&rotStr[rotValue][0]);
	// gå tillbaka till början
	if (ImGui::Button(u8"Gå tillbaka", ButtonSize))
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
	ImGui::TextWrapped(u8"Nästa steg är att bestämma hotnivån.\n"
		"Hotnivån är ett värde från noll och uppåt. "
		"Ett högt värde betyder en farligare sektor, men också större chans att "
		"det finns orörda artefakter där.\n"
		"Om sektormiljön (ovan) är bosättning har sektorn "
		"ingen hotnivå.Gå direkt till steg 5 "
		"om bosättningar, nedan.");

	int size = ARRAY_SIZE(hotNiva);
	for (int i = 0; i < size; i++)
		ImGui::RadioButton(&hotNiva[i][0], &hotValue, i);

	// SLUMPA KNAPP
	if (ImGui::Button(u8"Slumpa värde", ButtonSize) || slumpa_alla)
	{
		hotValue = rand() % size;

	}; ImGui::SameLine();
	// NÄSTA KNAPP
	if (ImGui::Button(u8"Fortsätt", ButtonSize) || slumpa_alla)
	{
		sektor_hot = false;
		sektor_random_hot = true;
		//randomHot();
	}
	ImGui::Text(u8"Vald hotnivå är: ");
	ImGui::SameLine(); ImGui::Text(&hotNiva[hotValue][0]);
	// gå tillbaka till början
	if (ImGui::Button(u8"Gå tillbaka", ButtonSize))
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
	ImGui::TextWrapped(u8"När rollpersonerna går in i sektorn rullar du ett antal grundtärningar lika "
		"med hotnivån.Varje RADIAK betyder att det finns en artefakt i sektorn.Slår du minst "
		"en BIOHAZARD betyder det att det finns ett hot i sektorn, och ju fler BIOHAZARD du slår, desto "
		"värre är hotet.Se steg 5 nedan.\n"
		"Om sektormiljön är skog eller totalt raserade ruiner(ovan) finns det inga artefakter i sektorn, oavsett vad tärningarna visar. "
		"RADIAK har alltså ingen betydelse.\n"
		"Nattetid är det farligt att röra sig i Zonen.Räkna hotnivån som tre steg högre än normalt.");

	static int e = 0;

	ImGui::RadioButton("Dagtid", &e, 0);
	ImGui::RadioButton("Nattetid", &e, 1);

	int tempHot = hotValue + (e * 3) + 1;
	

	// SLUMPA KNAPP
	if (ImGui::Button(u8"Slumpa värde", ButtonSize) || slumpa_alla)
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
	// NÄSTA KNAPP
	if (ImGui::Button(u8"Fortsätt", ButtonSize) || slumpa_alla)
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
	ImGui::Text(u8"Hotnivån är: %d", tempHot);
	if (!hot)
		ImGui::Text(u8"Inga hot påträffas på grund av zonvalen");
	else
		ImGui::Text(u8"Styrka på hotet : %d", randHot);
	if (!artefakt)
		ImGui::Text(u8"Inga artefakter påträffas på grund av zonvalen");
	else
		ImGui::Text(u8"Antal artefakter är: %d", randArtefakt);

	// gå tillbaka till början
	if (ImGui::Button(u8"Gå tillbaka", ButtonSize))
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
		"Humanoider, monster och fenomen.Läs mer om dem i kapitel 5. Slå en T6 eller välj : ");

	for (int i = 0; i < ARRAY_SIZE(hotIZonenStr); i++)
		ImGui::RadioButton(&hotIZonenStr[i][0], &hotIZonenValue, i);

	// SLUMPA KNAPP
	if (ImGui::Button(u8"Slumpa värde", ButtonSize) || slumpa_alla)
	{
		int temp = rand() % 6 + 1;
		if (temp == 1 || temp == 2)
			hotIZonenValue = 0;
		else if (temp == 6)
			hotIZonenValue = 2;
		else
			hotIZonenValue = 1;

	}; ImGui::SameLine();
	// Räkna ut vilken typ av hot det gäller
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
	// NÄSTA KNAPP
	if (ImGui::Button(u8"Fortsätt", ButtonSize) || slumpa_alla)
	{

		sektor_hot_i_zonen = false;
		//hotIZonen();

	}
	// gå tillbaka till början
	if (ImGui::Button(u8"Gå tillbaka", ButtonSize))
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
	ImGui::TextWrapped(u8"humanoider är individer eller grupper av tänkande varelser.Slå T66 eller välj : ");

	int size = ARRAY_SIZE(humanoider.text);
	for (int i = 0; i < size; i++)
		ImGui::RadioButton(&humanoider.text[i][0], &humanoider.value, i);

	// SLUMPA KNAPP
	if (ImGui::Button(u8"Slumpa värde", ButtonSize) || slumpa_alla)
	{
		int temp = diceRoll(2);
		std::cout << temp << std::endl;
		for (int i = 0; i < size; i++)
		{
			if (humanoider.range[0][i] <= temp && humanoider.range[1][i] >= temp)
				humanoider.value = i;
		}

	}; ImGui::SameLine();

	// NÄSTA KNAPP
	if (ImGui::Button(u8"Fortsätt", ButtonSize) || slumpa_alla)
	{
		hot = false;
		sammanstallning = true;
	}
	ImGui::Text(u8"Vald humanoid är: ");
	ImGui::SameLine(); ImGui::Text(&humanoider.text[humanoider.value][0]);
	// gå tillbaka till början
	if (ImGui::Button(u8"Gå tillbaka", ButtonSize))
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
	ImGui::TextWrapped(u8"MONSTER är sinnesslöa bestar som härjar i Zonens vidder.Vissa är muterade "
		"versioner av nutidens djur, andra något helt främmande.Slå T66 eller välj : ");

	int size = ARRAY_SIZE(monster.text);
	for (int i = 0; i < size; i++)
		ImGui::RadioButton(&monster.text[i][0], &monster.value, i);


	// SLUMPA KNAPP
	if (ImGui::Button(u8"Slumpa värde", ButtonSize) || slumpa_alla)
	{
		int temp = diceRoll(2);
		std::cout << temp << std::endl;
		for (int i = 0; i < size; i++)
		{
			if (monster.range[0][i] <= temp && monster.range[1][i] >= temp)
				monster.value = i;
		}
		

	}; ImGui::SameLine();
	// NÄSTA KNAPP
	if (ImGui::Button(u8"Fortsätt", ButtonSize) || slumpa_alla)
	{
		hot = false;
		sammanstallning = true;
	}
	ImGui::Text(u8"Vald humanoid är: ");
	ImGui::SameLine(); ImGui::Text(&monster.text[monster.value][0]);

	// gå tillbaka till början
	if (ImGui::Button(u8"Gå tillbaka", ButtonSize))
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
	ImGui::TextWrapped(u8"fenomen är annat än levande varelser. Slå T66 eller välj : ");

	int size = ARRAY_SIZE(fenomen.text);
	for (int i = 0; i < size; i++)
		ImGui::RadioButton(&fenomen.text[i][0], &fenomen.value, i);

	// SLUMPA KNAPP
	if (ImGui::Button(u8"Slumpa värde", ButtonSize) || slumpa_alla)
	{
		int temp = diceRoll(2);
		std::cout << temp << std::endl;
		for (int i = 0; i < size; i++)
		{
			if (fenomen.range[0][i] <= temp && fenomen.range[1][i] >= temp)
				fenomen.value = i;
		}

	}; ImGui::SameLine();
	// NÄSTA KNAPP
	if (ImGui::Button(u8"Fortsätt", ButtonSize) || slumpa_alla)
	{
		hot = false;
		sammanstallning = true;
	}
	ImGui::Text(u8"Vald humanoid är: ");
	ImGui::SameLine(); ImGui::Text(&fenomen.text[fenomen.value][0]);

	// gå tillbaka till början
	if (ImGui::Button(u8"Gå tillbaka", ButtonSize))
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
	ImGui::Text(u8"Detta är en sammanställning av alla valen : ");
	ImGui::Text("");
	if (randHot == 0)
	{
		ImGui::TextColored(YELLOW, u8"Hotnivån är : %d", hotValue);
		ImGui::SameLine(); ImGui::TextColored(YELLOW, u8", men inga hot påträffades!");
	}
	if(miljo.value != 12)
	{
		ImGui::TextColored(YELLOW, u8"RÖTNIVÅ : %d", rotValue);
		ImGui::SameLine(); ImGui::TextColored(YELLOW, &rotStr[rotValue][0]);
	}
	
	
	ImGui::Text("");
	ImGui::TextColored(GREEN, u8"MILJÖ : ");
	ImGui::SameLine(); ImGui::TextColored(GREEN, &miljo.text[miljo.value][0]);
	


	if (ruin_standard)
	{
		ImGui::TextColored(GREEN, u8"Den dominerande ruinen är : ");
		ImGui::SameLine(); ImGui::TextColored(GREEN, &stdRuin.text[stdRuin.value][0]);
		if (!artefakt)
			ImGui::TextColored(GREEN, u8"Inga artefakter hittade på grund av zonvalen");
		else
			ImGui::TextColored(GREEN, u8"Antal artefakter är: %d", randArtefakt);
		stdRuinBeskrivning(stdRuin.value);
	}
	else if (ruin_industri)
	{
		ImGui::TextColored(GREEN, u8"Den dominerande ruinen är : ");
		ImGui::SameLine(); ImGui::TextColored(GREEN, &indRuin.text[indRuin.value][0]);
		if (!artefakt)
			ImGui::TextColored(GREEN, u8"Inga artefakter hittade på grund av zonvalen");
		else
			ImGui::TextColored(GREEN, u8"Antal artefakter är: %d", randArtefakt);
		indRuinBeskrivning(indRuin.value);
	}
	if (is_humanoider && randHot > 0)
	{
		ImGui::TextColored(PURPLE, u8"Hotet i zonen är humanoider : ");
		ImGui::SameLine(); ImGui::TextColored(PURPLE, &humanoider.text[humanoider.value][0]);
		ImGui::TextColored(PURPLE, u8"Styrka på hotet : %d", randHot);
		humanoidBeskrivning(humanoider.value);///////////////////////////////////////
	}
	else if (is_monster && randHot > 0)
	{
		ImGui::TextColored(PURPLE, u8"Hotet i zonen är monster : ");
		ImGui::SameLine(); ImGui::TextColored(PURPLE, &monster.text[monster.value][0]);
		ImGui::TextColored(PURPLE, u8"Styrka på hotet : %d", randHot);
		monsterBeskrivning(monster.value);///////////////////////////////////////
	}
	else if (is_fenomen && randHot > 0)
	{
		ImGui::TextColored(PURPLE, u8"Hotet i zonen är fenomen : ");
		ImGui::SameLine(); ImGui::TextColored(PURPLE, &fenomen.text[fenomen.value][0]);
		ImGui::TextColored(PURPLE, u8"Styrka på hotet : %d", randHot);
		fenomenBeskrivning(fenomen.value);
	}

	ImGui::Text("");

	if (ImGui::Button(u8"Gå tillbaka", ButtonSize))
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
	// Hur många hot och artefakter
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
		ImGui::TextWrapped(u8"AFFÄRSGALLERIA\n"
			"En stad i staden öppnar sig, en enorm sal\n"
			"med forntida affärer och restauranger i\n"
			"flera våningar längs väggarna. Allt är\n"
			"illa härjat av lågorna och täckt av tjocka\n"
			"lager sot, men kanske gömmer sig något\n"
			"värdefullt under askan ? Här och var står\n"
			"svarta figurer i förbränd plast i märkliga\n"
			"ställningar, som om de talade om den\n"
			"gamla världens viktiga kunskaper.");
		break;
	case 2:
		ImGui::TextWrapped(u8"BADHUS\n"
			"Inne i denna stora ruin finns stora bassänger\n"
			"fyllda av gyttja och slemmiga\n"
			"gröna alger. Motbjudande och osunda\n"
			"ångor möter besökare som försöker ta\n"
			"sig in. Det är svårt att tänka sig att fornfolket\n"
			"frivilligt gick hit och doppade sig\n"
			"i bassängerna.");
		break;
	case 3:
		ImGui::TextWrapped(u8"BENSINSTATION\n"
			"Under ett öppet tak vid en låg byggnad\n"
			"står några motorkärror och rostar. Under\n"
			"taket finns maskiner med slangar och\n"
			"kranar. Den zonkloke vet att fornfolket\n"
			"reparerade sina fordon och fyllde dem\n"
			"med bränsle på platser som denna.");
		break;
	case 4:
		ImGui::TextWrapped(u8"BIOGRAF\n"
			"Intill en stor entré i en husfasad hänger\n"
			"en mängd gulnade planscher i ramar,\n"
			"alla visar dramatiska motiv från tiden\n"
			"före undergången. Här inne tittade den\n"
			"gamla världens människor på rörliga bilder\n"
			"på en duk.");
		break;
	case 5:
		ImGui::TextWrapped(u8"VILLAOMRÅDE\n"
			"En smal gata med små, identiska hus i\n"
			"jämna rader på varje sida.Trädgårdarnas\n"
			"gräs och buskar har vittrat bort, allt\n"
			"är täckt av damm och aska. Inne i husen\n"
			"ligger multnande kläder och prylar kvar,\n"
			"nvånarna verkar ha flytt i en hast. Fordonsvrak\n"
			"och mängder av spruckna och\n"
			"förstörda lämningar från dem som en\n"
			"ång bott i området finns överallt. Förfallet\n"
			"r markant och många av husen har\n"
			"aft svårt att motstå tidens tand.");
		break;
	case 6:
		ImGui::TextWrapped(u8"BUSSTATION\n"
			"Bland flagnade skyltar med oläsliga siffror\n"
			"och bokstäver på står flera avlånga\n"
			"fornkärror och rostar. Vissa är röda, \n"
			"andra är blå, och flera har stora dragspel\n"
			"på mitten. Ännu ett av den gamla världens\n"
			"mysterier.");
		break;
	case 7:
		ImGui::TextWrapped(u8"FLYGPLANSVRAK\n"
			"En enorm metalltub ligger knäckt och\n"
			"sönderbruten på marken. Den är svedd\n"
			"av eld och inuti syns mängder av forntida\n"
			"fåtöljer, svartbrända av lågorna. I\n"
			"ett avlångt fält som sträcker sig bort\n"
			"från tuben är marken sargad och alla\n"
			"hus ödelagda, som om tuben störtat från\n"
			"himlen i fruktansvärd hastighet.");
		break;
	case 8:
		ImGui::TextWrapped(u8"FÖRVILDAD PARK\n"
			"Spretiga träd och buskar växer vilt och\n"
			"ohejdat här. Bänkar, gångvägar och\n"
			"fontäner göms under tjockt sly och\n"
			"gräsmattorna har förvandlats till ett\n"
			"vattensjukt träsk. Det går många rykten\n"
			"om farliga varelser som döljer sig i\n"
			"Zonens forna parker.");
		break;
	case 9:
		ImGui::TextWrapped(u8"HÖGHUSRUIN\n"
			"Ett högt torn reser sig mot Zonens guldisiga\n"
			"himmel. Betongen är sargad och\n"
			"sprucken, och alla fönster har för länge\n"
			"sedan fallit ur sina ramar. Zonvinden viner\n"
			"genom våningsplanen, som gapar öppna\n"
			"och mörka. Varje våning är fylld av små\n"
			"och likformade bostadsenheter eller kontor.\n"
			"Bara de översta våningarna skiljer sig\n"
			"med markant större och lyxigare bostäder.");
		break;
	case 10:
		ImGui::TextWrapped(u8"JAKTAFFÄR\n"
			"Bland välta hyllor går det att hitta både\n"
			"fiskespön, krokar och linor. En zonstrykare\n"
			"kan ta reda på i vilka av Zonens vatten\n"
			"man vågar fiska. På väggarna sitter\n"
			"halvt sönderrivna bilder på gevär med\n"
			"kikarsikten. I ett hörn står ett tungt låst\n"
			"metallskåp högt som en mutant, som\n"
			"kan innehålla något värdefullt.");
		break;
	case 11:
		ImGui::TextWrapped(u8"KONTORSKOMPLEX\n"
			"I denna mörka ruin finns sal efter sal\n"
			"med oräkneliga mängder identiska\n"
			"bord i varsitt litet bås. På golvet ligger\n"
			"gulnade papper utspridda. Före undergången\n"
			"pågick säkert något viktigt\n"
			"arbete här. Nu virvlar zondammet in\n"
			"genom de krossade fönstren.");
		break;
	case 12:
		ImGui::TextWrapped(u8"KRATER\n"
			"Ett gigantiskt hål gapar i zonmarken, \n"
			"minst 50 meter tvärs över. Här måste\n"
			"något fornvapen av horribel styrka slagit\n"
			"ned och förintat allt i närheten. I botten\n"
			"av kratern har en liten sjö bildats av\n"
			"grönaktigt zonsjukt vatten.");
		break;
	case 13:
		ImGui::TextWrapped(u8"KRÅKSLOTT\n"
			"Detta mörka och fallfärdiga hus var\n"
			"gammalt redan före undergången. Det\n"
			"är byggt i trä och sten och har på något\n"
			"sätt överlevt katastrofens härjningar.");
		break;
	case 14:
		ImGui::TextWrapped(u8"KYRKA\n"
			"Det uråldriga stentornet på denna byggnad\n"
			"når högt i skyn, dess vassa spets\n"
			"går förlorad i Zonens dis. Inne i byggnaden\n"
			"står rader av murkna träbänkar\n"
			"vända mot en mansfigur som hänger på\n"
			"ett kors. Hit gick fornfolket för att prata\n"
			"med sin hövding i himlen. Nu är det tyst.\n"
			"Om det funnits en gud har den för länge\n"
			"sedan övergivit denna plats.");
		break;
	case 15:
		ImGui::TextWrapped(u8"LEKPLATS\n"
			"Bildäck hänger i rostiga kättingar från\n"
			"en ställning.En trälåda med sand i har\n"
			"blivit en osund lerpöl. Syraregn har för\n"
			"länge sedan frätt sönder rutschkanan.\n"
			"Här lekte den gamla världens människor\n"
			"med sina barn. Några spruckna plastleksaker\n"
			"har överlevt tidens tand och\n"
			"sticker upp ur gruset.");
		break;
	case 16:
		ImGui::TextWrapped(u8"MILJONPROGRAM\n"
			"Flera likformade byggnader i grå och\n"
			"skadad betong. Mellan husen finns\n"
			"manshöga ställningar av förvridet skrot, \n"
			"halvt raserade staket och trasiga bänkar.\n"
			"Otaliga vrak pryder också området.\n"
			"I husen har tydligt någon sorts bostäder\n"
			"legat. Varje enhet tycks lik de andra och\n"
			"varierar minimalt. Det tycks enbart vara\n"
			"antalet rum som skiljer dem åt. Rester\n"
			"av forntida möbler, husgeråd och annat\n"
			"finns i de flesta av utrymmena.");
		break;
	case 17:
		ImGui::TextWrapped(u8"MOTORVÄG\n"
			"Asfalten är sprucken och de vita\n"
			"strecken har nästan flagnat bort. Före\n"
			"undergången susade den gamla världens\n"
			"folk fram här i hisnande hastighet\n"
			"i sina blänkande motorkärror. Nu står\n"
			"fordonen utbrända och sönderrostade\n"
			"längs vägkanten.");
		break;
	case 18:
		ImGui::TextWrapped(u8"MUSEUM\n"
			"En mäktig trappa i sprucken sten leder\n"
			"upp till en pampig byggnad. Där inne\n"
			"finns mörka salar med märkliga föremål\n"
			"från den gamla världen och ännu tidigare\n"
			"världar, uppställda i montrar för\n"
			"fornfolket att beskåda.");
		break;
	case 19:
		ImGui::TextWrapped(u8"NÖJESFÄLT\n"
			"En bisarr samling maskiner och stora\n"
			"tält i färger som en gång varit bjärta står\n"
			"och vittrar sönder. Resterna av tältduken\n"
			"slår i vinden. På en bild syns en leende\n"
			"man med vitmålat ansikte och röd näsa.\n"
			"En maskin har formen av en stor rund\n"
			"träplatta med plasthästar på. Plattan är\n"
			"monterad på en motor och verkar ha\n"
			"kunnat rotera.");
		break;
	case 20:
		ImGui::TextWrapped(u8"PARKERINGSHUS\n"
			"Betongpelarna har vikt sig och hela den\n"
			"runda byggnaden har säckat ihop som\n"
			"ett sammanpressat dragspel. Men i rasmassorna\n"
			"mellan våningsplanen döljer\n"
			"sig mängder av motorkärror från den\n"
			"gamla världen. Bilar, kallades de före\n"
			"undergången.");
		break;
	case 21:
		ImGui::TextWrapped(u8"POLISSTATION\n"
			"Utanför den stora byggnaden vilar vrak\n"
			"som en gång var identiska fordon. Däcken\n"
			"är sedan länge borta, men takens blå\n"
			"plastkåpor finns kvar. Rummen i byggnadens\n"
			"källare går inte att öppna inifrån.");
		break;
	case 22:
		ImGui::TextWrapped(u8"RADIOSTATION\n"
			"En skog av rostiga antenner och buckliga\n"
			"plåtfat sträcker sig mot zonhimlen\n"
			"från taket på denna massiva byggnad.\n"
			"En skylt på fasaden visar en antenn med\n"
			"ringar omkring.");
		break;
	case 23:
		ImGui::TextWrapped(u8"RASERAD BRO\n"
			"Högt i Zonens gula dis reser sig resterna\n"
			"av en majestätisk bro. Den har rasat åt\n"
			"både väster och öster och bara två tjocka\n"
			"pelare står kvar, men den som vågar\n"
			"klättra upp har hittat en perfekt utkiksplats\n"
			"över Zonens ruiner.");
		break;
	case 24:
		ImGui::TextWrapped(u8"SKYDDSRUM\n"
			"En massiv rostig stålport i bergväggen\n"
			"leder ned i djupet. Där nere sökte den\n"
			"gamla världens folk skydd mot fasorna i\n"
			"ytvärlden. Ett perfekt ställe att leta efter\n"
			"artefakter. Men vem vet vad som gömmer\n"
			"sig där nere, och vem mer som kan\n"
			"ha hittat hit ? ");
		break;
	case 25:
		ImGui::TextWrapped(u8"SLAGFÄLT\n"
			"Området är fullkomligt ödelagt. Rostiga\n"
			"armeringsjärn spretar ur knäckta\n"
			"betongblock som en gång varit husväggar.\n"
			"Ruinerna är svedda av explosioner\n"
			"och genomborrade av kulhål, marken\n"
			"sargad av kratrar.");
		break;
	case 26:
		ImGui::TextWrapped(u8"SNABBMATSRESTAURANG\n"
			"Ett stort M i form av två smutsgula bågar\n"
			"pryder fasaden på denna ruin. Liknande\n"
			"byggnader finns på massor av ställen i\n"
			"Zonen, minst en i varje sektor. Vad som\n"
			"fanns där förr vet ingen längre, men\n"
			"M-husen undviks av de flesta zonstrykare\n"
			"på grund av den starka rötan i dem.");
		break;
	case 27:
		ImGui::TextWrapped(u8"SPORTHALL\n"
			"En stor och rymlig byggnad i ärgad\n"
			"korrugerad plåt. Där inne finns märkliga\n"
			"saker som tjocka madrasser i röd\n"
			"plast, stora uppspända nät, klubbor\n"
			"med hål i och bollar i olika storlekar\n"
			"och färger. Väggarna är täckta av trästegar\n"
			"som nu har murknat och fallit\n"
			"loss. Det sägs att folk före katastrofen\n"
			"gick hit för att träna.");
		break;
	case 28:
		ImGui::TextWrapped(u8"SJUKHUS\n"
			"Till detta betongkomplex sökte sig den\n"
			"gamla världens folk i horder när apokalypsen\n"
			"kom. Nu är huset en fallfärdig ruin.\n"
			"Överallt i salar och korridorer, på sängar, \n"
			"britsar och golv, ligger förtorkade lik.");
		break;
	case 29:
		ImGui::TextWrapped(u8"STRIDSVAGN\n"
			"En skräckinjagande metalltingest blockerar\n"
			"vägen. Dess enorma larvfötter har för\n"
			"länge sedan rostat fast och ingen död och\n"
			"förintelse har skjutits ur kanonröret på\n"
			"otaliga år, men att närma sig pansarbesten\n"
			"väcker ändå oro och smygande fasa.");
		break;
	case 30:
		ImGui::TextWrapped(u8"TEATER\n"
			"Inne i denna mörka ruin finns en stor sal, \n"
			"fylld med långa rader av fåtöljer framför\n"
			"en stor scen med en mäktig röd ridå. Hit\n"
			"gick den gamla världens folk för att se\n"
			"sång, dans och skådespel.");
		break;
	case 31:
		ImGui::TextWrapped(u8"TUNNELBANA\n"
			"På en sargad husfasad hänger en\n"
			"flagnad skylt med ett blått T i en vit\n"
			"cirkel. Alla zonstrykare vet vad symbolen\n"
			"betyder. Här finns en nedgång\n"
			"till tunnlarna som löper under Zonen.\n"
			"I djupet ruvar stora faror – men kanske\n"
			"också orörda artefakter.");
		break;
	case 32:
		ImGui::TextWrapped(u8"TÅGSTATION\n"
			"På rostiga och övervuxna spår står flera\n"
			"forntida rälsvagnar och vittrar sakta sönder.\n"
			"I dessa trängde den gamla världens\n"
			"folk in sig för att snabbt och effektivt\n"
			"resa tillsammans kors och tvärs i den\n"
			"stora staden. Nu sveper vinden genom\n"
			"vagnarnas krossade fönster.");
		break;
	case 33:
		ImGui::TextWrapped(u8"SNABBKÖP\n"
			"En storslagen byggnad reser sig bland\n"
			"ruinerna.Dess stora skyltfönster är krossade\n"
			"för länge sedan och zondammet\n"
			"driver in obehindrat. Före undergången\n"
			"rymde byggnaden en oändlig mängd\n"
			"kläder, skor, böcker, köksartiklar, möbler\n"
			"och andra prylar som folk konsumerade\n"
			"i den gamla världen. Nu är det mesta\n"
			"bränt och söndervittrat.");
		break;
	case 34:
		ImGui::TextWrapped(u8"VÄGTUNNEL\n"
			"Mynningen till en bred tunnel gapar\n"
			"svart och hotfull. Utanför tunneln trängs\n"
			"en oräknelig mängd motorkärror, sönderrostade\n"
			"och tätt sammanpressade.\n"
			"Vilken fasa fordonens passagerare än\n"
			"flytt ifrån är det uppenbart att få lyckades\n"
			"komma undan.");
		break;
	case 35:
		ImGui::TextWrapped(u8"ÖVERGIVEN SKOLA\n"
			"På det fallfärdiga tegelhusets fasad\n"
			"hänger en vit klocka som stannat för\n"
			"länge sedan. Längre ned på väggen har\n"
			"någon målat text, vad det står går inte\n"
			"längre att läsa. På asfaltplanen framför\n"
			"byggnaden står två rostiga metallstavar\n"
			"nedkörda i marken, drygt två meter höga\n"
			"och med varsin ring högst upp.");
		break;
	case 36:
		ImGui::TextWrapped(u8"ÖDELAGD MARINA\n"
			"Vid strandkanten ligger otaliga båtvrak\n"
			"från den gamla tiden, halvt nedsjunkna i\n"
			"det osunda vattnet. Segelbåtar, motorbåtar\n"
			"och kanoter som folk roade sig med i\n"
			"den gamla världen har förvandlats till ett\n"
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
			"Plåttaket har rasat in på denna stora\n"
			"barackliknande byggnad. Under taket\n"
			"finns maskiner och märkliga fordon med\n"
			"stora gafflar längst fram. Det mesta är\n"
			"sargat av väder och vind. Några rostiga\n"
			"tunnor har vält och en grönaktig sörja\n"
			"har runnit ut på marken...");
		break;
	case 2:
		ImGui::TextWrapped(u8"FARTYGSVRAK\n"
			"Ett stort rostigt fartyg ligger förlist vid\n"
			"strandkanten, halvt nedsjunket under\n"
			"vattenytan. De runda gluggarna i skrovet\n"
			"får båten att se ut som ett stort strandat\n"
			"valkadaver. Det kan finnas skrot kvar\n"
			"i maskinrummet.");
		break;
	case 3:
		ImGui::TextWrapped(u8"KRAFTLEDNING\n"
			"Ett högt torn byggt av rostiga stålbalkar\n"
			"reser sig mellan ruinerna. Det är knäckt\n"
			"på mitten, och den spretiga toppen är\n"
			"vikt ned mot marken. Därifrån löper\n"
			"tjocka ledningar, som en gång försåg\n"
			"hela Zonen med elektricitet.");
		break;
	case 4:
		ImGui::TextWrapped(u8"MILITÄRBAS\n"
			"En stor öppen grusplan är omgiven av\n"
			"tegelbyggnader på alla fyra sidor. I husen\n"
			"finns ett stort antal likadana rum med\n"
			"våningssängar i metall. Bortanför byggnaderna\n"
			"finns en jättelik garagebyggnad\n"
			"och utanför står ett utbränt skelett av en\n"
			"stridsvagn. Stridsvagnen kommer aldrig\n"
			"mer att röra sig, men kan utgöra ett bra\n"
			"skydd vid ett hastigt överfall.");
		break;
	case 5:
		ImGui::TextWrapped(u8"PIPELINE\n"
			"Ett grovt och rostigt metallrör löper genom\n"
			"Zonen, stort nog att krypa i. Det har för\n"
			"länge sedan slutat flöda gas eller olja i\n"
			"röret, nu duger det bara till metallskrot.");
		break;
	case 6:
		ImGui::TextWrapped(u8"RADIOMAST\n"
			"Ett högt metalltorn reser sig över ruinlandskapet.\n"
			"Det är uppstagat av mäktiga vajrar\n"
			"och i toppen spretar stora rostiga antenner.\n"
			"Därifrån sändes etersändningar ut till\n"
			"nytta och nöje för den gamla världens folk, \n"
			"sägs det. Nu hörs bara zonvinden.");
		break;
	case 7:
		ImGui::TextWrapped(u8"RAFFINADERI\n"
			"Ett helt landskap av rostiga och sönderfallande\n"
			"maskiner breder ut sig här.\n"
			"Enorma cisterner binds samman av\n"
			"metallrör, vissa tjocka nog för en fullvuxen\n"
			"mutant att krypa in i. Den som\n"
			"vågar kan klättra omkring i det metalliska\n"
			"mardrömslandskapet på rostiga\n"
			"stegar och lejdare.");
		break;
	case 8:
		ImGui::TextWrapped(u8"RENINGSVERK\n"
			"Stanken tilltar för varje steg närmare\n"
			"denna anläggning, men alla zonstrykare\n"
			"vet att lukten av förruttnelse kan dölja en\n"
			"ovärderlig skatt. De stora maskinerna här\n"
			"renade fornmänniskans avloppsvatten\n"
			"och gjorde det drickbart igen. Rent vatten\n"
			"kanske ännu finns i någon av cisternerna? ");
		break;
	case 9:
		ImGui::TextWrapped(u8"SKJUTBANA\n"
			"Under taket som sträcker sig längs ena\n"
			"sidan av den öppna ytan går det fortfarande\n"
			"att hitta tomhylsor. I andra änden\n"
			"av planen finns en hög vall och i ett rangligt\n"
			"skjul ligger måltavlor som förställer\n"
			"både människor och djur. I den gamla\n"
			"världen hade man råd att använda patroner\n"
			"till att skjuta på pappfigurer.");
		break;
	case 10:
		ImGui::TextWrapped(u8"SOPTIPP\n"
			"Ett berg av avskräde skymmer sikten.\n"
			"Här dumpade den gamla världens folk\n"
			"allt de inte fann värt att spara. Mycket\n"
			"har gnagts till stoft av tidens tand, men\n"
			"plastprylar och stora stycken rostig\n"
			"metall sticker upp ur bråten. Trots åren\n"
			"som gått hänger en fadd stank i luften.");
		break;
	case 11:
		ImGui::TextWrapped(u8"VINDKRAFTVERK\n"
			"En runt vitt torn i nu rostig metall reser\n"
			"sig ur ruinerna. Högst upp sitter en\n"
			"enorm propeller. Det sägs att den gamla\n"
			"världens människor började bygga vindsnurror\n"
			"som den här för att få energi när\n"
			"oljan i marken sinade. Det var förstås alldeles\n"
			"för sent.");
		break;
	case 12:
		ImGui::TextWrapped(u8"OLJECISTERN\n"
			"Den stora cylinderformade byggnaden\n"
			"har en trappa som slingrar sig runt utsidan\n"
			"upp till taket. Uppe på taket kan\n"
			"man upptäcka faror på stort avstånd.\n"
			"Byggnaden innehåller en svart klibbig\n"
			"vätska som en skrotskalle skulle kunna\n"
			"använda som drivmedel. Det gäller bara\n"
			"att få ut det...");
		break;
	}
}

void humanoidBeskrivning(int i)
{
	switch (i + 1)
	{
	case 1:
		ImGui::TextWrapped(u8"DJURMUTANTER -1\n"
			"I Zonen finns en stam av varelser som inte liknar några andra. De är "
			"inte människor, de är inte bestar - de är hälften av varje. De är djur, men går "
			"upprätt på bakbenen, använder verktyg och tycks ha utvecklat ett primitivt, "
			"grötigt talspråk. De är nykomlingar i Zonen, och är misstänksamma mot "
			"alla de möter. Trots sitt ringa antal verkar djurmutanterna tillhöra flera olika "
			"arter - några liknar raggiga hundar på två ben, andra är kortvuxna och liknar "
			"gnagare och en tredje art är apliknande klättrare.\n"
			"Sanningen är att djurmutanterna - likt Folket självt - har sitt ursprung i "
			"den gamla världens genlab, framexperimenterade för att överleva katastrofens "
			"härjningar.\n"
			"De är djur som har begåvats med delar av människans intellekt och finmotorik. Den "
			"lilla stam som nu rör sig i Zonen har rymt från en forskningsanläggning i de fjärran "
			"bergen, där djurmutanter varit inspärrade i generationer och där många artfränder "
			"fortfarande hålls fångna av de mystiska Väktarna.Rymlingarna har sökt sig till "
			"Zonen för att finna en ny hemvist.\n"
			"SPRÅK: Djurmutanternas språk är besläktat med Folkets, men svårt att förstå efter "
			"som det är blandat med skall, gläfsanden, morranden och kroppsspråk. "
			"En rollperson måste lyckas med att känna zonen för att begripa språket.\n"
			"\n"
			"HUND: STY 4, KYL 5, SKP 3, KNS 2,\n"
			"Slåss FV 1, Fly FV 1.\n"
			"\n"
			"BJÖRN: STY 5, KYL 4, SKP 2, KNS 3,\n"
			"Slåss FV 2.\n"
			"\n"
			"GNAGARE : STY 2, KYL 5, SKP 5, KNS 2,\n"
			"Smyga FV 2.\n"
			"\n"
			"APA : STY 3, KYL 5, SKP 4, KNS 2,\n"
			"Fly FV 2.\n"
			"\n"
			"MUTATIONER : Djuren är en annan slags mutanter än Folket.De har inte\n"
			"mutationer och använder inte mutationspoäng.\n"
			"VAPEN : Skrotspjut, slunga, huggtänder (vapenskada 2, prylbonus noll).");
		break;
	case 2:
		ImGui::TextWrapped(u8"EXILMUTANTER ±0\n"
			"Ett tiotal mutanter i eländigt skick. Deras kläder är i trasor, "
			"några haltar eller har dåligt läkta sår. De är alla utmärglade och "
			"de flesta är smittade av Rötan, och kanske även parasitsvamp(sid 72) "
			"och magtorsk(sid 70).\n"
			"Dessa mutanter kommer från en annan Ark, men har blivit utstötta. "
			"Bemöts exilmutanterna vänligt tigger de om vatten och mat. Är "
			"rollpersonerna fientliga kan deras utsatthet gå över till vrede och "
			"trots att de är svaga och illa rustade kan de gå till anfall. "
			"Hjälp från RP gör gruppen övervallande tacksam.\n"
			"\n"
			"Exilmutanterna kan berätta om sin forna Ark - och kan ha en ledtråd om Eden(kapitel 8).\n"
			"GRUNDEGENSKAPER: STY 2, KYL 2, SKP 2, KNS 2.\n"
			"FÄRDIGHETER : Fly FV 1, Slåss FV 1.\n"
			"MUTATIONER : En slumpmässig mutation.\n"
			"VAPEN : Tillhygge eller slunga.");
		break;
	case 3:
		ImGui::TextWrapped(u8"EXPEDITION FRÅN ANNAN ARK +1\n"
			"En expedition, inte olika rollpersonernas egen, men från en annan Ark.\n"
			"Främlingarna är ungefär lika många som rp.\n"
			"Om rollpersonerna inte har stött på folk från en främmande Ark "
			"tidigare(till exempel via hotkortet Det andra Folket, sid 30) "
			"kan detta bli ett omvälvande möte, för båda parter."
			"Läs mer om andra Arker och andra mutanter ovan.\n"
			"\n"
			"GRUNDEGENSKAPER: STY 3, KYL 4, SKP 3, KNS 2\n"
			"(eller enligt syssla\n"
			"FÄRDIGHETER : Som SLP i Arken(sid 19).\n"
			"MUTATIONER : En slumpmässig mutation.\n"
			"VAPEN : Slagträ med spikar eller skrotgevär.\n"
			"SPECIAL : Minst en i gruppen är en zonstrykare, med fv 3 i leda vägen. De kan ha en ledtråd om Eden(kapitel 8).");
		break;
	case 4:
		ImGui::TextWrapped(u8"KANNIBALER ±0\n"
			"En skara utmärglade mutanter klädda i trasor och med ansikten "
			"täckta av vita markeringar.De är beväpnade med skrotspjut och andra primitiva vapen.\n"
			"Gruppen är mutanter från en Ark där forskarna dog tidigt(kapitel 8) "
			"och brutala vanor blev vardag. De är sluga och blodtörstiga och hungrar "
			"efter färskt mutantkött. De går inte att resonera med och eventuell "
			"frist innebär enbart att de söker ett större övertag mot sina offer.\n"
			"\n"
			"Enbart överväldigande vapenmakt och kraftig decimering av den egna "
			"gruppen kan avskräcka kannibalerna. Deras egna som faller blir senare "
			"offer för deras egen hunger.\n"
			"\n"
			"GRUNDEGENSKAPER: STY 5, KYL 4, SKP 3, KNS 2\n"
			"FÄRDIGHETER : Slåss FV 2, Fly FV 1.\n"
			"MUTATIONER : En slumpmässig mutation.\n"
			"VAPEN : Skrotspjut eller slunga.");
		break;
	case 5:
		ImGui::TextWrapped(u8"MINNESLÖS -2\n"
			"En kvinna som stapplar fram genom Zonen.Hennes blick är "
			"blank och riktad mot någon inre fasa, och hon rabblar "
			"något knappt hörbart. Kvinnans klädsel är märklig, en lortig "
			"overall med märkliga tecken på. Hon är okontaktbar och kollapsar "
			"om RP stoppar henne. Hon måste VÅRDAS för att inte avlida. Om hon "
			"överlever kan RP prata med henne, men hennes dialekt är främmande "
			"och hon verkar har mist allt minne av vem hon är och var hon kommer "
			"från. Hon kan dock ha en ledtråd om Eden(se kapitel 8).\n"
			"\n"
			"Om ni har spelat hotkortet Mannen på stranden(sid 32) känner RP "
			"igen både klädseln och dialekten - kvinnan har samma. Hon reagerar "
			"också likartat om hon förs till Arken. Om de två märkliga främlingarna "
			"förs samman kanske de börjar minnas något?\n"
			"\n"
			"GRUNDEGENSKAPER : STY 2 (nu 1), KYL 2 (nu 1), SKP 4, KNS 2\n"
			"FÄRDIGHETER : Förstå sig på FV 2, Vårda FV 1.\n"
			"MUTATION : Inga.\n"
			"VAPEN : Inga.");
		break;
	case 6:
		ImGui::TextWrapped(u8"MORLOCKER ±0\n"
			"Degenererade vildmutanter som tagit sin tillflykt under jord, "
			"i tunnlar, källare och skyddsrum. Från början var morlockerna som "
			"Folket själva och levde i en annan Ark, men deras kroppar och sinnen "
			"har förfallit och deras civilisation gått förlorad. Morlockerna har "
			"glömt den gamla världens språk och talar genom grymtningar och tecken. "
			"De är inte fientliga, snarare avvaktande, och kan hjälpa rollpersonerna "
			"om de får något i utbyte.\n"
			"De försvarar sig med djuriskt ursinne om de blir angripna.\n"
			"\n"
			"De kan ha en ledtråd om Eden(kapitel 8).\n"
			"\n"
			"GRUNDEGENSKAPER: STY 2, KYL 3, SKP 5, KNS 2\n"
			"FÄRDIGHETER : Fly FV 2, Slåss FV 1.\n"
			"MUTATIONER : En slumpmässig mutation.\n"
			"VAPEN : Skrotspjut.");
		break;
	case 7:
		ImGui::TextWrapped(u8"MOTORFOLK ±0\n"
			"Ett dån som av tilltagande åska hörs över Zonen. "
			"Snart mullrar en hord av bisarra fordon fram mellan ruinerna.\n"
			"Bilar, bussar, motorcyklar och lastbilar; alla prydda av vassa spikar, "
			"dödskallar och fornskrot. Det fruktade motorfolket är mutanter "
			"som lämnat sin Ark för länge sedan och hade turen att komma över "
			"en stor samling fungerande fordon från den gamla världen.\n"
			"\n"
			"GRUNDEGENSKAPER: STY 4, KYL 5, SKP 2, KNS 3\n"
			"FÄRDIGHETER : Skjuta FV 3, Slåss FV 2, Mecka FV 1.\n"
			"MUTATIONER : En slumpmässig mutation.\n"
			"VAPEN : Motorcykelkedja, skrotgevär, skrotrustning(skyddsvärde 3)."
			"SPECIAL: Motorfolket har bilar, mopeder och andra fordon (se sidan 97 för speldata), ofta bestyckade med eldkastare eller till och med skrotkanoner. Läs mer om motorfolket på sidan 119.");
		break;
	case 8:
		ImGui::TextWrapped(u8"PATRULL FRÅN ARKEN\n"
			"En patrull från Arken, som har hittat en värdefull artefakt "
			"eller säger sig veta var en finns. Ledaren vill antingen få "
			"hjälp med att få fyndet till Arken eller hitta det. En av "
			"deltagarna i patrullen kan gärna vara en SLP som en eller "
			"flera rollpersoner har en relation till - kanske är det till "
			"och med ledaren. Patrullen har splittrad lojalitet då hälften "
			"är underhuggare till en boss, resten till en annan.\n"
			"\n"
			"GRUNDEGENSKAPER: Beroende på syssla, som slp i Arken(sid 19).\n"
			"FÄRDIGHETER : Som slp i Arken(sid 19).\n"
			"MUTATIONER : En slumpmässig mutation var.\n"
			"VAPEN : Slagträ med spikar, skrotkniv, skrotgevär.\n"
			"SPECIAL : Minst en i gruppen är en zonstrykare, med fv 3 i Vägvisare.");
		break;
	case 9:
		ImGui::TextWrapped(u8"SEKTEN NOVA -1\n"
			"En procession av män och kvinnor med rakade skallar och klädda i långa slöjor."
			"Gruppen lämnar några bylten på ett betongblock och rör sig sedan "
			"hastigt därifrån.Går RP fram hittar de en handfull spädbarn. "
			"Alla är nedkylda och svaga och risken är stor att alla dör. "
			"Överlever någon kommer denne uppvisa kraftiga mentala mutationer."
			"De beslöjade tillhör den mytomspunna sekten Nova(sid 35). "
			"Om RP stoppar dem behandlas de som luft, eller tilltalas "
			"som dumma barn. Tar RP till våld tvekar sektmedlemmarna inte "
			"att använda sina mentala mutationer. RP kan också skugga dem "
			"och hitta vägen till deras bunker(sid 113)."
			"\n"
			"GRUNDEGENSKAPER: STY 2, KYL 3, SKP 5, KNS 4"
			"FÄRDIGHETER : Fly FV 2, Vårda FV 1."
			"MUTATIONER : En mutation, men enbart av mentalt slag(Mardrömmare, Mental dominans, Parasit, Patokinesi, Pyrokinesi eller Telepati)."
			"Novas medlemmar är mäktiga mentala mutanter och har normalt 5 MP från start i stället för 3."
			"VAPEN : -");
		break;
	case 10:
		ImGui::TextWrapped(u8"SKROTORAKEL -1\n"
			"En kort och satt mutant klädd i en grotesk utstyrsel prydd av "
			"otaliga föremål från forntiden. Han är en särling med kraftiga "
			"mentala mutationer. Mutanten är telepat och kan få starka visioner "
			"om RP:s möjliga öde. Skrotoraklets syner kan hjälpa RP med att "
			"tolka en ledtråd om Eden, ge dem en ledtråd om de saknar en, "
			"guida dem till lösningen för hur en ännu okänd artefakt fungerar "
			"eller kanske leda dem till var en mäktig artefakt går att hitta.\n"
			"\n"
			"GRUNDEGENSKAPER : STY 2, KYL 3, SKP 4, KNS 3\n"
			"FÄRDIGHETER : Manipulera FV 2.\n"
			"MUTATIONER : Telepati, Patokinesi, Röt - ätare\n"
			"VAPEN : -\n"
			"SPECIAL : Skrotoraklet är en mäktig mental mutant, och har 5 mp.");
		break;
	case 11:
		ImGui::TextWrapped(u8"SMITTADE ±0\n"
			"En grupp degenererade vildmutanter från en Ark som förfallit och gått under.\n"
			"De är sorgliga trashankar, täckta av bölder och utväxter. "
			"Alla har deformerade kroppsdelar eller sjukliga lyten. "
			"En del har öppna sår ur vilket tjockt var flyter. Andra utsöndrar "
			"stinkande sekret ur svulster och kroppsöppningar. De är en tragisk "
			"samling mutanter i djupt stadium av sönderfall och sjukdom som "
			"varken har ork eller vilja till annat än att sakta vandra tills de stupar.\n"
			"\n"
			"GRUNDEGENSKAPER: STY 1, KYL 1, SKP 1, KNS 1\n"
			"FÄRDIGHETER : -\n"
			"MUTATIONER : En slumpmässig mutation.\n"
			"VAPEN : -\n"
			"SPECIAL : Bara att vistas en stund i närheten av de smittade räknas "
			"som en rötattack(kapitel 7 i Regelboken). Varje direktkontakt med "
			"deras kroppar räknas som ytterligare en rötattack. De smittade bär "
			"sannolikt även på parasitsvamp(sid 72) och magtorsk(sid 70).");
		break;
	case 12:
		ImGui::TextWrapped(u8"UNDERGÅNGSKULT ±0\n"
			"En skara mutanter, sedan åratal utstötta från sin Ark, som dyrkar "
			"en särskild ruin eller artefakt i Zonen. De har mist all sans och "
			"lever bara för att tillbedja sitt kultföremål, som de tror ska locka "
			"den gamla världens människor tillbaka till jorden. Kultisterna kan "
			"försöka värva rollpersoner och SLP, eller fånga in dem för att offra "
			"dem till sin gud. Sekten kan ha en ledtråd om Eden(kapitel 8).\n"
			"\n"
			"GRUNDEGENSKAPER: STY 3, KYL 2, SKP 3, KNS 4\n"
			"FÄRDIGHETER : Slåss FV 1, Manipulera FV 2.\n"
			"MUTATIONER : En slumpmässig mutation.\n"
			"VAPEN : Tillhygge.");
		break;
	case 13:
		ImGui::TextWrapped(u8"VANDRARE -1\n"
			"En mager och bister skara människor i lappade och trasiga kläder. "
			"De är på sin vakt men inte direkt fientliga - om RP är fredliga "
			"kan främlingarna berätta, på en märklig dialekt, att de har vandrat "
			"i åratal från bergen i väster. De ber om krubb och vatten. De vill "
			"veta var RP:s bosättning finns, och kan bli hotfulla om RP vägrar "
			"svara. Om ni redan har spelat hotkortet Vandrarna från bergen(sid 36) "
			"kan detta vara en grupp utbrytare, som har drivits bort från "
			"huvudgruppen efter en maktkamp.\n"
			"Slutligen kan dessa vandrare ha en ledtråd om Eden(kapitel 8).\n"
			"\n"
			"GRUNDEGENSKAPER : STY 4, KYL 3, SKP 4, KNS 3\n"
			"FÄRDIGHETER : Skjuta FV 3, Slåss FV 2, Förstå sig på FV 1.\n"
			"MUTATIONER : Inga.\n"
			"VAPEN : Skrotkniv, vedyxa, en artefakt.");
		break;
	case 14:
		ImGui::TextWrapped(u8"VATTENHANDLARE ±0\n"
			"En ensam resenär som trotsar Zonens faror för att handla med rent vatten. "
			"Det är en storväxt mutant som släpar på en skrotkärra, följd av två "
			"strykarhundar. På kärran finns plastflaskor, plåtdunkar och en underlig "
			"konstruktion av tuber och rör.\n"
			"Det senare är ett hemmabyggt reningsverk drivet av ett sotigt kokkärl. "
			"Vattenhandlaren har ett öppet och trevligt sätt och är alltid redo för affärer. "
			"Han är inte dumdristig och kapabel till att försvara sig. "
			"Mutanten vet mycket om Zonen och kan berätta var andra bosättningar finns.\n"
			"Han kan också ha en ledtråd om Eden.\n"
			"Handlaren tar glatt emot information om var Folkets Ark finns.\n"
			"\n"
			"GRUNDEGENSKAPER: STY 2, KYL 3, SKP 4, KNS 5\n"
			"FÄRDIGHETER : Manipulera FV 2, Skjuta FV 1.\n"
			"MUTATIONER : En slumpmässig mutation.\n"
			"VAPEN : Tillhygge, skrotpistol(5 patroner)\n"
			"SPECIAL : Vattenhandlaren har rötfritt vatten att sälja. "
			"Det kan dock innehålla magtorsk(sid 70).Priset beror, som alltid, på efterfrågan.");
		break;
	case 15:
		ImGui::TextWrapped(u8"VRAKRÖVARE ±0\n"
			"Ett oväntat intakt fordonsvrak står orört i en glänta eller på ett ruintorg. "
			"Vrakets rutor är smutsiga, luckorna är låsta och skrovet är av tjock pansarplåt. "
			"Vraket är en fälla. En skock vildmutanter eller zongastar rustade med skrotpansar "
			"och enkla vapen använder fordonet som lockbete för nomader och vandrare.\n"
			"Deras vanliga plan är simpel: de rusar fram och söker genom numerärt "
			"överläge övervinna sina offer. De plundrar sina byten på allt och släpar "
			"iväg dem som slavar till sin stam. Mutanterna har själva inte kunnat "
			"öppna vraket. Lyckas någon ta sig in i det hittar man två artefakter i "
			"mycket gott skick.\n"
			"\n"
			"GRUNDEGENSKAPER : STY 3, KYL 4, SKP 3, KNS 2\n"
			"FÄRDIGHETER : Slåss FV 3, Skjuta FV 2, Smyga FV 1.\n"
			"MUTATIONER : En slumpmässig mutation om vrakrövarna är vildmutanter, "
			"ingen mutation om de är zongastar(nedan).\n"
			"VAPEN : Slagträ med spikar, slunga.");
		break;
	case 16:
		ImGui::TextWrapped(u8"ZONGASTAR ±0\n"
			"Zongastar är vad Folket berättar skräckhistorier vid lägerelden om. "
			"Detta vildsinta folk, som lever som nomader ute i Zonen, rör sig som "
			"skuggor i ruinerna och anfaller andra zonvandrare utan förvarning. "
			"De är mest aktiva om natten, och är sällan ute i dagsljus. Ryktena om "
			"zongastarna är många, det sägs att de flår och äter mutanter som de fångat.\n"
			"De är som regel kortväxta och rör sig ofta i stora grupper. Zongastar "
			"döljer sina ansikten bakom skräckinjagande masker från den gamla tiden och går oftast klädda i plastrockar som döljer deras huvuden.De slåss med grova tillhyggen, slungor och stenar.De använder sällan artefakter.\n"
			"\n"
			"Beskriv zongastarna som skräckinjagade första gången rollpersonerna möter dem. "
			"De är smygande fasor i ruinerna, dolda hot, ständigt på jakt. Berätta ryktena "
			"som går om zongastarna i Arken. I strid är de fasansfulla vildar utan nåd "
			"och besinning.\n"
			"Senare, om RP möter zongastarna igen, ge dem en chans att se en annan "
			"sida av de fruktade fienderna.Kanske stöter de på en sårad zongast.\n"
			"\n"
			"SPRÅK: Zongastar har ett primitivt gutturalt språk som Folket har mycket "
			"svårt att förstå. En rollperson måste lyckas KÄNNA ZONEN för att förstå det.\n"
			"GRUNDEGENSKAPER : STY 3, KYL 4, SKP 3, KNS 2\n"
			"FÄRDIGHETER : Fly FV 3, Skjuta FV 2, Slåss FV 1.\n"
			"MUTATIONER : Inga.\n"
			"VAPEN : Cykelkedja, slunga.\n"
			"SPECIAL : I direkt dagsljus tar zongastar en poäng skada per timme, "
			"trots sina skyddsdräkter.\n"
			"\n"
			"SANNINGEN OM ZONGASTARNA\n"
			"Vad Folket inte vet är att zongastarna är ättlingar till människor "
			"som överlevde undergången. Mot alla odds har de överlevt Rötan och "
			"hungern.Men de blir inte gamla, de flesta är tonåringar.\n"
			"Under alla paltor är deras kroppar bleka och krumma, inte sällan "
			"förvridna. Zongastarna har förlorat alla minnen av den mänskliga "
			"civilisationen genom åren, och de ser Folket som ett dödligt hot.\n"
			"Om grupperna kunde övervinna skräcken för den andre kanske de kunde "
			"bli värdefulla allierade.\n"
			"Zongastarna kan också bli en möjlig - om än oaptitlig - lösning på "
			"Folkets oförmåga att föröka sig(kapitel 8).");
		break;
		
	}
}
void monsterBeskrivning(int i)
{
	switch (i + 1)
	{
	case 1:
		ImGui::TextWrapped(u8"ASGNAGARE +1\n"
			"Dessa utmärglade, skabbiga gnagare lever i horder i Zonen. "
			"Rötan tycks bara göra dem större och elakare. Zonens råttor "
			"är långa som en mansarm och gnager i sig allt de kommer över, "
			"levande som dött, med sina gulnade och sylvassa huggtänder.\n"
			"\n"
			"GRUNDEGENSKAPER: STY 8, KYL 5\n"
			"PANSAR : -\n"
			"VAPEN : Sylvassa tänder(vapenskada 1).\n"
			"SVÄRM : Kan endast skadas av eldkastare, explosioner och mutationer.\n"
			"RÖTA : Varje attack från asgnagare, oavsett om den lyckas eller "
			"inte, ger offret en rötpoäng.");
		break;
	case 2:
		ImGui::TextWrapped(u8"BETESDJUR +1\n"
			"Här och var i Zonen lever flockar av relativt beskedliga betesdjur, "
			"muterade versioner av den gamla tidens får, getter, grisar och kor. "
			"De är förvildade och tvekar inte att anfalla zonvandrare som de "
			"uppfattar som hot mot flocken. Betesdjur är särskilt lämpliga att "
			"jaga för att få krubb(se talangen Slaktare i Regelboken).\n"
			"\n"
			"GRUNDEGENSKAPER: STY 2 - 5, KYL 2 - 4\n"
			"PANSAR : -\n"
			"VAPEN : Horn, bett eller trynen(vapenskada 1).");
		break;
	case 3:
		ImGui::TextWrapped(u8"DRÅPARTRÄD -1\n"
			"Ett område som är obehagligt tyst och stilla. Det är hemvist "
			"åt Zonens fruktade modergrönska; ett dråparträd. Det består av "
			"en mörk stam som slutar i ormande tentakelgrenar. Knotiga rötter "
			"täckta av illaluktande sekret har spräckt ruiner och mark. Rester "
			"av ben, skinn och fjädrar ligger överallt runt om. "
			"Dråparträdets märkliga frökapslar - humanoida kringvandrande "
			"växtmutanter som undertvingade dråparträdets vilja jagar åt "
			"rovgrönskan - finns i området och kommer göra allt för att "
			"fånga in levande väsen som rör sig nära och mata dem åt sin hungrande trädmoder.\n"
			"\n"
			"GRUNDEGENSKAPER: STY 10, KYL 6\n"
			"FÄRDIGHETER : Slåss FV 3.\n"
			"PANSAR : 6\n"
			"VAPEN : Tentakler(vapenskada 1, vid träff blir offret snärjt "
			"och måste Ta krafttag för att komma loss, även vänner intill "
			"kan slå), ringformat gap(vapenskada 3, kräver att offret är snärjt).\n"
			"\n"
			"TANKEVÅGOR :\n"
			"Dråparträdet lockar sina offer till sig med kraftfulla tankevågor. "
			"Alla inom kort avstånd måste klara att FLY för att stå emot - den "
			"som misslyckas tar en poäng förvirring och måste avancera fram mot trädet.\n"
			"\n"
			"TENTAKLER :\n"
			"När ett offer väl är på nära avstånd kan dråparträdet SLÅSS med sina "
			"tentakler. Trädet kan fördela sina grund - och färdighetstärningar på "
			"flera offer och angripa dem i samma runda.\n"
			"\n"
			"FRÖKAPSLAR :\n"
			"Offer som lyckas motstå dråparträdets lockelser blir som regel angripna "
			"av trädets humanoida frökapslar.\n"
			"I sitt naturliga tillstånd har dessa varelser grönaktig hy och saknar "
			"kroppsbehåring och tydliga anletsdrag.\n"
			"\n"
			"En frökapsel har STY 3 och KYL 5 och FV 2 i att SLÅSS.\n"
			"De kan dock efterapa en humanoids exakta utseende, genom att skicka in "
			"växtstänglar i offrets hjärna och suga ut information om offrets DNA. "
			"Offret, som måste vara fasthållet eller brutet av skada för att processen "
			"ska gå att genomföra, dör på kuppen. En frökapsel som efterapat en "
			"människa eller mutant är helt identisk med offret och kan även tala med "
			"dess röst, men är totalt känslokall och går inte att MANIPULERA eller MUCKA med.");
		break;
	case 4:
		ImGui::TextWrapped(u8"DRÖMBLOMSTER -1\n"
			"Ett område fyllt av blommor som klänger och växer överallt. Tunga "
			"fuktiga kronblad, ståndare och pistiller samt intensiva färger utmärker dem.\n"
			"Om något rör sig i området sänder zonblommorna ut sövande moln av sporer. "
			"När något faller i sömn ringlar taggiga utskott fram, borrar sig in i "
			"offrets mjukdelar och suger i sig näring tills offret är dött. Resterna "
			"av tidigare offer är täckta av den snabbt växande grönskan. En hel del "
			"prylar och kanske till och med en artefakt eller två ligger på marken i "
			"området, dolda av blomstren.\n"
			"\n"
			"EFFEKT:\n"
			"Den som Spejar mot området och lyckas får syn på rester av tidigare offer. "
			"Den som rör sig in i området måste klara att Fly för att inte somna på "
			"stället.Den som sövs tar en poäng skada varje runda tills han blir bruten "
			"eller tills någon Vårdar honom. Rustning ger normalt skydd, men bara "
			"tills offret har tagit sin första poäng skada. Drömblomstren kan enbart "
			"bekämpas med eld - räkna dess STY som 10. Vid noll är växterna döda och "
			"området kan passeras utan risk.");
		break;
	case 5:
		ImGui::TextWrapped(u8"ETTERBEST +1\n"
			"Nära Zonens vattendrag lever varelser som zonstrykare kallar "
			"etterbestar - förvuxna ödlor, långa som två mutanter om man räknar in svansen.\n"
			"Etterbestar är intelligenta och det sägs till och med att de "
			"går att tämja som riddjur. Att försöka med det är dock en riskabel "
			"affär, med tanke på etterbestarnas vresiga humör och den förmåga som "
			"givit dem deras namn : de spottar frätande syra med förödande träffsäkerhet.\n"
			"\n"
			"GRUNDEGENSKAPER : STY 5, KYL 4 (lägre i kallt väder)\n"
			"FÄRDIGHETER : Kämpa på FV 3, Slåss FV 2, Skjuta FV 3.\n"
			"PANSAR : 2\n"
			"VAPEN : Bett(vapenskada 2), syraspott(räckvidd nära, vapenskada 1, "
			"rustning skyddar normalt).");
		break;
	case 6:
		ImGui::TextWrapped(u8"FRÄTARGRÄS -1\n"
			"Ett fält av midjehögt gräs. En sur doft ligger över platsen och här "
			"och var skymtar uttorkade kadaver eller vitnande knotor.\n"
			"Gräset böjer sig till och från som om en osynlig vind sveper fram. "
			"Växtligheten är ett fält av lömskt rovgräs. Om djur eller mutanter "
			"går in i det börjar gräset sno sig runt deras fötter, ben och föremål. "
			"Om gräset snärjer något börjar det utsöndra ett frätande sekret. Offret "
			"smälts ned till en sörjig gegga som växten kan suga i sig.\n"
			"\n"
			"EFFEKT: En RP som Spejar mot området och lyckas får syn på rester av "
			"tidigare offer. Den som rör sig in i området blir omgående angripen med "
			"sex grundtärningar.\n"
			"Vid träff blir offret snärjt och tar direkt en poäng skada av frätande "
			"sekret. Rustning skyddar normalt. För att ta sig loss måste offret Ta krafttag. "
			"Till dess tar han en poäng skada till varje runda. Frätargräset kan "
			"enbart bekämpas med eld – räkna dess STY som 10. Vid noll är gräset "
			"dött och området kan passeras utan risk.");
		break;
	case 7:
		ImGui::TextWrapped(u8"JORDSLUKARE ±0"
			"En best som jagar sina byten genom att luckra upp marken ovanför "
			"sig och vänta på att ett oförsiktigt offer skall gå ned sig. "
			"Jordslukaren är en avlång och skaltäckt varelse i dubbel manslängd "
			"med otaliga ben och kraftiga mandibler. Den är enträgen och enveten i "
			"sina attacker och anfaller till offret är nedkämpat eller jordslukaren går under."
			"\n"
			"GRUNDEGENSKAPER: STY 6, KYL 4"
			"FÄRDIGHETER : Slåss FV 3, Smyga FV 5 (under jord)."
			"PANSAR : -"
			"VAPEN : Ringformat gap(vapenskada 2)."
			"Den som blir träffad blir även nerdragen i jorden. Offret måste TA KRAFTTAG "
			"för att komma loss(räknas som en handling). Så länge offret sitter "
			"fast får jordslukaren + 3 på alla attacker mot det. Om ett offer kommer "
			"loss brukar jordslukaren ge upp och Fly.");
		break;
	case 8:
		ImGui::TextWrapped(u8"MAGTORSK ±0\n"
			"Magtorsken är en fruktad parasitvarelse som sprids via dricksvatten. "
			"Även vatten som i övrigt är rötfritt och drickbart kan vara smittat - "
			"det finns inget säkert sätt att skydda sig mot magtorsken utom att koka "
			"vattnet. Parasiten växer inne i buken på värdkroppen och livnär sig på "
			"maten som värden äter. När magtorsken, som snarare liknar en ål, är "
			"fullvuxen äter den sig ut genom buken och slingrar sig i väg till närmaste vattendrag.\n"
			"\n"
			"EFFEKT: Efter smittotillfället blir offret fruktansvärt hungrigt och "
			"måste äta minst två ransoner krubb per dag. Varje utebliven ranson ger ett i skada.\n"
			"Efter några dagar eller en vecka är magtorsken fullvuxen och äter sig ut. "
			"Offret blir automatiskt bruten av skada, oavsett STY, och måste bli VÅRDAD	 "
			"för att inte avlida.");
		break;
	case 9:
		ImGui::TextWrapped(u8"MASKINVARELSE -2\n"
			"I enklavernas mörker byggde titanmakterna(kapitel 8) varelser av metall, plast och kretsar som var så avancerade att de kunde mäta sig med oss människor i intelligens och initiativförmåga.\n"
			"Vissa forskare experimenterade till och med att smälta samman människa och maskin, i hopp om att skapa en varelse bättre lämpad för livet efter katastrofen.\n"
			"\n"
			"Robotar, av en mängd olika slag, kommer att utforskas grundligt i den kommande modulen Mutant : Maskinarium.Men redan i Mutant : År Noll kan rollpersonerna stöta på enstaka maskinvarelser som av någon anledning befinner sig i Zonen.\n"
			"För Folket lär en sådan robot framstå som en fruktansvärd styggelse, en vandrande eller svävande metallvarelse med dödliga vapen inbyggda i själva kroppen.\n"
			"\n"
			"GRUNDEGENSKAPER : STY 8, KYL 6\n"
			"FÄRDIGHETER : Slåss FV 4, Skjuta FV 4.\n"
			"PANSAR : 10\n"
			"VAPEN : Sågklinga(vapenskada 2), energivapen(vapenskada 2, lång räckvidd).\n"
			"Vapnen är inbyggda och ger ingen prylbonus.\n"
			"SPECIAL : Roboten kan inte drabbas av stress, dess värde i KYL går aldrig ner.");
		break;
	case 10:
		ImGui::TextWrapped(u8"MASKSVÄRM -1\n"
			"Dessa kluster av decimeterlånga svarta maskar har utvecklat ett gemensamt medvetande och beter sig mer som en enda varelse än som separata individer.\n"
			"En typisk svärm består av miljontals maskar och kan bli flera meter i omfång.\n"
			"Det döljer sig ofta i mörka och fuktiga platser och angriper byten som passerar i närheten. Angreppet sker genom att svärmen hastigt försöker omsluta offret och försätta det i panik, för att sedan förtära det.\n"
			"\n"
			"GRUNDEGENSKAPER: STY 8 (normalt startvärde som kan öka och minska, se nedan)\n"
			"PANSAR : -\n"
			"VAPEN : Omslutning(vapenskada 1).\n"
			"Den första attacken ger stress. Fortsätter angreppet tar offret skada från runda två och vidare. För varje poäng skada offret tar ökar masksvärmens STY ett steg.\n"
			"SVÄRM : Masksvärmen kan endast skadas av eldkastare, explosioner och mutationer.");
		break;
	case 11:
		ImGui::TextWrapped(u8"PARASITSVAMP ±0\n"
			"Denna dödliga svampvarelse smittar genom mycel som sprids vid närkontakt med en infekterad kropp. Efter smittotillfället börjar parasiten växa inne i den nya värdens kropp och hjärna, som upplever fruktansvärda smärtor. Svamporganismen påverkar också offrets hjärna.\n"
			"\n"
			"EFFEKT: Efter smittotillfället tar offret en poäng skada och en poäng förvirring per dygn. Offret måste alltså äta och sova onormalt mycket för att inte bli bruten. Då och då försöker parasitsvampen tvinga offret att komma nära ett nytt offer, för att sprida smittan vidare. Offret måste klara ett slag för Kyla(ingen färdighet kan användas) för att stå emot impulsen. Det enda sättet att bli fri från smittan är att bli bruten och sedan bli Vårdad.");
		break;
	case 12:
		ImGui::TextWrapped(u8"PSI-MYGG -1\n"
			"Ett område fyllt av surrande svärmar av mygg med psioniska krafter. Alla som rör sig i deras närhet blir deprimerade och uppgivna. Allt tycks hopplöst för de som utsätts. Offren får en allt starkare önskan att inte göra något, vilket låter myggorna landa och suga friskt. Attacken är inte fysiskt dödlig, men uppgivenheten kan i sig kosta en zonfarare livet.\n"
			"\n"
			"EFFEKT: Individer som drabbas av psimyggen får varje runda en attack mot sig med ett antal grundtärningar lika med svärmens STY. Varje / ger ett poäng tvivel (trauma mot KNS). För att samla sig och ta sig i skydd krävs att man lyckas Fly. Myggsvärmen kan enbart bekämpas med eld eller explosioner. När svärmens STY når noll är den helt bekämpad. En typisk svärm psi-myggor i Zonen har STY 8 från start.");
		break;
	case 13:
		ImGui::TextWrapped(u8"RAGGBEST ±0\n"
			"Ett vildsint och storväxt rovdjur med nära manshöjd över manken. Besten har smutsgrå päls och utmärks av ett orimligt stort gap, en outsinlig hunger och förmåga att röra sig näst intill ljudlöst.\n"
			"Raggbesten smyger sig med oändligt tålamod mot sitt offer och det är sällan bytet hinner uppfatta vad som händer innan monstret halvt slukat sitt byte. Käften kan vidgas så att besten utan problem kan hugga en storväxt mutant runt överkroppen. Frätande saliv smälter sedan sönder offret som inte släpps förrän allt är svalt eller raggbesten tvingas fly.\n"
			"\n"
			"GRUNDEGENSKAPER: STY 6, KYL 5\n"
			"FÄRDIGHETER: Slåss FV 3, Smyga FV 4.\n"
			"PANSAR: 2\n"
			"VAPEN: Klor (vapenskada 1), bett (vapenskada 3, men kan bara användas om offret är ovetande om attacken).\n"
			"Om ett bett lyckas sitter offret fast och tar 1 poäng skada varje runda. För att komma loss måste offret (eller en kamrat) Ta krafttag (modifikation -2). Varje slag räknas som en handling.");
		break;
	case 14:
		ImGui::TextWrapped(u8"RUINSPINDEL ±0\n"
			"I mörka tunnlar och ruiner lever muterade spindlar som har vuxit till grotesk storlek. Oförsiktiga zonvandrare som trampar igenom gamla murkna tunneltak eller utforskar ödsliga ruiner kan hamna mitt ibland dem. Insekterna livnär sig på allt ifrån lik och avskräde till storväxta byten som de tar död på med sitt giftbett.\n"
			"\n"
			"GRUNDEGENSKAPER: STY 5\n"
			"FÄRDIGHETER: Slåss FV 2, Fly FV 2.\n"
			"PANSAR: 3\n"
			"VAPEN: Giftbett (vapenskada 1 och förlamande gift, se nedan).\n"
			"NERVGIFT: Om giftbettet gör skada tränger giftet in och gör en poäng stress per runda tills offret vårdas. Medan giftet verkar läker vatten inte kyl.");
		break;
	case 15:
		ImGui::TextWrapped(u8"RÖTMYROR ±0\n"
			"Rötmyrorna är en legend bland zonstrykare.\n"
			"Det farliga är inte insekterna i sig - nog för att de är otrevliga nog, stora som en handflata och med kraftiga käkar - utan deras överväldigande antal. Rötmyror bygger inga stackar, i stället väller de fram i mångmiljonhövdade horder genom Zonen och förintar allt i sin väg.\n"
			"De är blinda och uppfattar allt levande som föda. Det finns zonstrykare som säger sig ha sett en svärm rötmyror sluka en raggbest på ett par minuter. Insekterna blir inte trevligare av att de sprider stark röta omkring sig där de drar fram.\n"
			"\n"
			"GRUNDEGENSKAPER: STY 6\n"
			"PANSAR : -\n"
			"VAPEN : Bett(vapenskada 1).\n"
			"RÖTA : Varje attack, oavsett om den lyckas eller inte, ger offret en rötpoäng.\n"
			"SVÄRM : Rötmyrornas svärm är enorm. Den går överhuvudtaget inte att skada med vapen som rollpersonerna kan tänkas förfoga över. Det enda sättet att undkomma är att FLY.");
		break;
	case 16:
		ImGui::TextWrapped(u8"SKRÄDESVRÅK ±0\n"
			"Från höga ruiner spanar skrädesvråken ut över Zonen med sina gula ögon. Gråbruna fjädrar låter den smälta in omgivningen. Sitt namn har besten fått av sina muterade klor som liknar långa knivblad. Skrädesvråken jagar genom att dyka ner på sina offer och med klorna knipsa av lämplig del, eller bara ta tag i offret och flyga iväg. De största bestarna har mätt upp till tio meter mellan vingspetsarna.\n"
			"Yngre eller mindre exemplar nöjer sig oftast med att riva ut ögon eller knipsa av öron.\n"
			"\n"
			"GRUNDEGENSKAPER: STY 8, KYL 8\n"
			"FÄRDIGHETER: Slåss FV 3, Fly 4, Ta krafttag FV 2.\n"
			"PANSAR: 1\n"
			"VAPEN: Klor (vapenskada 2, vid träff slits ett rejält köttstycke loss).\n"
			"STÖRTDYKNING: En skrädesvråk i luften kan störtdyka från långt avstånd och direkt slåss med ett offer i samma runda. Vråken tar själv ett i skada av attacken.\n"
			"LYFTA OFFER: När skrädesvråken lyckats med att slåss kan den lyfta upp offret och flyga iväg med det genom att klara ett slag för ta krafttag.");
		break;
	case 17:
		ImGui::TextWrapped(u8"SKÖVLARE\n"
			"Ett ofantligt monster som når nära dubbel manshöjd över manken och dubbelt i längd. Trots besten små ben kan den få upp en skrämmande fart och under det trubbiga trynet sitter två grova betar som sliter sönder allt ifrån vrak och träd till mutanter och skrotpalissader.\n"
			"Allt tycks reta upp varelsen och att få stopp på en rasande skövlare är nära omöjligt. Besten verkar inte vara ett utpräglat rovdjur och jagar inte för föda, däremot äter den lika gärna as och lik som frodig grönska\n"
			"\n"
			"GRUNDEGENSKAPER: STY 10, KYL 6\n"
			"FÄRDIGHETER: Slåss FV 4.\n"
			"PANSAR: 8\n"
			"VAPEN: Tjurrusning (vapenskada 2, måste inledas på kort avstånd, ger en extra manöver till förflyttning före attacken), betar(vapenskada 3).");
		break;
	case 18:
		ImGui::TextWrapped(u8"SKROTKRÅKOR +1\n"
			"Stora smutsgrå fåglar som lever i flockar i Zonen. De anfaller gärna oförsiktiga zonvandrare och hackar besinningslöst med sina vassa näbbar mot ansikte och ögon. Dessa motbjudande fjäderfän kallas skrotkråkor av Folket, eftersom de finner allt som blänker oemotståndligt lockande - särskilt dyrbart fornskrot.\n"
			"\n"
			"GRUNDEGENSKAPER: STY 7, KYL 6\n"
			"PANSAR: -\n"
			"VAPEN: Näbbar och klor (vapenskada 1).\n"
			"SVÄRM: Kan endast skadas av eldkastare, explosioner och mutationer.");
		break;
	case 19:
		ImGui::TextWrapped(u8"STRYKARHUNDAR +1\n"
			"Hungriga och muterade flockar av byrackor är vanliga i Zonen. De är magra, seniga och saknar ofta päls helt eller fläckvis.\n"
			"\n"
			"GRUNDEGENSKAPER: STY 3, KYL 4\n"
			"FÄRDIGHETER: Slåss FV 2, Fly FV 3.\n"
			"PANSAR: –\n"
			"VAPEN: Bett (vapenskada 2).");
		break;
	case 20:
		ImGui::TextWrapped(u8"TJÄRNMAL ±0\n"
			"I ruingölar och gamla vattentäkter ruvar tjärnmalen. Monstret kan ta många skepnader, från platta fjällbetäckta fiskar till ål- eller ormliknande bestar med vassa fenor. Tjärnmalen rycker ofta ner byten som simmar i vattnet eller går längs stränder för att sen dränka offret genom att hålla fast det under ytan.\n"
			"\n"
			"GRUNDEGENSKAPER: STY 10, KYL 5\n"
			"FÄRDIGHETER: Slåss FV 3, Fly FV 4 (under vatten).\n"
			"PANSAR: 5\n"
			"VAPEN: Bett (vapenskada 2)\n"
			"DRÄNKA OFFER: Om malen lyckas slåss kan den dra ner offret i djupet för att dränka det. Offret kan ta sig loss genom att klara att fly.");
		break;
	case 21:
		ImGui::TextWrapped(u8"ZONGETINGAR ±0\n"
			"Det första zonvandrare brukar märka av dessa odjur är surrandet, onaturligt dovt och motbjudande. Zongetingarna är gulsvarta, stora som knytnävar och har hårda ytterskal. Deras sting injicerar ett kraftigt hallucinogent gift som förvirrar offret och gör det lättare för insekterna att slita i stycken. Som om det inte vore nog utsöndrar zongetingar när de hamnar i strid ett feromon som direkt lockar till sig fler surrande missfoster.\n"
			"\n"
			"GRUNDEGENSKAPER: STY 5\n"
			"PANSAR: 2\n"
			"VAPEN: Bett (vapenskada 1 och hallucinogent gift, se nedan).\n"
			"SVÄRM: Kan endast skadas av eldkastare, explosioner och mutationer.\n"
			"HALLUCINOGENT GIFT: Varje attack som ger 1 eller mer i skada ger offret en poäng förvirring.\n"
			"FEROMONER: Svärmens sty ökar ett steg varje runda tills alla offer är döda eller har lyckats fly.");
		break;
	case 22:
		ImGui::TextWrapped(u8"ZONIGLAR ±0\n"
			"Fingerlånga kryp som håller till i täta snår eller inne i mörka ruiner och faller ner över sina byten. De slemmiga kräken har varken ben eller ögon, bara ett gap fyllt av nålvassa tänder. Kommer zoniglarna i kontakt med bar hud hugger de blixtsnabbt och huden domnar. En svag pulserande smärta förnimms i området och sedan fylls sakta larvkroppen av offrets blod och vätska.\n" 
			"Ett tiotal kryp som lyckas suga sig fast på en mutant kommer lämna ett uttorkat lik efter sig.\n"
			"\n"
			"GRUNDEGENSKAPER: STY 6\n"
			"PANSAR: -\n"
			"VAPEN: Blodsugning. Om iglarnas attack (Slåss) lyckas fortsätter offret att ta en poäng skada per runda tills de tagits bort. Att skära eller rycka bort dem kräver att offret, eller en vän, lyckas Slåss. Flera försök är tillåtna.\n"
			"SVÄRM: Zoniglar kan endast skadas av eldkastare, explosioner och mutationer.");
		break;
	case 23:
		ImGui::TextWrapped(u8"ZONTÖRNE -1\n"
			"Trakten är övervuxen med snår och buskar, många täckta av grova törnen och taggar. Zonbuskarnas rotutväxter har tryckkänsliga områden. Kliver någon på dem skjuter busken mängder av sylvassa taggar i hög hastighet. Taggarna är täckta med hallucinogent gift. Offret förvirras av orimliga syner, desorienteras och riskerar att vandra in bland snåren, bara för att fastna på hullingar och törnen. Varelser som fastnar blir måltid åt de asgnagare och sumpfåglar som lever på buskarnas offer.\n"
			"\n"
			"EFFEKT: Den som går in i området utsätts för en attack med sex grundtärningar. Rustningar skyddar normalt. En träff ger ingen skada - i stället drabbas offret av hallucinogent gift som ger en poäng förvirring för varje slagen RADIAK. För att ta sig ur snåret måste offret klara att Fly.\n"
			"Snåret angriper en gång varje runda.\n"
			"Zontörnet kan enbart bekämpas med eld - räkna dess STY som 10. Vid noll är snåret nedbränt och området kan passeras utan risk.");
		break;
	case 24:
		ImGui::TextWrapped(u8"ÄRGFLYN ±0\n"
			"Ärgflyn är udda varelser, stora som en handflata, och påminner om halvgenomskinliga blåsor som svävar fram. Tunna armslånga spröt som glänser av sekret hänger under deras kroppar. Deras rörelser fyller luften med en frän stickande doft. Allt spröten berör får brännande smet över sig. Om de når metall vrider spröten sig runt föremålet och än mer syra utsöndras.\n"
			"Varelserna drar sig undan vid anfall. De hungrar bara efter metall, inte levande varelser.\n"
			"\n"
			"EFFEKT: Ärgflyna attackerar inte levande offer, utan deras prylar - och bara om de är gjorda av metall. Slå för en attack mot varje pryl, med sex grundtärningar. Varje RADIAK sänker målets prylbonus ett steg. Attackerna fortsätter ofta runda efter runda, tills offren lyckas Fly.");
		break;
	}
}
void fenomenBeskrivning(int i)
{
	switch (i + 1)
	{
	case 1:
		ImGui::TextWrapped(u8"ASKVIND +1\n"
			"En kall vind sveper genom Zonen och drar med sig den svarta askan som ligger i drivor mellan ruinerna. Askan skymmer sikten och svider i ögonen och det blir allt svårare att andas. Den otrevliga blåsten varar i några timmar eller ännu längre. När vinden till sist mojnar är det mesta täckt av ett finpudrigt asklager och alla vattenkällor inom några kilometer är förgiftad sörja. Vintertid blir snön gråfärgad och det dröjer länge innan askan försvinner.\n"
			"\n"
			"EFFEKT: Zonfarare som vill fortsätta vandra genom askvinden måste klara att Kämpa på. Den som misslyckas måste söka skydd tills askvinden är över.");
		break;
	case 2:
		ImGui::TextWrapped(u8"ATOMKÖLD ±0\n"
			"Stora vita moln sveper in och vinden friskar i rejält. Kylan blir svår och tät snö faller. Plötsligt upphör blåsten och snöfallet. En vägg av vita moln bildar en hög cylinder ovanför området. En blå himmel skymtar däröver. Stillheten har ett pris: temperaturen faller dramatiskt.\n"
			"Att vistas utomhus är förenat med livsfara. Allt levande som inte söker skydd dör. Eldar måste matas hårt eller falna.\n"
			"\n"
			"EFFEKT: Zonfarare som inte bär tjocka atomvinterställ eller forntida skyddsdräkter utsätts för effekten av köld (kapitel 6 i Regelboken). Ett slag i timmen måste slås tills köldknäppen är över. Det enda sättet att skydda sig är att tända en eld - vilket kräver bränsle, elddon och att någon lyckas med ett slag för Kyla (händerna skakar och bränslet är svårt att få fyr på).");
		ImGui::TextWrapped(u8"\nEXTREMHETTA ±0\n"
			"Använd detta fenomen endast på sommarhalvåret. Om ni spelar på vintern, välj Atomköld i stället.\n"
			"\n"
			"EFFEKT: Hettan är så svår att zonfararna måste slå för Kämpa på efter varje fysiskt ansträngande handling. Slå slaget direkt - det räknas inte som en handling.\n"
			"Misslyckande innebär att offret direkt tar en poäng stress. Även den som inte gör något alls måste dricka en ranson vatten varje timme i stället för varje dygn, så länge hettan varar - vilket brukar vara fram till kvällen.");
		break;
	case 3:
		ImGui::TextWrapped(u8"BLINDGÅNGARE +1\n"
			"Hela området är sönderskjutet och nerlusat med bomber, minor och granater.\n"
			"Många ligger odetonerade, halv dolda i zongruset. Att gå här är förenat med livsfara.\n"
			"\n"
			"EFFEKT: En person som vill passera området måste klara ett slag för att Fly. Ett misslyckat slag betyder att en gammal granat detonerar, med sprängverkan 10 (se Regelboken).");
		break;
	case 4:
		ImGui::TextWrapped(u8"DAMMTORNADO -1\n"
			"Luften fylls med sand och aska och himlen färgas smutsigt gulbrun. En ruin från den gamla världen rister plötsligt till och en kilometerhög tornado sliter den i stycken. Den enorma virveln av sand och damm maler allt i sin väg till stoft.\n"
			"När tornadon dragit förbi går ett halvkilometer brett spår av total ödeläggelse så långt man kan se. Inget som varit i dess väg har klarat sig.\n"
			"\n"
			"EFFEKT: Alla som är i dammtornadons väg måste klara att Fly för att hinna ur vägen i tid. Den som misslyckas utsätts för en attack med hela tolv grundtärningar. Varje RADIAK ger ett poäng skada. Rustning skyddar.");
		break;
	case 5:
		ImGui::TextWrapped(u8"ELEKTRISK STORM ±0\n"
			"Tunga mörka moln sänker sig över området. Luften börjar smaka metalliskt, hår ställer sig på ända och spröda blixtar vandrar längs metallföremål.\n"
			"Plötsligt börjar blixtar slå ned överallt. Om och om igen bombarderas området. Ögon tåras av det skarpa ljuset och dånet från dundrande knallar är öronbedövande. Stormen varar bara några minuter men det känns som en evighet.\n"
			"\n"
			"EFFEKT: Alla som utsätts för stormen blir mål för en attack med tre grundtärningar. Varje RADIAK ger en poäng stress, rustningar ger inget skydd. För den som står på en höjd eller klättrar i en metallställning slår du i stället sex grundtärningar.\n"
			"Slå gång på gång tills stormen har bedarrat eller tills zonfararna har lyckats söka skydd - det kräver att de klarar att Fly.");
		break;
	case 6:
		ImGui::TextWrapped(u8"LJUSPELARE ±0\n"
			"Starka ljuspelare syns vid horisonten, och strålar upp mot den svarta zonhimlen. Var kommer ljusen ifrån? Och vad har väckt dem till liv just nu?\n"
			"\n"
			"EFFEKT: Ingen. Men låt gärna SLP berättar skrönor om ljusen, att någon har hört att de visar vägen till Eden, till en annan Ark eller att de är ett ont omen.");
		break;
	case 7:
		ImGui::TextWrapped(u8"MAGNETFÄLT -1\n"
			"Rester av någon anläggning från den gamla världen skapar i detta område ett magnetiskt kraftfält av fluktuerande styrka. Zonfarare kan märka av fenomenet genom ett tilltagande surrande och att deras hår ställer sig rakt upp. Stannar de i området märker de hur alla metallföremål de bär dras mot en viss punkt.\n"
			"\n"
			"EFFEKT: Zonfararna måste välja mellan att släppa alla metallföremål de bär på eller Ta krafttag för att lyckas ta sig ur kraftfältet med sakerna i behåll. Den som bär en skrotrustning måste Ta krafttag, det går inte att ta av sig rustningen snabbt nog. Den som misslyckas får slå igen i nästa runda, men får då en modifikation -1. Den negativa modifikationen växer ett steg för varje runda. Kraftfältet upphör plötsligt och oförklarligt när du finner det lämpligt, men återkommer senare på samma plats.");
		break;
	case 8:
		ImGui::TextWrapped(u8"MISÄRVIND ±0\n"
			"En vind fylld av pest, förruttnelse och röta sveper in över Zonen. Luften fylls av damm och otrevliga odörer och blir tung att andas. I spåren följer smittor, krubb ruttnar och vatten blir odrickbart.\n"
			"Den eländiga vinden håller i sig i flera timmar innan den avtar.\n"
			"\n"
			"EFFEKT: Vinden ger en rötpoäng direkt. Dessutom blir allt krubb och vatten besudlat. Det går fortfarande att äta/dricka, men ger en rötpoäng varje gång man gör så.");
		break;
	case 9:
		ImGui::TextWrapped(u8"NATTSKEN -1\n"
			"Flammande ljussken i sjukliga färger sveper över nattskyn. Artefakter som drivs på batterier och fornkraft fungerar erratiskt eller inte alls. En del ser undergångsomen, andra hopp om en ny gryning. Åter andra tycker bara färgerna är fina eller gömmer sig i rädsla. Zonskenet varar tills det ljusnar.\n"
			"\n"
			"EFFEKT: Alla elektriska artefakter (Strålkastare, Kassettradio, Videokamera) slutar fungera så länge nattskenet pågår.\n"
			"KOMMENTAR: Om du slår fram detta fenomen under dagtid, låt det inträffa följande kväll i stället.");
		break;
	case 10:
		ImGui::TextWrapped(u8"OBELISK ±0\n"
			"Mitt i Zonen står en flera meter hög, svart obelisk. Den verkar vara tillverkad i någon sorts mycket tålig metall. Den finns inga synliga öppningar eller luckor, och obelisken verkar helt oberörd av tidens tand. Obelisken ger ett främmande intryck och har märkliga psykiska effekter på personer i närheten, psykedeliska hallucinationer eller väsande röster i huvudet.\n"
			"\n"
			"EFFEKT: Varje runda nära obelisken måste offret slå för att Känna Zonen. Att misslyckas ger en poäng förvirring.");
		break;
	case 11:
		ImGui::TextWrapped(u8"RÖTHÄRD -1\n"
			"Kusten verkar klar. Ruinen är tom på fiender, och utforskningen kan börja obehindrat.\n"
			"Men den osynliga Rötan är stark här och kan döda oförsiktiga zonfarare.\n"
			"\n"
			"EFFEKT: Ge RP chansen att ana oråd genom att beskriva något avvikande i miljön; till exempel en samling kadaver, ett rör som läcker grön sörja, eller en plats där mörkret är onaturligt kompakt och påträngande. Den som trots allt vandrar in i härden tar en rötpoäng per minut (kapitel 8 i Regelboken).\n"
			"Om du vill ställa spelarna inför ett knivigt val kan den enda vägen framåt gå genom röthärden. Om RP anar oråd kan de Speja mot området för att få veta att det är en röthärd.");
		break;
	case 12:
		ImGui::TextWrapped(u8"SLUKHÅL ±0\n"
			"Den söndervittrande ruinen ger vika och en eller flera personer dras med i ett moln av grus och zondamm.\n"
			"\n"
			"EFFEKT: Alla som dras med i raset måste klara ett slag för att Fly för att undvika att ta en poäng skada (rustning skyddar).\n"
			"Men det är bara början på problemet. Offren för raset kan ha hamnat i någon forntida bunkar eller tunnel, fylld av monster eller röta. Att klättra upp kan också visa sig svårt.");
		break;
	case 13:
		ImGui::TextWrapped(u8"SPLITTERVIND ±0\n"
			"En hård blåst börjar vina. Luften fylls av virvlande korn av glas och metallsplitter som river upp oskyddad hud.\n"
			"\n"
			"EFFEKT: Alla som befinner sig ute i splittervinden utsätts för en attack med sex (eller ännu fler, om vinden är särskilt hård) grundtärningar. Varje RADIAK ger en poäng skada. Att hitta skydd kräver normalt att man lyckas Fly, alternativt använder en mutation med samma effekt.");
		break;
	case 14:
		ImGui::TextWrapped(u8"SPÖKSKEN ±0\n"
			"Ett blekt grönt ljussken spelar mellan ruinrester eller trädstammar. Om någon närmar sig drar skenen sig undan. Ingen förklaring står att finna men spökljusen tycks förflytta sig om en betraktare gör det. Sedan försvinner de lika hastigt i intet som de dök upp.\n"
			"\n"
			"EFFEKT: Ingen - men låt gärna spelarna tro annorlunda. Det finns inget rollpersonerna kan göra som påverkar spökskenet.");
		break;
	case 15:
		ImGui::TextWrapped(u8"SYNVILLA -1\n"
			"Området runt zonfararna tycks plötsligt hacka till. Det är som om allt skevar till ett ögonblick. Området byter skepnad och ser i en bråkdels sekund helt annorlunda ut: välordnat, städat, vackert. Zonfararna har vandrat in i ett hologram, resterna av ett urtida försvarsverk. Det är ett avancerat kamouflage som i sig är harmlöst men skapar en risk att rp går in i saker (ruiner, träd, betongblock, vrak) som förvrängts till att likna något annat.\n"
			"Området är cirka 20x20 meter stort.\n"
			"\n"
			"EFFEKT: Ingen - men effekten kan vara förvirrande.");
		break;
	case 16:
		ImGui::TextWrapped(u8"SYRAREGN +1\n"
			"Tunga droppar börjar plötsligt falla från zonhimlen. Men detta är inget vanligt regn. Dropparna fräser av syra när de slår ner, och den som befinner sig i syraskuren gör bäst i att snabbt söka skydd. Efter regnet är ruiner, betongblock, träd, blad och vrak pepprade med syrafrätta hål eller har fått djupa rännilar av strilande syravatten graverat. Pölar av frätande sörja och lera täcker marken. Vintertid blir regnet till tätt fallande syrasnö som bildar en tjock frätande snögegga. Snön dunstar inte utan försvagas över flera dygns tid.\n"
			"\n"
			"EFFEKT: Den som är ute i syraregnet löper stor risk att ta skada. Slå ett antal grundtärningar beroende på regnets intensitet - 6 är normalt, men allt från 4 till 10 är möjligt. Varje ger offret en poäng skada. Rustning skyddar på vanligt sätt (kapitel 6 i Regelboken).\n"
			"Offren fortsätter ta skada av regnet på samma sätt varje runda tills de lyckas söka skydd genom att Fly.");
		break;
	case 17:
		ImGui::TextWrapped(u8"TRÖGHETSFÄLT -2\n"
			"Zonfararna ser ett monster eller humanoid som tycks helt frusen mitt i en rörelse, totalt orörlig. Om någon närmar sig objektet kan andra se hur hans rörelser tycks bromsa in till slow motion. I området finns ett märkligt kraftfält som får själva tiden att bromsa in. Den som går in i fältet märker bara att omgivningen börjar röra sig snabbare, och effekten tilltar ju längre in personen går. Det går att ropa till en person i fältet, men han uppfattar röster utifrån som högfrekventa och pipiga. Hans egen röst låter mörk och långsam för personer utanför fältet.\n"
			"\n"
			"EFFEKT: Att befinna sig i fältets ytterkant medför ingen större risk, men den som närmar sig dess centrum kommer att bli fången i fältet för evigt. Låt RP som går in i fältet (eller bara ser effekten av det på någon annan) slå för att Känna Zonen. Om slaget lyckas ska du förklara risken för spelarna.");
		break;
	case 18:
		ImGui::TextWrapped(u8"VAKUUM -2\n"
			"Zonfararna passerar ett område som är helt tomt på luft. Det finns ingen uppenbar förklaring till varför luften inte fyller området - inga tydliga markeringar eller gränser syns. Det första zonfararna märker är att de inte kan andas eller prata.\n"
			"\n"
			"EFFEKT: Alla måste slå för att fly. Den som misslyckas får panik och blir kvar i vakuumet, vilket ger en poäng skada varje runda. Rustning skyddar inte. Ett slag per runda är tillåtet för att ta sig ut ur vakuumet.\n"
			"Den som tagit sig ut kan hjälpa (sid 54 i Regelboken) ut en vän som blivit kvar.");
		break;
	case 19:
		ImGui::TextWrapped(u8"ZONGÖLAR ±0\n"
			"En ruintrakt där varm och tjockflytande lera täcker alla öppna ytor. Solid mark finns under geggan och en normallång mutant sjunker ned till knäet. Här och var finns meterdjupa kratrar, skapade av områdets zonfenomen. Under ytan döljs lömska källor med hett vatten. Om någon rör sig i närheten eller rakt över dem kommer de brisera och en kaskad av skållande vatten sprutar upp med oerhörd kraft. Vattenstrålen når ett tiotal meter i höjd och dränker allt inom några meter. En person som träffas riskerar att få sin hud bortskållad.\n"
			"\n"
			"EFFEKT: Den som Spejar framgångsrikt mot gölarna noterar att det pyser och bubblar på vissa platser. För att röra sig genom området och undvika dessa platser krävs att man klarar ett slag för Känna Zonen. Ett misslyckat slag betyder att offret blir måltavla för en skållhet kaskad. Samma sak gäller den som går in i området ovetande. Slå ett antal grundtärningar beroende på kaskadens intensitet, sex är normalt. Slår du en eller fler RADIAK träffas offret, och tar skada lika med antalet RADIAK. Den som tar en eller flera poäng i skada tar också en rötpoäng. Den enda fördelen med denna plats är att den alla i närheten skyddas mot köld.");
		break;
	case 20:
		ImGui::TextWrapped(u8"ZONSMOG +1\n"
			"Stråk av smog börjar virvla upp från marken. Först i knähöjd men sedan stiger dimman allt mer. Ögon börjar tåras och det bränner i näsa och mun samt sticker otrevligt över huden. Man riskerar ingen direkt skada men har mycket svårt att se och lukt- och smaksinne är helt borta i flera timmar framåt. Bar hud kommer vara röd och smått svullen någon dag efter. Smogmolnet ligger tätt resten av dagen över området. Därtill har zonbestar med förmåga att jaga obehindrat i smogen lärt sig utnyttja dimmans effekt. Tar sig zonfararna till högre höjd (ruin, trädtopp, kulle) undkommer de smogen.\n"
			"\n"
			"EFFEKT: Det går inte att Skjuta på kort eller långt håll i smoggen. Alla som hamnar i smoggen tar också en rötpoäng.\n"
			"Välj eller slå fram ett zonmonster som jagar i smoggen.");
		break;
	case 21:
		ImGui::TextWrapped(u8"Luften blir allt varmare och på en del platser väller ånga fram ur sprickor i marken. Små pölar med bubblande och illaluktande vatten finns också. Vattnet är kokande hett men här och var blandas det med regnpölar och bäckar och tycks behagligt varmt.\n"
			"\n"
			"EFFEKT: Ett uråldrigt kraftverk långt under ytan är fortfarande aktivt. Det läcker röta och vattnet är smittat. Den som vadar genom vattnet tar en rötpoäng.\n"
			"Fördelen är att källan skyddar mot köld (se Regelboken) så länge man håller sig i närheten.");
		break;
	}
}