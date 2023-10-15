#pragma once
#include <Windows.h>

enum class TypesOfTargets {
	Quadrate, Rhomb
};

double randomDouble() {
	return (double)(rand()) / RAND_MAX*(1 - 0) + 0;
}

int randomInt(int min, int max) {
	return rand() % (max - min + 1) + min;
}

class Target {
public:
	virtual void generateTargetNear(int glare) = 0;
	virtual void generateTargetAfar(int glare) = 0;
};

class QuadrateTarget : public Target {
public:
	QuadrateTarget(int width) : width{width} {
		this->arr_near = new char* [this->width];
		this->new_width = width + 4;
		this->arr_afar = new char* [this->new_width];

		for (size_t i = 0; i < this->width; i++) {
			this->arr_near[i] = new char[this->width];
		}

		for (size_t i = 0; i < this->new_width; i++) {
			this->arr_afar[i] = new char[this->new_width];
		}

		type = "quadrate";
	}

	~QuadrateTarget() {
		for (size_t i = 0; i < this->width; i++) {
			delete[] this->arr_near[i];
		}

		for (size_t i = 0; i < this->new_width; i++) {
			delete[] this->arr_afar[i];
		}

		delete[] this->arr_afar;
		delete[] this->arr_near;
	}
	
	void generateTargetNear(int glare) override {
		for (size_t i = 0; i < this->width; i++) {
			for (size_t j = 0; j < this->width; j++) {
				if (j == this->width - 1) {
					this->arr_near[i][j] = 49;
				}
				else if (j == 0) {
					this->arr_near[i][j] = 49;
				}
				else if (i == 0 && j < this->width - 1) {
					this->arr_near[i][j] = 49;
				}
				else if (i == this->width - 1 && j < this->width - 1) {
					this->arr_near[i][j] = 49;
				}
				else {
					if (i == 2 && j >= 2 && j <= 7) {
						this->arr_near[i][j] = 53;
					}
					else if (j == 2 && i >= 2 && i <= 7) {
						this->arr_near[i][j] = 53;
					}
					else if (i == 7 && j >= 2 && j <= 7) {
						this->arr_near[i][j] = 53;
					}
					else if (j == 7 && i >= 2 && i <= 7) {
						this->arr_near[i][j] = 53;
					}
					else {
						if (i == 4 && j >= 4 && j <= 5) {
							this->arr_near[i][j] = 55;
						}
						else if (i == 5 && j >= 4 && j <= 5) {
							this->arr_near[i][j] = 55;
						}
						else {
							this->arr_near[i][j] = 96;
						}
					}
				}

				if (glare == 1) {
					if (j > i && j + 2 < this->width - 1) {
						this->arr_near[i][j] = 254;
					}
				}
			}
		}
	}
	
	void generateTargetAfar(int glare) override {
		generateTargetNear(glare);
		
		for (size_t i = 0; i < this->new_width; i++) {
			for (size_t j = 0; j < this->new_width; j++) {
				this->arr_afar[i][j] = 34; // символ, показывающий, что мишень далеко
			}
		}

		for (size_t i = 0; i < this->width; i++) {
			for (size_t j = 0; j < this->width; j++) {
				this->arr_afar[i + 2][j + 2] = this->arr_near[i][j];
				if (glare == 1) {
					if (j > i && j + 2 < this->width - 1) {
						this->arr_afar[i][j] = 254;
					}
				}
			}
		}
	}

	void printNear(int glare) {
		std::cout << "Distance: " << this->short_distance << std::endl;
		std::cout << "Square of the target: " << squareOfTarget(this->width) << std::endl;
		std::cout << "Target: " + this->type << std::endl;

		if (glare == 1) {
			std::cout << "Glare: yes" << std::endl;
			print(this->arr_near, this->width);
		}
		else {
			std::cout << "Glare: no" << std::endl;
			print(this->arr_near, this->width);
		}

		std::cout << "Visibility: " << visibility(this->width, this->width, glare, this->short_distance, this->arr_near) << "%\n" << std::endl;
	}

