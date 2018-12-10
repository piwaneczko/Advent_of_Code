#include <fstream>
#include <iostream>
#include <string>

using namespace std;
int main(int argc, char *argv[]) {
    size_t sum = 0;
    if (argc > 1) {
        fstream input(argv[1]);
        if (input.is_open()) {
            while (!input.eof()) {
                string line;
                getline(input, line);
                sum += stoi(line);
            }
        }
    }
    cout << "Answer:" << sum << endl;
    system("pause");
    return 0;
}
