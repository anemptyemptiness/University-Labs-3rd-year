#include <iostream>
#include <ctime>
using namespace std;

/*
* Класс Matrix, реализующий возможности модификации матрицы пользователем согласно варианту №3 лабораторной работы #2
* Выполнил: студент группы КС-33, Неруссков Дмитрий
*/
class Matrix {
private:
    int** _matrix = 0;
    unsigned _N = 0;

    /*
    * Рандомайзер чисел-значений для матрицы
    *
    * @param min - левая граница допустимого значения.
    * @param max - правая граница допустимого значения.
    * @return возвращает случайное число из указанного диапазона.
    */
    int _random(int min, int max) {
        return rand() % (max - min + 1) + min;
    }

    /*
    * Функция, заполняющая матрицу нулями
    */
    void _zeroMatrix(int** p_matrix, size_t p_N) {
        for (size_t i = 0; i < p_N; i++) {
            for (size_t j = 0; j < p_N; j++) {
                p_matrix[i][j] = 0;
            }
        }
    }

    /*
    * Функция, печатающая матрицу
    */
    void _printMatrix() {
        for (size_t i = 0; i < _N; i++) {
            for (size_t j = 0; j < _N; j++) {
                cout << _matrix[i][j] << "\t";
            }
            cout << endl;
        }
    }

    /*
    * Функция, реализующая очистку памяти, выделенную под матрицу
    */
    void _deleteMatrix() {
        for (size_t i = 0; i < _N; i++) {
            delete[] _matrix[i];
        }

        delete[] _matrix;
    }

public:
    /*
    * Конструктор класса Matrix, выделяющий память для матрицы размера N*N и заполняющий матрицу нулями
    * 
    * @param p_N - размер матрицы, вводимый пользователем, неотрицательный
    */
    Matrix (size_t p_N) {
        _matrix = new int* [p_N];
        _N = p_N;

        for (size_t i = 0; i < _N; i++) {
            _matrix[i] = new int[_N];
        }

        for (size_t i = 0; i < _N; i++) {
            for (size_t j = 0; j < _N; j++) {
                _matrix[i][j] = 0;
            }
        }
    }

    /*
    * Функция, реализующая симметричную матрицу относительно главной диагонали
    */
    void symmetricalMatrix() {
        for (size_t i = 0; i < _N; i++) {
            for (size_t j = 0; j < _N; j++) {
                if (i == j) {
                    _matrix[i][j] = 1;
                }
                else {
                    if (j > i) {
                        _matrix[i][j] = rand() % (9 - 1 + 1) + 1;
                        _matrix[j][i] = _matrix[i][j];
                    }
                    else
                        continue;
                }
            }
        }

        _printMatrix();
        _deleteMatrix();
    }

    /*
    * Функция, реализующая симметричную матрицу относительно побочной диагонали
    */
    void persymmetricalMatrix() {
        for (size_t i = 0; i < _N; i++) {
            for (size_t j = 0; j < _N; j++) {
                _matrix[i][_N - i - 1] = 1;
                if (i == j && _matrix[i][j] != 1) {
                    _matrix[i][j] = rand() % (9 - 1 + 1) + 1;
                    _matrix[_N - i - 1][_N - j - 1] = _matrix[i][j];
                }
                else if (i < j && j < _N - i - 1) {
                    _matrix[i][j] = rand() % (9 - 1 + 1) + 1;
                    _matrix[_N - j - 1][_N - i - 1] = _matrix[i][j];
                }
                else if (i > j && i < _N - j - 1) {
                    _matrix[i][j] = rand() % (9 - 1 + 1) + 1;
                    _matrix[_N - j - 1][_N - i - 1] = _matrix[i][j];
                }
            }
        }

        _printMatrix();
        _deleteMatrix();
    }