	void printAfar(int glare) {
		std::cout << "Distance: " << this->long_distance << std::endl;
		std::cout << "Square of the target: " << squareOfTarget(this->new_width) << std::endl;
		std::cout << "Target: " + this->type << std::endl;

		if (glare == 1) {
			std::cout << "Glare: yes" << std::endl;
			print(this->arr_afar, this->new_width);
		}
		else {
			std::cout << "Glare: no" << std::endl;
			print(this->arr_afar, this->new_width);
		}

		std::cout << "Visibility: " << visibility(this->width, this->new_width, glare, this->long_distance, this->arr_afar) << "%\n" << std::endl;
	}

	void printTargetAfterShooting(int distance) {
		if (distance == 0)
			print(this->arr_near, this->width);
		else
			print(this->arr_afar, this->new_width);
	}

	void generateTargetAfterShooting(int distance) {
		if (distance == 0)
			this->arr_near = changeTargetAfterShooting(this->arr_near, distance);
		else
			this->arr_afar = changeTargetAfterShooting(this->arr_afar, distance);
	}

	double generateChance(int glare, int distance_mode, int moving_mode) {
		double chance = randomDouble();

		if (glare)
			chance -= 0.10;
		if (distance_mode)
			chance -= 0.10;
		if (this->type == "quadrate")
			chance -= 0.05;
		if (moving_mode)
			chance -= 0.10;
		
		return chance < 0 ? 0.01 : chance;
	}

	int getDamage(int hits) {
		int* total_health = &this->max_hits;
		*total_health -= hits;
		return *total_health;
	}

	int accessShooting() {
		return this->max_hits <= 0 ? 0 : 1;
	}

	int checkHealth() {
		return this->max_hits;
	}

private:
	int max_hits = 70; // target's max hits
	int width = 0;
	int new_width = 0;
	char** arr_near = 0;
	char** arr_afar = 0;

	int long_distance = 100;
	int short_distance = 50;

	std::string type = "";

	void print(char** arr, int width) {
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < width; j++) {
				if (j == width - 1) {
					if (arr[i][j] == 'X') {
						std::cout << "\033[31m" << arr[i][j] << "\033[0m";
					}
					else {
						std::cout << arr[i][j];
					}
				}
				else {
					if (arr[i][j] == 'X') {
						std::cout << "\033[31m" << arr[i][j] << "\033[0m" << " ";
					}
					else {
						std::cout << arr[i][j] << " ";
					}
				}
			}
			std::cout << std::endl;
		}
	}

	double squareOfTarget(double a) {
		return a * a;
	}

	double visibility(int width, int width_area, int glare, int distance, char** arr) {
		double count_glare = 0.0;
		double visible_area = (squareOfTarget(width) / squareOfTarget(width_area)) * 100;
		
		if (glare == 1) {
			for (size_t i = 0; i < width_area; i++) {
				for (size_t j = 0; j < width_area; j++) {
					if (arr[i][j] == (char)254) {
						count_glare += 0.1;
					}
				}
			}
		}

		visible_area -= count_glare;
		visible_area -= distance / 10;
 
		return visible_area <= 0 ? 0 : visible_area;
	}

	char** changeTargetAfterShooting(char** arr, int distance) {
		int x = 0;
		int y = 0;

		if (distance == 0) {
			x = randomInt(0, this->width - 1);
			y = randomInt(0, this->width - 1);
		}
		else {
			x = randomInt(2, this->new_width - 3);
			y = randomInt(2, this->new_width - 3);
		}

		// если попал в засвеченную зону - ничего не делаем
		if (arr[x][y] != (char)254) {
			arr[x][y] = 'X';
		} 
		
		return arr;
	}
};

class RhombTarget : public Target {
public:
	RhombTarget(int width) : width{ width } {
		this->arr_near = new char* [this->width];
		this->new_width = width + 4;
		this->arr_afar = new char* [new_width];

		for (size_t i = 0; i < this->width; i++) {
			this->arr_near[i] = new char[this->width];
		}

		for (size_t i = 0; i < this->new_width; i++) {
			this->arr_afar[i] = new char[this->new_width];
		}

		this->type = "rhomb";
	}

	~RhombTarget() {
		for (size_t i = 0; i < this->width; i++) {
			delete[] this->arr_near[i];
		}

		for (size_t i = 0; i < this->new_width; i++) {
			delete[] this->arr_afar[i];
		}

		delete[] this->arr_afar;
		delete[] this->arr_near;
	}

