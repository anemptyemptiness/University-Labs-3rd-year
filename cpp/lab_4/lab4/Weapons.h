#pragma once
#include <sstream>

std::string wizard_magic_type[3] = { "Fire", "Electric", "Water" };
int randomInt(int min, int max);

class Weapon {
public:
	Weapon(int damage) {
		str_damage << damage;
	}

	std::stringstream str_damage;
};

class Sword : public Weapon {
public:
	Sword(int damage): Weapon(damage) { }
	std::string type = "Sword";
};

class Bow : public Weapon {
public:
	Bow(int damage): Weapon(damage) { }
	std::string type = "Bow";
};

class Magic : public Weapon {
public:
	Magic(int damage): Weapon(damage) {	}
	std::string type = wizard_magic_type[randomInt(0, 2)];
};