    /*
    * Функция, реализующая матрицу Паскаля
    */
    void pascalMatrix() {
        for (size_t i = 0; i < _N; i++) {
            for (size_t j = 0; j < _N; j++) {
                if (j == 0 || i == j) {
                    _matrix[i][j] = 1;
                }
                else {
                    _matrix[i][j] = 0;
                }
            }
        }

        for (size_t i = 0; i < _N; i++) {
            int temp = 0;

            for (size_t j = 0; j < _N; j++) {
                if (i < j || (j != _N - 1 && i != _N - 1)) {
                    if (_matrix[i][j + 1] != 0) {
                        temp = _matrix[i][j] + _matrix[i][j + 1];
                        _matrix[i + 1][j + 1] = temp;
                    }
                }
            }
        }

        _printMatrix();
        _deleteMatrix();
    }

    /*
    * Функция, реализующая матрицу "Сапёр"
    */
    void sapperMatrix() {
        if (_N < 3) {
            cout << "Wrong size of array... Try again\n" << endl;
        }
        else {
            int count_mines = ceil(_N * _N * 0.1);
            int counter = 0;

            _zeroMatrix(_matrix, _N);

            while (counter != count_mines) {
                int i = _random(0, _N - 1);
                int j = _random(0, _N - 1);

                if (_matrix[i][j] != -1)
                    _matrix[i][j] = -1;

                counter++;
            }

            for (size_t i = 0; i < _N; i++) {
                for (size_t j = 0; j < _N; j++) {
                    // середина поля
                    if (i >= 1 && j >= 1 && i < _N - 1 && j < _N - 1) {
                        if (_matrix[i][j] != -1) {
                            if (_matrix[i - 1][j - 1] == -1)
                                _matrix[i][j] += 1;
                            if (_matrix[i + 1][j + 1] == -1)
                                _matrix[i][j] += 1;
                            if (_matrix[i - 1][j] == -1)
                                _matrix[i][j] += 1;
                            if (_matrix[i - 1][j + 1] == -1)
                                _matrix[i][j] += 1;
                            if (_matrix[i][j + 1] == -1)
                                _matrix[i][j] += 1;
                            if (_matrix[i + 1][j] == -1)
                                _matrix[i][j] += 1;
                            if (_matrix[i + 1][j - 1] == -1)
                                _matrix[i][j] += 1;
                            if (_matrix[i][j - 1] == -1)
                                _matrix[i][j] += 1;
                        }
                    }
                    // верхняя строка без угловых
                    if (i == 0 && j != 0 && j != _N - 1) {
                        if (_matrix[i][j] != -1) {
                            if (_matrix[i][j - 1] == -1)
                                _matrix[i][j] += 1;
                            if (_matrix[i + 1][j - 1] == -1)
                                _matrix[i][j] += 1;
                            if (_matrix[i + 1][j] == -1)
                                _matrix[i][j] += 1;
                            if (_matrix[i + 1][j + 1] == -1)
                                _matrix[i][j] += 1;
                            if (_matrix[i][j + 1] == -1)
                                _matrix[i][j] += 1;
                        }
                    }
                    // крайний левый столбец у стенки
                    if (j == 0 && i > 0 && i < _N - 1) {
                        if (_matrix[i][j] != -1) {
                            if (_matrix[i - 1][j] == -1)
                                _matrix[i][j] += 1;
                            if (_matrix[i - 1][j + 1] == -1)
                                _matrix[i][j] += 1;
                            if (_matrix[i][j + 1] == -1)
                                _matrix[i][j] += 1;
                            if (_matrix[i + 1][j + 1] == -1)
                                _matrix[i][j] += 1;
                            if (_matrix[i + 1][j] == -1)
                                _matrix[i][j] += 1;
                        }
                    }

                    // углы
                    // левый нижний
                    if (i == _N - 1 && j == 0) {
                        if (_matrix[i][j] != -1) {
                            if (_matrix[i - 1][j] == -1)
                                _matrix[i][j] += 1;
                            if (_matrix[i - 1][j + 1] == -1)
                                _matrix[i][j] += 1;
                            if (_matrix[i][j + 1] == -1)
                                _matrix[i][j] += 1;
                        }
                    }
                    // правый нижний
                    if (i == _N - 1 && j == _N - 1) {
                        if (_matrix[i][j] != -1) {
                            if (_matrix[i][j - 1] == -1)
                                _matrix[i][j] += 1;
                            if (_matrix[i - 1][j - 1] == -1)
                                _matrix[i][j] += 1;
                            if (_matrix[i - 1][j] == -1)
                                _matrix[i][j] += 1;
                        }
                    }
                    // правый верхний
                    if (i == 0 && j == _N - 1) {
                        if (_matrix[i][j] != -1) {
                            if (_matrix[i][j - 1] == -1)
                                _matrix[i][j] += 1;
                            if (_matrix[i + 1][j - 1] == -1)
                                _matrix[i][j] += 1;
                            if (_matrix[i + 1][j] == -1)
                                _matrix[i][j] += 1;
                        }
                    }
                    // левый верхний
                    if (i == 0 && j == 0) {
                        if (_matrix[i][j] != -1) {
                            if (_matrix[i][j + 1] == -1)
                                _matrix[i][j] += 1;
                            if (_matrix[i + 1][j + 1] == -1)
                                _matrix[i][j] += 1;
                            if (_matrix[i + 1][j] == -1)
                                _matrix[i][j] += 1;
                        }
                    }

                    // крайний правый столбец у стенки
                    if (j == _N - 1 && i > 0 && i < _N - 1) {
                        if (_matrix[i][j] != -1) {
                            if (_matrix[i + 1][j] == -1)
                                _matrix[i][j] += 1;
                            if (_matrix[i + 1][j - 1] == -1)
                                _matrix[i][j] += 1;
                            if (_matrix[i][j - 1] == -1)
                                _matrix[i][j] += 1;
                            if (_matrix[i - 1][j - 1] == -1)
                                _matrix[i][j] += 1;
                            if (_matrix[i - 1][j] == -1)
                                _matrix[i][j] += 1;
                        }
                    }
                    // нижняя строка без угловых
                    if (i == _N - 1 && j != 0 && j != _N - 1) {
                        if (_matrix[i][j] != -1) {
                            if (_matrix[i][j - 1] == -1)
                                _matrix[i][j] += 1;
                            if (_matrix[i - 1][j - 1] == -1)
                                _matrix[i][j] += 1;
                            if (_matrix[i - 1][j] == -1)
                                _matrix[i][j] += 1;
                            if (_matrix[i - 1][j + 1] == -1)
                                _matrix[i][j] += 1;
                            if (_matrix[i][j + 1] == -1)
                                _matrix[i][j] += 1;
                        }
                    }
                }
            }
        }

        _printMatrix();
        _deleteMatrix();
    }
};