	void generateTargetNear(int glare) override {
		int center = this->width / 2;

		for (size_t i = 0; i < this->width; i++) {
			for (size_t j = 0; j < this->width; j++) {
				if (i <= center) {
					if (j == center - i || j == center + i) {
						this->arr_near[i][j] = 49;
					}
					else if (j > center - i && j <= center) {
						this->arr_near[i][j] = 96;
						if (i == 3 && (j >= center - (i - 3) && j == center)) {
							this->arr_near[i][j] = 53;
						}
						if (i > 3 && j == center - (i - 3)) {
							this->arr_near[i][j] = 53;
						}
						if (i == center && j == center) {
							this->arr_near[i][j] = 55;
						}
					}
					else if (j >= center && j < center + i) {
						this->arr_near[i][j] = 96;
						if (i == 3 && (j >= center - (i - 3) && j == center)) {
							this->arr_near[i][j] = 53;
						}
						if (i > 3 && j == center + (i - 3)) {
							this->arr_near[i][j] = 53;
						}
					}
					else {
						this->arr_near[i][j] = 34;
					}
				}
				else {
					if (j == i - center || j == this->width - (i - center + 1)) {
						this->arr_near[i][j] = 49;
					}
					else if (j > i - center && j <= center) {
						this->arr_near[i][j] = 96;
						if (i == 6 && (j >= center - 1 && j < center)) {
							this->arr_near[i][j] = 53;
						}
						if (i == 7 && j == center) {
							this->arr_near[i][j] = 53;
						}
					}
					else if (j >= center && j < this->width - (i - center + 1)) {
						this->arr_near[i][j] = 96;
						if (i == 6 && j == center + 1) {
							this->arr_near[i][j] = 53;
						}
					}
					else {
						this->arr_near[i][j] = 34;
					}
				}

				if (glare == 1) {
					if (j > i && j + 2 < this->width - 1) {
						this->arr_near[i][j] = 254;
					}
				}
			}
		}
	}

	void generateTargetAfar(int glare) override {
		generateTargetNear(glare);

		for (size_t i = 0; i < this->new_width; i++) {
			for (size_t j = 0; j < this->new_width; j++) {
				this->arr_afar[i][j] = 34; // символ, показывающий, что мишень далеко
			}
		}

		for (size_t i = 0; i < this->width; i++) {
			for (size_t j = 0; j < this->width; j++) {
				this->arr_afar[i + 2][j + 2] = this->arr_near[i][j];
				if (glare == 1) {
					if (j > i && j + 2 < this->width - 1) {
						this->arr_afar[i][j] = 254;
					}
				}
			}
		}
	}

	void printNear(int glare) {
		std::cout << "Distance: " << this->short_distance << std::endl;
		std::cout << "Square of the target: " << squareOfTarget(this->width) << std::endl;
		std::cout << "Target: " + this->type << std::endl;

		if (glare == 1) {
			std::cout << "Glare: yes" << std::endl;
			print(this->arr_near, this->width);
		}
		else {
			std::cout << "Glare: no" << std::endl;
			print(this->arr_near, this->width);
		}

		std::cout << "Visibility: " << visibility(this->width, this->width, glare, this->short_distance, this->arr_near) << "%\n" << std::endl;
	}

	void printAfar(int glare) {
		std::cout << "Distance: " << this->long_distance << std::endl;
		std::cout << "Square of the target: " << squareOfTarget(this->new_width) << std::endl;
		std::cout << "Target: " + this->type << std::endl;

		if (glare == 1) {
			std::cout << "Glare: yes" << std::endl;
			print(this->arr_afar, this->new_width);
		}
		else {
			std::cout << "Glare: no" << std::endl;
			print(this->arr_afar, this->new_width);
		}

		std::cout << "Visibility: " << visibility(this->width, this->new_width, glare, this->long_distance, this->arr_afar) << "%\n" << std::endl;
	}

	void printTargetAfterShooting(int distance) {
		if (distance == 0)
			print(this->arr_near, this->width);
		else
			print(this->arr_afar, this->new_width);
	}

	void generateTargetAfterShooting(int distance) {
		if (distance == 0)
			this->arr_near = changeTargetAfterShooting(this->arr_near);
		else {
			this->arr_near = changeTargetAfterShooting(this->arr_near);

			for (size_t i = 0; i < this->width; i++) {
				for (size_t j = 0; j < this->width; j++) {
					this->arr_afar[i + 2][j + 2] = this->arr_near[i][j];
				}
			}
		}
	}

