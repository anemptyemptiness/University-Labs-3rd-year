#include <iostream>
#include "ShootingRange.h"

const int SIZE_QUADRATE_TARGET = 10;
const int SIZE_RHOMB_TARGET = 11;

void distanceShootingQuadrate(int mode, int glare, QuadrateTarget* quadrate);
void distanceShootingRhomb(int distance_mode, int glare, RhombTarget* rhomb);

int main()
{
	srand(time(NULL));

	int glare = 0; // 1 0
	int distance_mode = 1; // 1 0
	int shooting_mode = 1; // 1 0
	int moving_mode = 0; // 1 0
	int count_magazines = 10; // any
	int count_targets = 4; //randomInt(1, 3);

	while (true) {
		std::cout << "Welcome to the shooting range! You will shoot in 60 seconds\nSome questions...\n" << std::endl;
		std::cout << "What distance? (1/0): ";
		std::cin >> distance_mode;
		std::cout << "Shooting mode? (3 patrons/1 patron): ";
		std::cin >> shooting_mode;
		std::cout << "Moving mode? (1/0): ";
		std::cin >> moving_mode;
		std::cout << "Glares mode? (1/0): ";
		std::cin >> glare;
		std::cout << "Enter count of targets (1-any): ";
		std::cin >> count_targets;
		std::cout << "How many magazines you want? (1-any): ";
		std::cin >> count_magazines;
		std::cout << std::endl;

		if ((distance_mode != 1 && distance_mode != 0) ||
			(shooting_mode != 3 && shooting_mode != 1) ||
			(moving_mode != 1 && moving_mode != 0) ||
			(glare != 1 && glare != 0) ||
			count_targets <= 0 ||
			count_magazines <= 0) {
			std::cout << "Some argument(-s) went wrong! Try again...\n" << std::endl;
			continue;
		}

		int count_destroyed_targets = 0;
		int seconds = 60;
		Gun M4A4{ shooting_mode, count_magazines };
		for (int i = 0; i < count_targets; i++) {
			int random_target = randomInt((int)TypesOfTargets::Quadrate, (int)TypesOfTargets::Rhomb);
			if (random_target == (int)TypesOfTargets::Quadrate) {
				std::cout << "\nTarget #" << i + 1 << std::endl;
				QuadrateTarget quadrate{ SIZE_QUADRATE_TARGET };
				distanceShootingQuadrate(distance_mode, glare, &quadrate);

				void* p_quadrate = &quadrate;
				M4A4.selectTarget(p_quadrate, random_target);

				ShootingRange shooting_range{ &M4A4, p_quadrate, random_target, distance_mode, glare, moving_mode, &seconds };
				shooting_range.rangeQuadrate();

				if (quadrate.checkHealth() <= 0) {
					count_destroyed_targets += 1;
					std::cout << "Target was destroyed!" << std::endl;
				}

				if (M4A4.checkMagazine() <= 0) {
					M4A4.resetMagazine();
					std::cout << "No ammo!" << std::endl;
				}

				if (seconds <= 0) {
					i = count_targets - 1;
					std::cout << "WARNING: Timeout!" << std::endl;
				}
			}
			else {
				std::cout << "\nTarget #" << i + 1 << std::endl;
				RhombTarget rhomb{ SIZE_RHOMB_TARGET };
				distanceShootingRhomb(distance_mode, glare, &rhomb);

				void* p_rhomb = &rhomb;
				M4A4.selectTarget(p_rhomb, random_target);

				ShootingRange shooting_range{ &M4A4, p_rhomb, random_target, distance_mode, glare, moving_mode, &seconds };
				shooting_range.rangeRhomb();

				if (M4A4.checkMagazine() <= 0) {
					M4A4.resetMagazine();
					std::cout << "No ammo!" << std::endl;
				}

				if (seconds <= 0) {
					i = count_targets - 1;
					std::cout << "WARNING: Timeout!" << std::endl;
				}

				if (rhomb.checkHealth() <= 0) {
					count_destroyed_targets += 1;
					std::cout << "Target was destroyed!" << std::endl;
				}
			}
		}

		std::cout << "\nResults:\nAlive targets: " << count_targets - count_destroyed_targets << std::endl;
		
		int average_damage = 0;
		if (count_destroyed_targets > 0) {
			std::cout << "Total average damage: " << M4A4.getTotalDamage() / count_destroyed_targets << std::endl;
		}
		else
			std::cout << "Total average damage: " << M4A4.getTotalDamage() << std::endl;
		
		std::cout << "Total seconds: " << 60 + (seconds * -1) << "\n\n" << std::endl;
	}

	return 0;
}

/*
 * Отрисовка квадратной мишени
 *
 * @param distance_mode - дистанция стрельбы
 * @param glare - режим засвета
 * @quadrate - тип мишени
 */
void distanceShootingQuadrate(int distance_mode, int glare, QuadrateTarget* quadrate) {
	if (distance_mode == 0) {
		if (glare == 1) {
			quadrate->generateTargetNear(glare);
			quadrate->printNear(glare);
		}
		else {
			quadrate->generateTargetNear(glare);
			quadrate->printNear(glare);
		}
	}
	else {
		if (glare == 1) {
			quadrate->generateTargetAfar(glare);
			quadrate->printAfar(glare);
		}
		else {
			quadrate->generateTargetAfar(glare);
			quadrate->printAfar(glare);
		}
	}
}

/*
 * Отрисовка ромбовидной мишени
 *
 * @param distance_mode - дистанция стрельбы
 * @param glare - режим засвета
 * @rhomb - тип мишени
 */
void distanceShootingRhomb(int distance_mode, int glare, RhombTarget* rhomb) {
	if (distance_mode == 0) {
		if (glare == 1) {
			rhomb->generateTargetNear(glare);
			rhomb->printNear(glare);
		}
		else {
			rhomb->generateTargetNear(glare);
			rhomb->printNear(glare);
		}
	}
	else {
		if (glare == 1) {
			rhomb->generateTargetAfar(glare);
			rhomb->printAfar(glare);
		}
		else {
			rhomb->generateTargetAfar(glare);
			rhomb->printAfar(glare);
		}
	}
}
