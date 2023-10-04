#pragma once
#include <iostream>
#include <vector>

class SparePart {
public:
	int price = 0; // стоимость детали/стоимость замены
	int price_for_once = 0; // стоимость однократной починки
	int time_to_repair = 0; // время, требуемое на починку
	int service_life = 0; // срок службы до полной починки
	int degree_of_wear = 0; // степень износа в %
	int crash_due_wear = 0; // поломка от износа
	int crash_due_year = 0; // поломка по макс.сроку работы
	int detail_downtime = 0; // простой детали

	virtual void detection_method() {} // метод определения поломки

	virtual void work_method(int intensity) {} // метод работы
};

class Shaft : public SparePart { // деталь Вал
public:
	void detection_method() override {
		if (degree_of_wear == 100) {
			crash_due_wear++;
		}
	}

	void work_method(int intensity) override {
		if (intensity == 1)
			degree_of_wear += 5;
		else if (intensity == 0)
			degree_of_wear += 1;
	}
};

class ElectricMotor : public SparePart {
public:
	void detection_method() override {
		if (degree_of_wear == 1000) {
			crash_due_wear++;
		}
	}

	void work_method(int intensity) override {
		if (intensity == 1)
			degree_of_wear += 10;
		else if (intensity == 0)
			degree_of_wear += 1;
	}
};

class ControlPanel : public SparePart {
public:
	void detection_method() override {
		if (degree_of_wear == 500) {
			crash_due_wear++;
		}
	}

	void work_method(int intensity) override {
		if (intensity == 1)
			degree_of_wear += 5;
		else if (intensity == 0)
			degree_of_wear += 1;
	}
};

class AmNyam : public SparePart { // деталь режущая головка
public:
	void detection_method() override {
		if (degree_of_wear == 300) {
			crash_due_wear++;
		}
	}

	void work_method(int intensity) override {
		if (intensity == 1)
			degree_of_wear += 5;
		else if (intensity == 0)
			degree_of_wear += 1;
	}
};

class Machine : public Shaft, public ElectricMotor, public ControlPanel, public AmNyam {
private:
	Shaft shaft = Shaft();
	ElectricMotor motor = ElectricMotor();
	ControlPanel panel = ControlPanel();
	AmNyam amnyam = AmNyam();

public:
	std::vector<SparePart*> details = { &shaft, &motor, &panel, &amnyam };

	void generateShaft() {
		(*details[0]).price = 10000;
		(*details[0]).price_for_once = 100;
		(*details[0]).time_to_repair = 2;
		(*details[0]).service_life = 1;
		(*details[0]).degree_of_wear = 0;
		(*details[0]).crash_due_wear = 0;
		(*details[0]).crash_due_year = 0;
		(*details[0]).detail_downtime = 0;
	}

	void generateMotor() {
		(*details[1]).price = 1500000;
		(*details[1]).price_for_once = 15000;
		(*details[1]).time_to_repair = 120;
		(*details[1]).service_life = 3;
		(*details[1]).degree_of_wear = 0;
		(*details[0]).crash_due_wear = 0;
		(*details[0]).crash_due_year = 0;
		(*details[0]).detail_downtime = 0;
	}
	
	void generatePanel() {
		(*details[2]).price = 250000;
		(*details[2]).price_for_once = 2500;
		(*details[2]).time_to_repair = 12;
		(*details[2]).service_life = 1;
		(*details[2]).degree_of_wear = 0;
		(*details[0]).crash_due_wear = 0;
		(*details[0]).crash_due_year = 0;
		(*details[0]).detail_downtime = 0;
	}
	
	void generateNyam() {
		(*details[3]).price = 10000;
		(*details[3]).price_for_once = 200;
		(*details[3]).time_to_repair = 5;
		(*details[3]).service_life = 1;
		(*details[3]).degree_of_wear = 0;
		(*details[0]).crash_due_wear = 0;
		(*details[0]).crash_due_year = 0;
		(*details[0]).detail_downtime = 0;
	}
};