	double generateChance(int glare, int distance_mode, int moving_mode) {
		double chance = randomDouble();

		if (glare)
			chance -= 0.10;
		if (distance_mode)
			chance -= 0.10;
		if (this->type == "rhomb")
			chance -= 0.10;
		if (moving_mode)
			chance -= 0.10;

		return chance < 0 ? 0.01 : chance;
	}

	int getDamage(int hits) {
		int* total_health = &this->max_hits;
		*total_health -= hits;
		return *total_health;
	}

	int accessShooting() {
		return this->max_hits <= 0 ? 0 : 1;
	}

	int checkHealth() {
		return this->max_hits;
	}

private:
	int max_hits = 70; // target's max hits
	int width = 0;
	int new_width = 0;
	char** arr_near = 0;
	char** arr_afar = 0;

	int long_distance = 100;
	int short_distance = 50;

	std::string type = "";

	void print(char** arr, int width) {
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < width; j++) {
				if (j == width - 1) {
					if (arr[i][j] == 'X') {
						std::cout << "\033[31m" << arr[i][j] << "\033[0m";
					}
					else {
						std::cout << arr[i][j];
					}
				}
				else {
					if (arr[i][j] == 'X') {
						std::cout << "\033[31m" << arr[i][j] << "\033[0m" << " ";
					}
					else {
						std::cout << arr[i][j] << " ";
					}
				}
			}
			std::cout << std::endl;
		}
	}

	double squareOfTarget(double a) {
		return a * a;
	}

	double visibility(int width, int width_area, int glare, int distance, char** arr) {
		double count_glare = 0.0;
		double visible_area = (squareOfTarget(width) / squareOfTarget(width_area)) * 100;

		if (glare == 1) {
			for (size_t i = 0; i < width_area; i++) {
				for (size_t j = 0; j < width_area; j++) {
					if (arr[i][j] == (char)254) {
						count_glare += 0.1;
					}
				}
			}
		}

		visible_area -= count_glare;
		visible_area -= distance / 10;

		return visible_area <= 0 ? 0 : visible_area;
	}

	char** changeTargetAfterShooting(char** arr) {
		int x = 0;
		int y = 0;

		while (true) {
			int center = this->width / 2;
			x = randomInt(0, this->width - 1);
			y = randomInt(0, this->width - 1);
				
			if (x <= center) {
				if (y >= center - x && y <= center + x)
					break;
			}
			else if (x >= center) {
				if (y >= center + x - this->width + 1 && y <= center - x + this->width - 1)
					break;
			}
			else
				continue;
		}

		// если попал в засвеченную зону - ничего не делаем
		if (arr[x][y] != (char)254) {
			arr[x][y] = 'X';
		}

		return arr;
	}
};

class Gun {
public:
	Gun(int shooting_mode, int count_magazines) {
		this->shooting_mode = shooting_mode;
		this->magazine *= count_magazines;
	}

	int bang(int glare, int distance_mode, int moving_mode) {
		int* total_magazine = &this->magazine;
		double target_chance = this->type_of_target ? (*(RhombTarget*)this->rhomb).generateChance(glare, distance_mode, moving_mode) : (*(QuadrateTarget*)this->quadrate).generateChance(glare, distance_mode, moving_mode);
		double bang_chance = randomDouble();
		this->gun_chance = bang_chance;
		double mode_chance = randomDouble();

		// если авт.режим стрельбы, но патронов не хватит на очередь из 3 выстрелов
		if (this->shooting_mode == 1 && *total_magazine - 3 <= 0 && bang_chance > target_chance) {
			*total_magazine -= *total_magazine;
			return 0;
		}

		if (mode_chance >= 0.9) {
			std::cout << "O-o-ops! Shooting mode has changed!" << std::endl;
			this->shooting_mode = this->shooting_mode ? 0 : 1;
		}

		if (this->magazine > 0 && this->shooting_mode == 1 && bang_chance < target_chance) {
			*total_magazine -= 3;

			total_damage += 3 * 3;
			return 3 * 3; // урон от всех 3х патрон
		}
		else if (this->magazine > 0 && this->shooting_mode == 0 && bang_chance < target_chance) {
			*total_magazine -= 1;

			total_damage += 1;
			return 1;
		}
		else if (this->magazine > 0 && bang_chance > target_chance) {
			*total_magazine -= this->shooting_mode ? 3 : 1;

			return 0;
		}
		else {
			return 0;
		}
	}

