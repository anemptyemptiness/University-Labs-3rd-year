#pragma once
#include <sstream>

class Swordsman {
public:
	Swordsman(int durability, std::string distance, int price, int index) {
		this->distance = distance;
		str_index_of_unit << index;
		str_durability << durability;
		str_price << price;
	}

	std::string info() {
		return "Unit: " + type + "\nDurability: " + str_durability.str() + "\nDistance: " + this->distance + "\nPrice: " + str_price.str() + "\n";
	}

	std::string type = "Swordsman";
	std::stringstream str_index_of_unit;
	std::string distance;
	std::stringstream str_durability;
	std::stringstream str_price;
};
