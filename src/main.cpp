#include <fstream>
#include <iostream>
#include <map>
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
    struct Info {
        string text;
        int min;
    };
    struct GuardsNaps {
        vector<pair<int, int>> naps;
        size_t minutesCount = 0;
    };
    map<string, Info> chronoLog;
    int year, month, day, hour, min;
    while (!input.eof()) {
        string line;
        getline(input, line);
        const auto br = line.find("] ");
        const auto time = line.substr(1, br - 1);
        sscanf_s(time.c_str(), "%04d-%02d-%02d %02d:%02d", &year, &month, &day, &hour, &min);
        chronoLog[time] = {line.substr(br + 2), min};
    }
    map<int, GuardsNaps> guardsShifts;

    auto id = 0;
    for (auto &it : chronoLog) {
        auto &info = it.second;
        if (sscanf_s(info.text.c_str(), "Guard #%d begins shift", &id) == 1) {
            if (guardsShifts.find(id) == guardsShifts.end()) guardsShifts[id] = GuardsNaps();
        } else if (info.text == "falls asleep") {
            guardsShifts[id].naps.emplace_back(make_pair(it.second.min, 0));
        } else if (info.text == "wakes up") {
            auto &nap = guardsShifts[id].naps.back();
            nap.second = it.second.min;
            guardsShifts[id].minutesCount += (nap.second - nap.first);
        } else
            throw exception("Parsing error!");
    }
    size_t longestNap = 0;
    for (auto &it : guardsShifts) {
        if (it.second.minutesCount > longestNap) {
            longestNap = it.second.minutesCount;
            id = it.first;
        }
    }
    auto mostMinute = 0;
    size_t mostMinuteCount = 0;
    map<int, size_t> mostMinutes;
    for (auto &nap : guardsShifts[id].naps) {
        for (min = nap.first; min < nap.second; min++) {
            mostMinutes[min]++;
            if (mostMinutes[min] > mostMinuteCount) {
                mostMinuteCount = mostMinutes[min];
                mostMinute = min;
            }
        }
    }
    auto guardMostMinuteId = 0;
    auto guardMostMinute = 0;
    auto guardMostMinuteCount = 0;
    for (auto &it : guardsShifts) {
        mostMinutes.clear();
        for (auto &nap : it.second.naps) {
            for (min = nap.first; min < nap.second; min++) {
                mostMinutes[min]++;
                if (mostMinutes[min] > guardMostMinuteCount) {
                    guardMostMinuteCount = mostMinutes[min];
                    guardMostMinuteId = it.first;
                    guardMostMinute = min;
                }
            }
        }
    }
    cout << "Puzzle part two answer:" << guardMostMinuteId * guardMostMinute << endl;

    return mostMinute * id;
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
