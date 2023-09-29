#pragma once
#include <vector>
#include "Mercenary.h"

class Guild {
public:
	void printUnits() {
		for (Mercenary element : mercenaries) {
			std::cout << element.info_str << "\n\n";
		}
	}

    std::vector<Mercenary> mercenaries;
};