	int checkMagazine() {
		return this->magazine;
	}

	double getGunChance() {
		return this->gun_chance;
	}

	void selectTarget(void* target, int type_of_target) {
		this->type_of_target = type_of_target;

		if (this->type_of_target == int(TypesOfTargets::Quadrate))
			this->quadrate = &(*(QuadrateTarget*)target);
		else
			this->rhomb = &(*(RhombTarget*)target);
	}

	void resetMagazine() {
		this->magazine = 0;
	}

	int getTotalDamage() {
		return total_damage;
	}

private:
	void* quadrate = 0;
	void* rhomb = 0;
	int type_of_target = 0;
	int shooting_mode = 0;
	int magazine = 30;
	double gun_chance = 0.0;
	int total_damage = 0;
};

class ShootingRange {
public:
	ShootingRange(void* gun, void* target, int type_of_target, int distance_mode, int glare, int moving_mode, int* seconds) {
		this->gun = gun;
		this->type_of_target = type_of_target;

		if (this->type_of_target == (int)TypesOfTargets::Quadrate)
			this->quadrate = &(*(QuadrateTarget*)target);
		else
			this->rhomb = &(*(RhombTarget*)target);

		this->distance_mode = distance_mode;
		this->moving_mode = moving_mode;
		this->glare = glare;
		this->seconds = seconds;
	}

	void rangeQuadrate() {
		while ((*(QuadrateTarget*)this->quadrate).accessShooting() && (*(Gun*)this->gun).checkMagazine()) {
			(*(QuadrateTarget*)this->quadrate).getDamage((*(Gun*)gun).bang(this->glare, this->distance_mode, this->moving_mode));

			if ((*(QuadrateTarget*)this->quadrate).generateChance(this->glare, this->distance_mode, this->moving_mode) < (*(Gun*)gun).getGunChance()) {
				(*(QuadrateTarget*)this->quadrate).generateTargetAfterShooting(this->distance_mode);
			}

			if (!(*(QuadrateTarget*)this->quadrate).accessShooting() || !(*(Gun*)this->gun).checkMagazine()) {
				(*(QuadrateTarget*)this->quadrate).printTargetAfterShooting(this->distance_mode);
				std::cout << "Magazine: " << (*(Gun*)this->gun).checkMagazine() << std::endl;
				std::cout << "Quadrate target health: " << (*(QuadrateTarget*)this->quadrate).checkHealth() << std::endl;
			}

			*this->seconds -= 1;
		}
	}

	void rangeRhomb() {
		 while ((*(RhombTarget*)this->rhomb).accessShooting() && (*(Gun*)this->gun).checkMagazine()) {
			 
			 (*(RhombTarget*)this->rhomb).getDamage((*(Gun*)this->gun).bang(this->glare, this->distance_mode, this->moving_mode));

			 if ((*(RhombTarget*)this->rhomb).generateChance(this->glare, this->distance_mode, this->moving_mode) < (*(Gun*)gun).getGunChance()) {
				 (*(RhombTarget*)this->rhomb).generateTargetAfterShooting(this->distance_mode);
			 }

			 if (!(*(RhombTarget*)this->rhomb).accessShooting() || !(*(Gun*)this->gun).checkMagazine()) {
				 (*(RhombTarget*)this->rhomb).printTargetAfterShooting(this->distance_mode);
				 std::cout << "Magazine: " << (*(Gun*)this->gun).checkMagazine() << std::endl;
				 std::cout << "Rhomb target health: " << (*(RhombTarget*)this->rhomb).checkHealth() << std::endl;
			 }

			 *this->seconds -= 1;
		 }
	 }

private:
	void* gun = 0;
	void* quadrate = 0;
	void* rhomb = 0;
	int type_of_target = 0;
	int distance_mode = 0;
	int moving_mode = 0;
	int glare = 0;
	int* seconds = 0;
	int seconds_flag = 0;
};
