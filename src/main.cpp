#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

void puzzle1(fstream &input) {
    size_t count = 0;
    while (!input.eof()) {
        string line;
        getline(input, line);
        count += stoi(line);
    }
    cout << "Answer:" << count << endl;
}
void puzzle2(fstream &input) {
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

    cout << "Answer:" << twoCount * threeCount << endl;
}
void puzzle3(fstream &input) {
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
    cout << "Answer:" << squareMeters << endl;

    for (auto &it : rects) {
        auto nooverlap = true;
        for (size_t x = it.second.x; nooverlap && x < (it.second.x + it.second.width); x++) {
            for (size_t y = it.second.y; nooverlap && y < (it.second.y + it.second.height); y++) {
                if (mat[x][y] != 1) nooverlap = false;
            }
        }
        if (nooverlap) cout << "No overlap claim:" << it.first << endl;
    }
}
void puzzle4(fstream &input) {
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
    map<int, int> mostMinutes;
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
    cout << "Puzzle part one answer:" << mostMinute * id << endl;
    cout << "Puzzle part two answer:" << guardMostMinuteId * guardMostMinute << endl;
}
void puzzle5(fstream &input) {
    string polymer;
    getline(input, polymer);

    const auto polymerReaction = [](string polymer) {
        size_t kr = 0;
        auto it = polymer.begin();
        auto nit = next(it);
        while (nit != polymer.end()) {
            if (abs(*it - *nit) == 32) {
                polymer.replace(it, nit + 1, "");
                if (kr > 0) --kr;
                it = next(polymer.begin(), kr);
            } else {
                ++it;
                ++kr;
            }
            nit = next(it);
        }
        return polymer.size();
    };
    cout << "Answer:" << polymerReaction(polymer) << endl;
    const auto reducePolymer = [](string polymer, const char ch) {
        auto pos = polymer.find(ch);
        while (pos != string::npos) {
            polymer.replace(pos, 1, "");
            pos = polymer.find(ch, pos);
        }
        return polymer;
    };

    auto shortestPolymer = polymer.size();
    for (auto ch = 'A'; ch <= 'Z'; ch++) {
        const auto reductedPolymerLength = polymerReaction(reducePolymer(reducePolymer(polymer, ch), ch + 32));
        if (shortestPolymer > reductedPolymerLength) {
            shortestPolymer = reductedPolymerLength;
        }
    }
    cout << "Shortest polymer length:" << shortestPolymer << endl;
}

void puzzle6(fstream &input) {
    struct Point {
        int x, y;
    };
    vector<Point> inputPoints;
    auto areaLeft = 10000;
    auto areaTop = 10000;
    auto areaRight = 0;
    auto areaBottom = 0;
    while (!input.eof()) {
        Point p;
        string line;
        getline(input, line);
        if (sscanf_s(line.c_str(), "%d, %d", &p.x, &p.y) == 2) {
            inputPoints.emplace_back(p);
            areaRight = max(areaRight, p.x);
            areaBottom = max(areaBottom, p.y);
            areaLeft = min(areaLeft, p.x);
            areaTop = min(areaTop, p.y);
        } else
            throw exception("Parsing error!");
    }
    areaLeft--;
    areaTop--;
    areaRight++;
    areaBottom++;
    const auto width = areaRight - areaLeft;
    const auto height = areaBottom - areaTop;
    vector<vector<int>> area(width, vector<int>(height, -1));

    struct PointsArea {
        vector<Point> points;
        vector<Point> lastAdded;
    };
    map<int, PointsArea> pointsAreas;
    for (auto i = 0; i < inputPoints.size(); i++) {
        const auto &p = inputPoints[i];
        const auto ap = Point{p.x - areaLeft, p.y - areaTop};
        area[ap.x][ap.y] = i;
        pointsAreas[i].points.emplace_back(p);
        pointsAreas[i].lastAdded.emplace_back(p);
    }
    const auto areaIsFilled = [&]() {
        auto filled = true;
        for (auto x = 0; x < width && filled; x++) {
            for (auto y = 0; y < height && filled; y++) {
                filled = area[x][y] != -1;
            }
        }
        return filled;
    };
    while (!areaIsFilled()) {
        for (auto &pa : pointsAreas) {
            auto &lastAdded = pa.second.lastAdded;
            if (lastAdded.size() > 0) {
                vector<Point> added;
                for (auto &p : lastAdded) {
                    const auto ap = Point{p.x - areaLeft, p.y - areaTop};
                    if (ap.x > 0 && area[ap.x - 1][ap.y] == -1) {
                        area[ap.x - 1][ap.y] = pa.first;
                        added.emplace_back(Point{p.x - 1, p.y});
                    }
                    if (ap.x < (width - 1) && area[ap.x + 1][ap.y] == -1) {
                        area[ap.x + 1][ap.y] = pa.first;
                        added.emplace_back(Point{p.x + 1, p.y});
                    }
                    if (ap.y > 0 && area[ap.x][ap.y - 1] == -1) {
                        area[ap.x][ap.y - 1] = pa.first;
                        added.emplace_back(Point{p.x, p.y - 1});
                    }
                    if (ap.y < (height - 1) && area[ap.x][ap.y + 1] == -1) {
                        area[ap.x][ap.y + 1] = pa.first;
                        added.emplace_back(Point{p.x, p.y + 1});
                    }
                }
                pa.second.points.insert(pa.second.points.end(), added.begin(), added.end());
                lastAdded = added;
            }
        }
    }

    vector<int> infiniteAreas;
    const auto addAreaIfNotAdded = [&](int id) {
        if (find(infiniteAreas.begin(), infiniteAreas.end(), id) == infiniteAreas.end()) {
            infiniteAreas.emplace_back(id);
        }
    };
    for (auto x = 0; x < width; x++) {
        addAreaIfNotAdded(area[x][0]);
        addAreaIfNotAdded(area[x][height - 1]);
    }
    for (auto y = 1; y < height - 1; y++) {
        addAreaIfNotAdded(area[0][y]);
        addAreaIfNotAdded(area[width - 1][y]);
    }
    size_t longestArea = 0;
    for (const auto &pa : pointsAreas) {
        if (find(infiniteAreas.begin(), infiniteAreas.end(), pa.first) == infiniteAreas.end() && longestArea < pa.second.points.size()) {
            longestArea = pa.second.points.size();
        }
    }

    cout << "Largest area: " << longestArea << endl;

    size_t regionSize = 0;
    for (auto x = areaLeft; x < areaRight; x++) {
        for (auto y = areaTop; y < areaBottom; y++) {
            auto totalDistance = 0;
            for (auto &p : inputPoints) {
                totalDistance += abs(x - p.x) + abs(y - p.y);
            }
            if (totalDistance < 10000) regionSize++;
        }
    }

    cout << "Region size(less than 10000): " << regionSize << endl;
}

