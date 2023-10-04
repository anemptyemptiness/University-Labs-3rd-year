#include <iostream>
#include "Workshop.h"

enum class eIntensity {
    Normal, Hard
};

int randomInt(int min, int max);
void firstMachineInPeriod(int period_in_hour, Machine* machine, int* summary_downtime_machine);
void secondMachineInPeriod(int period_in_year, Machine* machine, int* summary_downtime_machine);
void generateMachine(Machine* machine);
void shaftMechanism(Machine* machine, int* downtime_shaft, int month_intensity, int* flag, int* year, int years, int* is_first_year);
void motorMechanism(Machine* machine, int* downtime_motor, int month_intensity, int* flag, int* year, int years, int* is_first_year);
void panelMechanism(Machine* machine, int* downtime_panel, int month_intensity, int* flag, int* year, int years, int* is_first_year);
void amnyamMechanism(Machine* machine, int* downtime_amnyam, int month_intensity, int* flag, int* year, int years, int* is_first_year);

int main()
{
    int summary_downtime_machine_1 = 0;
    int summary_downtime_machine_2 = 0;
    int years = 0;
    
    while (true) {
        Machine machine = Machine();
        generateMachine(&machine);

        try {
            std::cout << "How long will the machines work? Enter count of years (max. 10): ";
            std::cin >> years;

            if (years <= 0 || years > 10) {
                std::cout << "Wrong count of years! Try again..." << std::endl;
                throw std::invalid_argument("Invalid count! Try again...");
            }

            firstMachineInPeriod(years, &machine, &summary_downtime_machine_1);
            secondMachineInPeriod(years, &machine, &summary_downtime_machine_2);

            if (summary_downtime_machine_1 > summary_downtime_machine_2) {
                std::cout << "Machine num.1 have a bigger total downtime value then Machine num.2" << std::endl;
                std::cout << "It is: " << summary_downtime_machine_1 << "\n" << std::endl;
            }
            else {
                std::cout << "Machine num.2 have a bigger total downtime value then Machine num.1" << std::endl;
                std::cout << "It is: " << summary_downtime_machine_2 << "\n" << std::endl;
            }
        }
        catch (const std::invalid_argument& e) {
            std::cout << e.what() << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    return 0;
}

/*
 * Рандомайзер целых чисел
 *
 * @param min - левая граница диапазона.
 * @param max - правая граница диапазона.
 * @return возвращает случайное целое число из диапазона.
 */
int randomInt(int min, int max) {
    return rand() % (max - min + 1) + min;
}

/*
 * Инициализация деталей станков
 *
 * @param machine - указатель на класс станка, который создаст вектор компонентов станка.
 */
void generateMachine(Machine* machine) {
    machine->generateShaft();
    machine->generateMotor();
    machine->generatePanel();
    machine->generateNyam();
}

/*
 * Механизм работы валовой детали
 *
 * @param machine - указатель на класс станка
 * @param downtime_shaft - время простоя станка, пока деталь на замене
 * @param month_intensity - интенсивность работы станка в конкретном месяце
 * @param flag - флаг на проверку, какой год подряд работает станок
 * @param year - текущий год
 * @param years - суммарное количество лет, за которые станок проработает
 * @param is_first_year - флаг на проверку работы станка в первый год эксплуатации
 */
void shaftMechanism(Machine* machine, int* downtime_shaft, int month_intensity, int* flag, int* year, int years, int* is_first_year) {    
    if ((*(Shaft*)machine->details[0]).degree_of_wear >= 100 || ((*year) != 0 && years >= (*(Shaft*)machine->details[0]).service_life)) {

        if ((*downtime_shaft) == 0) {
            (*(Shaft*)machine->details[0]).degree_of_wear = 0;
            (*downtime_shaft) = 4;
            (*flag) = 1;

            (*(Shaft*)machine->details[0]).crash_due_wear++;
            if (years >= (*(Shaft*)machine->details[0]).service_life && (*year) != 0) {
                (*(Shaft*)machine->details[0]).crash_due_year++;
            }

            (*(Shaft*)machine->details[0]).detail_downtime++;
            (*year) = 0;
        }
        else {
            (*downtime_shaft)--;
        }
    }
    else {
        if ((*(Shaft*)machine->details[0]).degree_of_wear == 70 && (*flag) == 1 && (*(Shaft*)machine->details[0]).time_to_repair != 0) {
            (*(Shaft*)machine->details[0]).time_to_repair--;
            (*(Shaft*)machine->details[0]).detail_downtime++;
        }
        else if ((*(Shaft*)machine->details[0]).degree_of_wear == 70 && (*is_first_year) == 0 && (*(Shaft*)machine->details[0]).time_to_repair != 0) {
            (*(Shaft*)machine->details[0]).time_to_repair--;
            (*(Shaft*)machine->details[0]).detail_downtime++;
        }
        else if ((*(Shaft*)machine->details[0]).time_to_repair == 0 && (*is_first_year) == 0) {
            (*(Shaft*)machine->details[0]).degree_of_wear -= 15;
            (*(Shaft*)machine->details[0]).time_to_repair = 2;
            (*is_first_year)++;
        }
        else if ((*(Shaft*)machine->details[0]).time_to_repair == 0 && (*flag) == 1) {
            (*(Shaft*)machine->details[0]).degree_of_wear -= 15;
            (*(Shaft*)machine->details[0]).time_to_repair = 2;
            (*flag) = 0;
        }
        else {
            (*(Shaft*)machine->details[0]).work_method(month_intensity);
            (*(Shaft*)machine->details[0]).detection_method();
        }
    }
}

/*
 * Механизм работы электрического мотора
 *
 * @param machine - указатель на класс станка
 * @param downtime_shaft - время простоя станка, пока деталь на замене
 * @param month_intensity - интенсивность работы станка в конкретном месяце
 * @param flag - флаг на проверку, какой год подряд работает станок
 * @param year - текущий год
 * @param years - суммарное количество лет, за которые станок проработает
 * @param is_first_year - флаг на проверку работы станка в первый год эксплуатации
 */
void motorMechanism(Machine* machine, int* downtime_motor, int month_intensity, int* flag, int* year, int years, int* is_first_year) {
    if ((*(ElectricMotor*)machine->details[1]).degree_of_wear >= 1000 || ((*year) != 0 && years  >= (*(ElectricMotor*)machine->details[1]).service_life)) {

        if ((*downtime_motor) == 0) {
            (*(ElectricMotor*)machine->details[1]).degree_of_wear = 0;
            (*downtime_motor) = 4;
            (*flag) = 1;

            (*(ElectricMotor*)machine->details[1]).crash_due_wear++;
            if (years >= (*(ElectricMotor*)machine->details[1]).service_life && (*year) != 0) {
                (*(ElectricMotor*)machine->details[1]).crash_due_year++;
            }

            (*(ElectricMotor*)machine->details[1]).detail_downtime++;
            (*year) = 0;
        }
        else {
            (*downtime_motor)--;
        }
    }
    else {
        if ((*(ElectricMotor*)machine->details[1]).degree_of_wear == 700 && (*flag) == 1 && (*(ElectricMotor*)machine->details[1]).time_to_repair != 0) {
            (*(ElectricMotor*)machine->details[1]).time_to_repair--;
            (*(ElectricMotor*)machine->details[1]).detail_downtime++;
        }
        else if ((*(ElectricMotor*)machine->details[1]).degree_of_wear == 700 && (*is_first_year) == 0 && (*(ElectricMotor*)machine->details[1]).time_to_repair != 0) {
            (*(ElectricMotor*)machine->details[1]).time_to_repair--;
            (*(ElectricMotor*)machine->details[1]).detail_downtime++;
        }
        else if ((*(ElectricMotor*)machine->details[1]).time_to_repair == 0 && (*is_first_year) == 0) {
            (*(ElectricMotor*)machine->details[1]).degree_of_wear -= 150;
            (*(ElectricMotor*)machine->details[1]).time_to_repair = 120;
            (*is_first_year)++;
        }
        else if ((*(ElectricMotor*)machine->details[1]).time_to_repair == 0) {
            (*(ElectricMotor*)machine->details[1]).degree_of_wear -= 150;
            (*(ElectricMotor*)machine->details[1]).time_to_repair = 120;
            (*flag) = 0;
        }
        else {
            (*(ElectricMotor*)machine->details[1]).work_method(month_intensity);
            (*(ElectricMotor*)machine->details[1]).detection_method();
        }
    }
}

/*
 * Механизм работы панели управления станка
 *
 * @param machine - указатель на класс станка
 * @param downtime_shaft - время простоя станка, пока деталь на замене
 * @param month_intensity - интенсивность работы станка в конкретном месяце
 * @param flag - флаг на проверку, какой год подряд работает станок
 * @param year - текущий год
 * @param years - суммарное количество лет, за которые станок проработает
 * @param is_first_year - флаг на проверку работы станка в первый год эксплуатации
 */
void panelMechanism(Machine* machine, int* downtime_panel, int month_intensity, int* flag, int* year, int years, int* is_first_year) {
    if ((*(ControlPanel*)machine->details[2]).degree_of_wear >= 500 || ((*year) != 0 && years >= (*(ControlPanel*)machine->details[2]).service_life)) {

        if ((*downtime_panel) == 0) {
            (*(ControlPanel*)machine->details[2]).degree_of_wear = 0;
            (*downtime_panel) = 4;
            (*flag) = 1;

            (*(ControlPanel*)machine->details[2]).crash_due_wear++;
            if (years >= (*(ControlPanel*)machine->details[2]).service_life && (*year) != 0) {
                (*(ControlPanel*)machine->details[2]).crash_due_year++;
            }

            (*(ControlPanel*)machine->details[2]).detail_downtime++;
            (*year) = 0;
        }
        else {
            (*downtime_panel)--;
        }
    }
    else {
        if ((*(ControlPanel*)machine->details[2]).degree_of_wear == 350 && (*flag) == 1 && (*(ControlPanel*)machine->details[2]).time_to_repair != 0) {
            (*(ControlPanel*)machine->details[2]).time_to_repair--;
            (*(ControlPanel*)machine->details[2]).detail_downtime++;
        }
        else if ((*(ControlPanel*)machine->details[2]).degree_of_wear == 350 && (*is_first_year) == 0 && (*(ControlPanel*)machine->details[2]).time_to_repair != 0) {
            (*(ControlPanel*)machine->details[2]).time_to_repair--;
            (*(ControlPanel*)machine->details[2]).detail_downtime++;
        }
        else if ((*(ControlPanel*)machine->details[2]).time_to_repair == 0 && (*is_first_year) == 0) {
            (*(ControlPanel*)machine->details[2]).degree_of_wear -= 70;
            (*(ControlPanel*)machine->details[2]).time_to_repair = 12;
            (*is_first_year)++;
        }
        else if ((*(ControlPanel*)machine->details[2]).time_to_repair == 0) {
            (*(ControlPanel*)machine->details[2]).degree_of_wear -= 70;
            (*(ControlPanel*)machine->details[2]).time_to_repair = 12;
            (*flag) = 0;
        }
        else {
            (*(ControlPanel*)machine->details[2]).work_method(month_intensity);
            (*(ControlPanel*)machine->details[2]).detection_method();
        }
    }
}

/*
 * Механизм работы режущей головки станка
 *
 * @param machine - указатель на класс станка
 * @param downtime_shaft - время простоя станка, пока деталь на замене
 * @param month_intensity - интенсивность работы станка в конкретном месяце
 * @param flag - флаг на проверку, какой год подряд работает станок
 * @param year - текущий год
 * @param years - количество лет, в течение которых станок будет работать
 * @param is_first_year - флаг на проверку работы станка в первый год эксплуатации
 */
void amnyamMechanism(Machine* machine, int* downtime_amnyam, int month_intensity, int* flag, int* year, int years, int* is_first_year) {
    if ((*(AmNyam*)machine->details[3]).degree_of_wear >= 300 || ((*year) != 0 && years >= (*(AmNyam*)machine->details[3]).service_life)) {

        if ((*downtime_amnyam) == 0) {
            (*(AmNyam*)machine->details[3]).degree_of_wear = 0;
            (*downtime_amnyam) = 4;
            (*flag) = 1;

            (*(AmNyam*)machine->details[3]).crash_due_wear++;
            if (years >= (*(AmNyam*)machine->details[3]).service_life && (*year) != 0) {
                (*(AmNyam*)machine->details[3]).crash_due_year++;
            }

            (*(AmNyam*)machine->details[3]).detail_downtime++;
            (*year) = 0;
        }
        else {
            (*downtime_amnyam)--;
        }
    }
    else {
        if ((*(AmNyam*)machine->details[3]).degree_of_wear == 200 && (*flag) == 1 && (*(AmNyam*)machine->details[3]).time_to_repair != 0) {
            (*(AmNyam*)machine->details[3]).time_to_repair--;
            (*(AmNyam*)machine->details[3]).detail_downtime++;
        }
        else if ((*(AmNyam*)machine->details[3]).degree_of_wear == 200 && (*is_first_year) == 0 && (*(AmNyam*)machine->details[3]).time_to_repair != 0) {
            (*(AmNyam*)machine->details[3]).time_to_repair--;
            (*(AmNyam*)machine->details[3]).detail_downtime++;
        }
        else if ((*(AmNyam*)machine->details[3]).time_to_repair == 0 && (*is_first_year) == 0) {
            (*(AmNyam*)machine->details[3]).degree_of_wear -= 50;
            (*(AmNyam*)machine->details[3]).time_to_repair = 5;
            (*is_first_year)++;
        }
        else if ((*(AmNyam*)machine->details[3]).time_to_repair == 0 && (*flag) == 1) {
            (*(AmNyam*)machine->details[3]).degree_of_wear -= 50;
            (*(AmNyam*)machine->details[3]).time_to_repair = 5;
            (*flag) = 0;
        }
        else {
            (*(AmNyam*)machine->details[3]).work_method(month_intensity);
            (*(AmNyam*)machine->details[3]).detection_method();
        }
    }
}

/*
 * Имитация работы первого станка
 *
 * @param period_in_year - количество лет, в течение которых станок будет работать
 * @param machine - указатель на класс станка
 * @param total_downtime - суммарное время простоя станка (в часах)
 */
void firstMachineInPeriod(int period_in_year, Machine* machine, int* total_downtime) {
    int little_repair_shaft = (*(Shaft*)machine->details[0]).time_to_repair;
    int reload_shaft = 4;
    int first_year_shaft = 0;
    int little_repair_motor = (*(ElectricMotor*)machine->details[1]).time_to_repair;
    int reload_motor = 4;
    int first_year_motor = 0;

    for (size_t year = 0; year < period_in_year; year++) {
        int temp_year_shaft = year;
        int temp_year_motor = year;
        for (size_t month = 0; month < 12; month++) {
            int month_intensity = randomInt(0, 1); // интенсивность в месяц
            for (size_t day = 0; day < 31; day++) {
                int random_error = randomInt(0, 100);
                for (size_t hour = 0; hour < 12; hour++) {
                    if (month_intensity == int(eIntensity::Hard)) {
                        shaftMechanism(machine, &reload_shaft, month_intensity, &little_repair_shaft, &temp_year_shaft, year, &first_year_shaft);
                        motorMechanism(machine, &reload_motor, month_intensity, &little_repair_motor, &temp_year_motor, year, &first_year_motor);
                    }
                    else {
                        shaftMechanism(machine, &reload_shaft, month_intensity, &little_repair_shaft, &temp_year_shaft, year, &first_year_shaft);
                        motorMechanism(machine, &reload_motor, month_intensity, &little_repair_motor, &temp_year_motor, year, &first_year_motor);
                    }
                }
            }
        }
    }

    (*total_downtime) = (*(Shaft*)machine->details[0]).detail_downtime + (*(ElectricMotor*)machine->details[1]).detail_downtime;
    std::cout << "~~~Machine num.1~~~" << std::endl;
    std::cout << period_in_year << " year(-s) total report:\n" << std::endl;
    std::cout << "The shaft was broken due of wear " << (*(Shaft*)machine->details[0]).crash_due_wear << " times" << std::endl;
    std::cout << "The detail's parts was replaced " << (*(Shaft*)machine->details[0]).crash_due_wear << " times" << std::endl;
    std::cout << "The shaft was broken due of " << period_in_year << " year(-s) using " << (*(Shaft*)machine->details[0]).crash_due_year << " times" << std::endl;
    std::cout << (*(Shaft*)machine->details[0]).price * (*(Shaft*)machine->details[0]).crash_due_year << " spent for the total reload" << std::endl;
    std::cout << (*(Shaft*)machine->details[0]).price_for_once * (*(Shaft*)machine->details[0]).crash_due_wear << " spent for the detail service\n" << std::endl;
    std::cout << "The electric motor was broken due of wear " << (*(ElectricMotor*)machine->details[1]).crash_due_wear << " times" << std::endl;
    std::cout << "The detail's parts was replaced " << (*(ElectricMotor*)machine->details[1]).crash_due_wear << " times" << std::endl;
    std::cout << "The electric motor was broken due of " << period_in_year << " year(-s) using " << (*(ElectricMotor*)machine->details[1]).crash_due_year << " times" << std::endl;
    std::cout << "The machine's count of breakdowns is " << (*(Shaft*)machine->details[0]).crash_due_wear + (*(ElectricMotor*)machine->details[1]).crash_due_wear << " times" << std::endl;
    std::cout << (*(ElectricMotor*)machine->details[1]).price * (*(ElectricMotor*)machine->details[1]).crash_due_year << " spent for the total reload" << std::endl;
    std::cout << (*(ElectricMotor*)machine->details[1]).price_for_once * (*(ElectricMotor*)machine->details[1]).crash_due_wear << " spent for the detail service\n" << std::endl;
    std::cout << "Total downtime of this machine is " << (*total_downtime) << " hours" << std::endl;
    std::cout << "Total money waste: " << (*(Shaft*)machine->details[0]).price * (*(Shaft*)machine->details[0]).crash_due_year
        + (*(Shaft*)machine->details[0]).price_for_once * (*(Shaft*)machine->details[0]).crash_due_wear
        + (*(ElectricMotor*)machine->details[1]).price * (*(ElectricMotor*)machine->details[1]).crash_due_year
        + (*(ElectricMotor*)machine->details[1]).price_for_once * (*(ElectricMotor*)machine->details[1]).crash_due_wear << "\n" << std::endl;
}

/*
 * Имитация работы второго станка
 *
 * @param period_in_year - количество лет, в течение которых станок будет работать
 * @param machine - указатель на класс станка
 * @param total_downtime - суммарное время простоя станка (в часах)
 */
void secondMachineInPeriod(int period_in_year, Machine* machine, int* total_downtime) {
    int little_repair_panel = (*(ControlPanel*)machine->details[2]).time_to_repair;
    int reload_panel = 4;
    int first_year_panel = 0;
    int little_repair_amnyam = (*(AmNyam*)machine->details[3]).time_to_repair;;
    int reload_amnyam = 4;
    int first_year_amnyam = 0;

    for (size_t year = 0; year < period_in_year; year++) {
        int temp_year_panel = year;
        int temp_year_amnyam = year;
        for (size_t month = 0; month < 12; month++) {
            int month_intensity = randomInt(0, 1); // интенсивность в месяц
            for (size_t day = 0; day < 31; day++) {
                int random_error = randomInt(0, 100);
                for (size_t hour = 0; hour < 12; hour++) {
                    if (month_intensity == int(eIntensity::Hard)) {
                        panelMechanism(machine, &reload_panel, month_intensity, &little_repair_panel, &temp_year_panel, year, &first_year_panel);
                        amnyamMechanism(machine, &reload_amnyam, month_intensity, &little_repair_amnyam, &temp_year_amnyam, year, &first_year_amnyam);
                    }
                    else {
                        panelMechanism(machine, &reload_panel, month_intensity, &little_repair_panel, &temp_year_panel, year, &first_year_panel);
                        amnyamMechanism(machine, &reload_amnyam, month_intensity, &little_repair_amnyam, &temp_year_amnyam, year, &first_year_amnyam);
                    }
                }
            }
        }
    }

    (*total_downtime) = (*(ControlPanel*)machine->details[2]).detail_downtime + (*(AmNyam*)machine->details[3]).detail_downtime;
    std::cout << "~~~Machine num.2~~~" << std::endl;
    std::cout << period_in_year << " year(-s) total report:\n" << std::endl;
    std::cout << "The control panel was broken due of wear " << (*(ControlPanel*)machine->details[2]).crash_due_wear << " times" << std::endl;
    std::cout << "The detail's parts was replaced " << (*(ControlPanel*)machine->details[2]).crash_due_wear << " times" << std::endl;
    std::cout << "The control panel was broken due of " << period_in_year << " year(-s) using " << (*(ControlPanel*)machine->details[2]).crash_due_year << " times" << std::endl;
    std::cout << (*(ControlPanel*)machine->details[2]).price * (*(ControlPanel*)machine->details[2]).crash_due_year << " spent for the total reload" << std::endl;
    std::cout << (*(ControlPanel*)machine->details[2]).price_for_once * (*(ControlPanel*)machine->details[2]).crash_due_wear << " spent for the detail service\n" << std::endl;
    std::cout << "The amnyam was broken due of wear " << (*(AmNyam*)machine->details[3]).crash_due_wear << " times" << std::endl;
    std::cout << "The detail's parts was replaced " << (*(AmNyam*)machine->details[3]).crash_due_wear << " times" << std::endl;
    std::cout << "The amnyam was broken due of " << period_in_year << " year(-s) using " << (*(AmNyam*)machine->details[3]).crash_due_year << " times" << std::endl;
    std::cout << "The machine's count of breakdowns is " << (*(ControlPanel*)machine->details[2]).crash_due_wear + (*(AmNyam*)machine->details[3]).crash_due_wear << " times" << std::endl;
    std::cout << (*(AmNyam*)machine->details[3]).price * (*(AmNyam*)machine->details[3]).crash_due_year << " spent for the total reload" << std::endl;
    std::cout << (*(AmNyam*)machine->details[3]).price_for_once * (*(AmNyam*)machine->details[3]).crash_due_wear << " spent for the detail service\n" << std::endl;
    std::cout << "Total downtime of this machine is " << (*total_downtime) << " hours" << std::endl;
    std::cout << "Total money waste: " << (*(ControlPanel*)machine->details[2]).price * (*(ControlPanel*)machine->details[2]).crash_due_year
        + (*(ControlPanel*)machine->details[2]).price_for_once * (*(ControlPanel*)machine->details[2]).crash_due_wear
        + (*(AmNyam*)machine->details[3]).price * (*(AmNyam*)machine->details[3]).crash_due_year
        + (*(AmNyam*)machine->details[3]).price_for_once * (*(AmNyam*)machine->details[3]).crash_due_wear << "\n" << std::endl;
}
