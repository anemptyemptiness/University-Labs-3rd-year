#include <iostream>
#include <string>
#include <stdexcept>
#include <time.h>
using namespace std;

const string ALPHABET = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

int random(int min, int max) {
    return min + rand() % (max - min + 1);
}

string generateString(int length_of_string) 
{
    string target_string = "";
    
    for (unsigned i = 0; i < length_of_string; i++) {
        target_string += ALPHABET[random(0, ALPHABET.length())];
    }

    return target_string;
}

void game(unsigned length_of_string)
{
    string target_string = generateString(length_of_string);
    string current_string = "";

    int iterations = 0;
    int max_attemts = 0;
    int temp_attempt = 0;

    while (current_string != target_string) {
        iterations++;
        temp_attempt++;

        char guess_char = ALPHABET[random(0, ALPHABET.length())];
        if (guess_char == target_string[current_string.length()]) {
            current_string += guess_char;

            if (max_attemts < temp_attempt) {
                max_attemts = temp_attempt;
            }

            temp_attempt = 0;
        }
    }

    cout << "\n\n\t\t\t\t\t--------GAME RESULTS--------\n\n" << endl;
    cout << "Target string: " << target_string << "\n\n";
    cout << "Current string: " << current_string << "\n\n";
    cout << "Iterations: " << iterations << "\n\n";
    cout << "Max attempts: " << max_attemts << "\n\n";
    cout << "\n\n\t\t\t\t\t--------   GOOD BYE  --------\n\n" << "\n\n";
}

int main()
{
    srand(time(0));

    unsigned length;
    
    while (true) {
        try {
            cout << "Please enter a length of target string(1-1000): ";
            cin >> length;

            if (length <= 0 or length > 1000) {
                throw invalid_argument("Invalid argument! Try again...");
            }

            break;
        }
        catch (const invalid_argument& e) {
            cerr << e.what() << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    game(length);

    return 0;
}