void showRules();

int main()
{
    srand(time(0));

    unsigned N = 0;
    char mode = 0;

    while (true) {
        try {
            cout << "Please enter the length of the matrix(1-10): ";
            cin >> N;

            if (N < 1 || N > 10) {
                throw invalid_argument("Invalid argument! Try again...");
            }

            cout << "Please select the matrix modification:" << endl;
            cout << "s - symmetrical matrix;\np - persymmertical matrix;\nl - pascal matrix;\nr - sapper matrix;" << endl;

            while (true)
            {
                cin >> mode;
                if (mode == 's') {
                    Matrix matrix(N);
                    matrix.symmetricalMatrix();
                    showRules();
                }
                else if (mode == 'p') {
                    Matrix matrix(N);
                    matrix.persymmetricalMatrix();
                    showRules();
                }
                else if (mode == 'l') {
                    Matrix matrix(N);
                    matrix.pascalMatrix();
                    showRules();
                }
                else if (mode == 'r') {
                    Matrix matrix(N);
                    matrix.sapperMatrix();
                    showRules();
                }
                else if (mode == 'c') {
                    break;
                }
                else {
                    cout << "Invalid agrument! Try again..." << endl;
                    mode = 0;
                    continue;
                }
                mode = 0;
            }
        }
        catch (const invalid_argument& e) {
            cerr << e.what() << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

/*
 * Функция, которая выводит для пользователя способы модификации матрицы
 */
void showRules() {
    cout << "\n\ns - symmetrical matrix;\np - persymmertical matrix;\nl - pascal matrix;\nr - sapper matrix;" << endl;
    cout << "Enter \"c\" symbol to end the program" << endl;
}
