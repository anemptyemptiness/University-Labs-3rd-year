#pragma once
#include "Wizard.h"
#include "Weapons.h"

class Mercenary {
public:
	std::string info_str = "";

	Mercenary(void* type_of_warrior, void* weapon) {
		info_str = (*(Wizard*)type_of_warrior).type + "_"
			+ (*(Wizard*)type_of_warrior).str_index_of_unit.str() 
			+ "\n" + (*(Wizard*)type_of_warrior).info()
			+ "Weapon:\nType: " + (*(Magic*)weapon).type 
			+ "\nDamage: " + (*(Magic*)weapon).str_damage.str();
	}
};
