#include <iostream>
#include <ctime>
#include <cmath>

using namespace std;

const float PI = 3.14;
const size_t SIZE = 5;

enum class Figures {Quadrate_type, Rectangle_type, Triangle_type, Circle_type};

struct Quadrate {
public:
    Quadrate(float p_a) {
        _a = p_a;
    }

    float square() {
        return _a * _a;
    }

    float inscribedCircle() {
        return _a / 2;
    }

private:
    float _a;
};

struct Rectangle {
public:
    Rectangle(float p_a, float p_b) {
        _a = p_a;
        _b = p_b;
    }

    float square() {
        return _a * _b;
    }

    float inscribedCircle() {
        return sqrt((pow(_a, 2) + pow(_b, 2)) / 2); // half of diagonal 
    }

private:
    float _a;
    float _b;
};

struct Triangle {
public:
    Triangle(float p_a, float p_b, float p_c) {
        _a = p_a;
        _b = p_b;
        _c = p_c;
    }

    float square() {
        float p = (_a + _b + _c) / 2;
        return sqrt(p * (p - _a) * (p - _b) * (p - _c));
    }

    float inscribedCircle() {
        return (_a * _b * _c) / (4 * square());
    }

private:
    float _a;
    float _b;
    float _c;
};

struct Circle {
public:
    Circle (float p_r) {
        _r = p_r;
    }

    float square() {
        return PI * pow(_r, 2);
    }

    float inscribedCircle() {
        return sqrt(square() / PI);
    }

private:
    float _r;
};

float randomFloat(int min, int max);
int randomInt(int min, int max);
float calculateTotalSquare(void* arr, char* arr_of_types);
void calculateTypes(char* what_types[], char* arr_of_types[], size_t N);
void printChoiceFigures(void* arr, char* arr_of_types, float choice, size_t index);
void findRadius(void* arr, float** squares, int figure, size_t index);

