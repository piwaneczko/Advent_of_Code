#include <fstream>
#include <iostream>
#include <map>
#include <string>

using namespace std;
int main(const int argc, char *argv[]) {
    size_t twoCount = 0, threeCount = 0;
    if (argc > 1) {
        fstream input(argv[1]);
        if (input.is_open()) {
            while (!input.eof()) {
                string line;
                getline(input, line);
                map<char, size_t> letters;
                for (const auto &ch : line) {
                    letters[ch]++;
                }
                auto twoCounted = false;
                auto threeCounted = false;
                for (const auto &it : letters) {
                    if (!twoCounted && it.second == 2) {
                        twoCount++;
                        twoCounted = true;
                    } else if (!threeCounted && it.second == 3) {
                        threeCount++;
                        threeCounted = true;
                    }
                }
            }
        }
    }
    cout << "Answer:" << twoCount << " * " << threeCount << " = " << twoCount * threeCount << endl;
    system("pause");
    return 0;
}