void puzzle7(fstream &input) {
    string order, notAdded;
    struct CharTrees {
        string parents;
        string childs;
    };
    map<char, CharTrees> treeMap;
    while (!input.eof()) {
        string line;
        getline(input, line);
        auto parent = line[5];
        auto child = line[36];
        if (treeMap[child].parents.find(parent) == string::npos) treeMap[child].parents += parent;
        if (treeMap[parent].childs.find(child) == string::npos) treeMap[parent].childs += child;
    }
    for (auto &it : treeMap) {
        sort(it.second.childs.begin(), it.second.childs.end());
        sort(it.second.parents.begin(), it.second.parents.end());
        if (it.second.parents.size() == 0) notAdded += it.first;
    }

    auto i = 0;
    const auto charAdded = [&](char ch) { return order.find(ch) != string::npos || notAdded.find(ch) != string::npos; };
    while (!notAdded.empty()) {
        sort(notAdded.begin(), notAdded.end());
        i = 0;
        while (i < notAdded.size()) {
            auto &parents = treeMap[notAdded[i]].parents;
            if (count_if(parents.begin(), parents.end(), [&order](char ch) { return order.find(ch) != string::npos; }) == parents.size()) break;
            i++;
        }
        order += notAdded[i];
        auto toReplace = treeMap[notAdded[i]].childs;
        auto it = find_if(toReplace.begin(), toReplace.end(), charAdded);
        while (it != toReplace.end()) {
            toReplace.erase(it);
            it = find_if(toReplace.begin(), toReplace.end(), charAdded);
        }
        notAdded.replace(i, 1, toReplace);
    }

    for (auto &ch : order) {
        auto &parents = treeMap[ch].parents;
        auto &childs = treeMap[ch].childs;
        printf_s("%12s -> %c -> %8s\n", parents.c_str(), ch, childs.c_str());
    }

    cout << "Instructions order: " << order << endl;

    const auto charTime = [](char ch) { return 61 + ch - 'A'; };
    notAdded = order;
    order.clear();
    vector<vector<pair<char, size_t>>> work;
    while (!notAdded.empty()) {
        sort(notAdded.begin(), notAdded.end());
        i = 0;
        vector<int> canBeAdded, added;
        while (i < notAdded.size()) {
            auto &parents = treeMap[notAdded[i]].parents;
            if (count_if(parents.begin(), parents.end(), [&order](char ch) { return order.find(ch) != string::npos; }) == parents.size())
                canBeAdded.emplace_back(i);
            i++;
        }
        if (work.empty()) {
            work.emplace_back(5);
            auto &workers = work.back();
            for (i = 0; i < min(int(notAdded.size()), int(workers.size())) && !canBeAdded.empty(); i++) {
                workers[i] = make_pair(notAdded[canBeAdded[i]], 1);
            }
        } else {
            auto &workers = work.back();
            vector<int> freeWorkers;
            for (i = 0; i < 5; i++) {
                if (workers[i].first == 0 || charTime(workers[i].first) == workers[i].second) {
                    if (workers[i].first != 0) {
                        added.emplace_back(distance(canBeAdded.begin(), find(canBeAdded.begin(), canBeAdded.end(), workers[i].first)));
                    }
                    freeWorkers.emplace_back(i);
                } else if (workers[i].first != 0)
                    workers[i].second++;
            }
            work.emplace_back(workers);
            for (auto &fw : freeWorkers) {
                work.back()[fw].first = '\0';
                work.back()[fw].second = 0;
            }
        }

        for (auto &ai : added) {
            i = ai;
            order += notAdded[i];
            auto toReplace = treeMap[notAdded[i]].childs;
            auto it = find_if(toReplace.begin(), toReplace.end(), charAdded);
            while (it != toReplace.end()) {
                toReplace.erase(it);
                it = find_if(toReplace.begin(), toReplace.end(), charAdded);
            }
            notAdded.replace(i, 1, toReplace);
        }
    }

    cout << "Time to complete all steps: " << work.size() << endl;
}

int main(const int argc, char *argv[]) {
    if (argc > 1) {
        try {
            fstream input(argv[1]);
            if (input.is_open()) {
                puzzle7(input);
            }
        } catch (exception &e) {
            cout << e.what() << endl;
        }
    }
    system("pause");
    return 0;
}