int main()
{
    srand(time(NULL));

    while (true) {

        void* arr[SIZE];
        char* arr_of_types[SIZE];
        char* what_types[SIZE];
        float** squares = new float* [SIZE];
        for (size_t i = 0; i < SIZE; i++)
            squares[i] = new float[SIZE];

        unsigned N = 0;
        char mode = 0;
        float total_square = 0;
        float choice_value = 0;

        cout << "Enter the square value with which the squares of the figures will be compared: ";

        try {
            cin >> choice_value;

            if (cin.fail()) {
                throw invalid_argument("Invalid argument! Try again...");
            }

            for (size_t i = 0; i < SIZE; i++) {
                int temp = randomInt(0, 3);
                if (temp == int(Figures::Quadrate_type)) {
                    Quadrate quadrate(randomFloat(50, 100));
                    arr[i] = &quadrate;
                    arr_of_types[i] = (char*)"Quadrate";

                    total_square += calculateTotalSquare(arr[i], arr_of_types[i]);
                    printChoiceFigures(arr[i], arr_of_types[i], choice_value, i);
                    findRadius(arr[i], squares, temp, i);
                }
                else if (temp == int(Figures::Rectangle_type)) {
                    Rectangle rectangle(randomFloat(50, 100), randomFloat(50, 100));
                    arr[i] = &rectangle;
                    arr_of_types[i] = (char*)"Rectangle";

                    total_square += calculateTotalSquare(arr[i], arr_of_types[i]);
                    printChoiceFigures(arr[i], arr_of_types[i], choice_value, i);
                    findRadius(arr[i], squares, temp, i);
                }
                else if (temp == int(Figures::Triangle_type)) {
                    Triangle triangle{ randomFloat(50, 100), randomFloat(50, 100), randomFloat(50, 100) };
                    arr[i] = &triangle;
                    arr_of_types[i] = (char*)"Triangle";

                    total_square += calculateTotalSquare(arr[i], arr_of_types[i]);
                    printChoiceFigures(arr[i], arr_of_types[i], choice_value, i);
                    findRadius(arr[i], squares, temp, i);
                }
                else if (temp == int(Figures::Circle_type)) {
                    Circle circle{ randomFloat(50, 100) };
                    arr[i] = &circle;
                    arr_of_types[i] = (char*)"Circle";

                    total_square += calculateTotalSquare(arr[i], arr_of_types[i]);
                    printChoiceFigures(arr[i], arr_of_types[i], choice_value, i);
                    findRadius(arr[i], squares, temp, i);
                }
            }

            calculateTypes(what_types, arr_of_types, SIZE);

            float max_radius = (*(squares[0]));
            for (int i = 0; i < SIZE; i++) {
                if (max_radius < (*(squares[i]))) {
                    max_radius = (*(squares[i]));
                }
            }

            cout << "\nTotal square of all shapes is " << total_square << endl;
            cout << "The radius of the circle into which all generated shapes can be entered is " << max_radius << "\n\n" << endl;

            for (size_t i = 0; i < SIZE; i++)
                delete[] squares[i];
            delete[] squares;
        }
        catch (const invalid_argument& e) {
            cerr << e.what() << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    return 0;
}

/*
 * Рандомайзер чисел с дробной частью
 *
 * @param min - левая граница диапазона.
 * @param max - правая граница диапазона.
 * @return возвращает случайное дробное число из диапазона.
 */
float randomFloat(int min, int max) {
    return round(min + static_cast<float>(rand()) * static_cast<float>(max - min) / RAND_MAX);
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
 * Функция для подсчёта суммы площадей всех фигур
 *
 * @param arr - указатель на тип(структуру) фигуры.
 * @param arr_of_type - указатель на имя типа(структуры).
 * @return возвращает сумму площадей фигур.
 */
float calculateTotalSquare(void* arr, char* arr_of_types) {
    float total_square = 0.0;
    
    if (arr_of_types == "Quadrate") {
        cout << (*(Quadrate*)arr).square() << endl;
        total_square += (*(Quadrate*)arr).square();
    }
    else if (arr_of_types == "Rectangle") {
        cout << (*(Rectangle*)arr).square() << endl;
        total_square += (*(Rectangle*)arr).square();
    }
    else if (arr_of_types == "Triangle") {
        total_square += (*(Triangle*)arr).square();
    }
    else if (arr_of_types == "Circle") {
        total_square += (*(Circle*)arr).square();
    }

    return total_square;
}

/*
 * Функция для вывода всех сгенерированных фигур
 *
 * @param what_types - массив указателей на тип(структуру) фигуры.
 * @param arr_of_type - указатель на имя типа(структуры).
 * @param N - размер массива
 * @return ничего не возвращает, функция void.
 */
void calculateTypes(char* what_types[], char* arr_of_types[], size_t N) {
    int quadrate = 0;
    int rectangle = 0;
    int triangle = 0;
    int circle = 0;

    for (size_t i = 0; i < N; i++) {
        if (arr_of_types[i] == "Quadrate") {
            what_types[i] = arr_of_types[i];
            quadrate += 1;
        }
        else if (arr_of_types[i] == "Rectangle") {
            what_types[i] = arr_of_types[i];
            rectangle += 1;
        }
        else if (arr_of_types[i] == "Triangle") {
            what_types[i] = arr_of_types[i];
            triangle += 1;
        }
        else if (arr_of_types[i] == "Circle") {
            what_types[i] = arr_of_types[i];
            circle += 1;
        }
    }
    
    cout << "There are " << quadrate + rectangle + triangle + circle << " figure(-s)\nIt is: ";
    for (size_t i = 0; i < N; i++) {
        if (i == N - 1)
            cout << what_types[i] << endl;
        else
            cout << what_types[i] << ", ";
    }
}

/*
 * Функция для вывода всех фигур, чья площадь больше указанного значения
 *
 * @param arr - указатель на тип(структуру) фигуры.
 * @param arr_of_type - указатель на имя типа(структуры).
 * @param choice - сравнимаемое значение
 * @param index индекс в искомом массиве для проверки, дошел ли индекс массива до конечного значения
 * @return ничего не возвращает, функция void.
 */
void printChoiceFigures(void* arr, char* arr_of_types, float choice, size_t index) {
    int count = 0;
    if (arr_of_types == "Quadrate" && choice < (*((Quadrate*)arr)).square()) {
        cout << "\nType of figure: " << arr_of_types << endl;
        cout << "Info:\nYour entered square is " << choice << "\nSquare of figure is " << (*((Quadrate*)arr)).square() << endl;
        count += 1;
    }
    else if (arr_of_types == "Rectangle" && choice < (*((Rectangle*)arr)).square()) {
        cout << "\nType of figure: " << arr_of_types << endl;
        cout << "Info:\nYour entered square is " << choice << "\nSquare of figure is " << (*((Rectangle*)arr)).square() << endl;
        count += 1;
    }
    else if (arr_of_types == "Triangle" && choice < (*((Triangle*)arr)).square()) {
        cout << "\nType of figure: " << arr_of_types << endl;
        cout << "Info:\nYour entered square is " << choice << "\nSquare of figure is " << (*((Triangle*)arr)).square() << endl;
        count += 1;
    }
    else if (arr_of_types == "Circle" && choice < (*((Circle*)arr)).square()) {
        cout << "\nType of figure: " << arr_of_types << endl;
        cout << "Info:\nYour entered square is " << choice << "\nSquare of figure is " << (*((Circle*)arr)).square() << endl;
        count += 1;
    }
    else if (count == 0 && index == SIZE - 1) {
        cout << "\nNothing figures which square is bigger then your square..." << endl;
    }
}

/*
 * Функция поиска радиуса окружности, в которую можно вписать все сгенерированные фигуры
 *
 * @param arr - указатель на тип(структуру) фигуры.
 * @param squares - массив указателей, запоминающий значения всех площадей для дальнейшего поиска максимального среди них.
 * @param figure - целочисленное значение, с которым сравниваются значения enum Figures
 * @param index - индекс в искомом массиве для движения по массиву указателей squares
 * @return ничего не возвращает, функция void.
 */
void findRadius(void* arr, float** squares, int figure, size_t index) {
    if (figure == int(Figures::Quadrate_type)) {
        *(squares[index]) = (*(Quadrate*)arr).inscribedCircle();
    }
    else if (figure == int(Figures::Rectangle_type)) {
        *(squares[index]) = (*(Rectangle*)arr).inscribedCircle();
    }
    else if (figure == int(Figures::Triangle_type)) {
        *(squares[index]) = (*(Triangle*)arr).inscribedCircle();
    }
    else if (figure == int(Figures::Circle_type)) {
        *(squares[index]) = (*(Circle*)arr).inscribedCircle();
    }
}
