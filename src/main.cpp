#include <time.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

size_t puzzle1(fstream &input) {
    size_t count = 0;
    while (!input.eof()) {
        string line;
        getline(input, line);
        count += stoi(line);
    }
    return count;
}
size_t puzzle2(fstream &input) {
    size_t twoCount = 0, threeCount = 0;
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
    return twoCount * threeCount;
}
size_t puzzle3(fstream &input) {
    size_t squareMeters = 0;
    struct Rect {
        int x, y, width, height;
    };
    int id;
    Rect rect;
    map<int, Rect> rects;
    while (!input.eof()) {
        string line;
        getline(input, line);
        if (sscanf_s(line.c_str(), "#%d @ %d,%d: %dx%d", &id, &rect.x, &rect.y, &rect.width, &rect.height) == 5)
            rects[id] = rect;
        else
            throw exception("Parsing error!");
    }
    vector<vector<int>> mat(1000, vector<int>(1000, 0));
    for (auto &it : rects) {
        for (size_t x = it.second.x; x < (it.second.x + it.second.width); x++) {
            for (size_t y = it.second.y; y < (it.second.y + it.second.height); y++) {
                if (mat[x][y] == 0) {
                    mat[x][y] = 1;
                } else if (mat[x][y] == 1) {
                    mat[x][y] = -1;
                    squareMeters++;
                }
            }
        }
    }

    for (auto &it : rects) {
        auto nooverlap = true;
        for (size_t x = it.second.x; nooverlap && x < (it.second.x + it.second.width); x++) {
            for (size_t y = it.second.y; nooverlap && y < (it.second.y + it.second.height); y++) {
                if (mat[x][y] != 1) nooverlap = false;
            }
        }
        if (nooverlap) cout << "No overlap claim:" << it.first << endl;
    }

    return squareMeters;
}
size_t puzzle4(fstream &input) {
    size_t result = 0;
    struct info {
        string text;
        tm time;
    };
    map<string, info> chronoLog;
    while (!input.eof()) {
        string line;
        getline(input, line);
        const auto br = line.find("] ");
        const auto time = line.substr(1, br - 1);
        struct tm l = {0};
        l.tm_isdst = -1;
        istringstream(time) >> get_time(&l, "%Y-%m-%d %H:%M");
        chronoLog[time] = {line.substr(br + 2), l};
    }
    return result;
}

int main(const int argc, char *argv[]) {
    if (argc > 1) {
        try {
            fstream input(argv[1]);
            if (input.is_open()) {
                cout << "Answer:" << puzzle4(input) << endl;
            }
        } catch (exception &e) {
            cout << e.what() << endl;
        }
    }
    system("pause");
    return 0;
}
