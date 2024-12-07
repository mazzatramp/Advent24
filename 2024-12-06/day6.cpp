#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <map>
#include <set>

void extractMap(std::string filename, std::vector<std::string>& map, std::pair<int,int>& pos) {
    std::ifstream inputstream(filename);
    std::string line;
    int y=0;
    while (std::getline(inputstream, line)) {
        if (line.empty())
            return;
        
        map.push_back(line);
        int x = 0;
        for (char c : line) {
            if (c == '^')
                pos = {x, y};
            x++;
        }
        y++;
    }
}

bool between(std::pair<int,int> p, int a, int b) {
    return p.first >= a && p.first < b && p.second >= a && p.second < b;
}

std::pair<int,int> sumPair (const std::pair<int,int>& p1, const std::pair<int,int>& p2) {
    return {p1.first + p2.first, p1.second + p2.second };
}

char at(const std::vector<std::string>& map, std::pair<int,int> pos) {
    return map[pos.second][pos.first];
}

std::pair<int,int> rot90Clockwise(std::pair<int,int> pos) {
    // x = y, y = -x
    return {-pos.second, pos.first};
}

bool obstructedPath(std::vector<std::string>& map,
                    std::pair<int,int> pos,
                    std::pair<int,int> dir,
                    std::pair<int,int> newObs = {-1,-1} )
{
    if (!between(sumPair(pos, dir), 0, map.size()))
        return false;

    if (at(map, sumPair(pos, dir)) == '#')
        return true;

    if (sumPair(pos, dir).first == newObs.first && sumPair(pos, dir).second == newObs.second)
        return true;

    return false;
}

static std::set<std::pair<int,int>> obstacles{};

bool anObstacleWouldLoop(std::vector<std::string> map,
                         std::pair<int,int> pos,
                         std::pair<int,int> dir,
                         std::map<std::pair<int,int>, std::set<std::pair<int,int>>> seenCells)
{
    std::pair<int,int> newObstacle = sumPair(pos,dir);

    dir = rot90Clockwise(dir);
    while(between(pos, 0, map.size())) {
        if (seenCells.at(dir).find(pos) != seenCells.at(dir).end()) { //Already visited cell with same direction.
            obstacles.insert(newObstacle);
            return true;
        }

        if (obstructedPath(map, pos, dir, newObstacle))
            dir = rot90Clockwise(dir);
        else pos = sumPair(pos, dir);
        
    }
    return false;
}

int countVisitedCells(std::vector<std::string>& map, std::pair<int,int> startPos) {
    std::pair<int,int> dir = {0, -1};
    std::pair<int,int> pos = startPos;

    std::set<std::pair<int,int>> seenCells = {};

    std::map<std::pair<int,int>, std::set<std::pair<int,int>>> seenCellsByDir{};
    
    seenCellsByDir.insert({{0,1}, {}});
    seenCellsByDir.insert({{0,-1}, {}});
    seenCellsByDir.insert({{1, 0}, {}});
    seenCellsByDir.insert({{-1, 0}, {}});

    int countPossibleObstructions = 0;

    while (between(pos, 0, map.size())) {
        seenCells.insert(pos);
        seenCellsByDir.at(dir).insert(pos);

        if (obstructedPath(map, pos, dir)) {
            dir = rot90Clockwise(dir);
        }
        else {
            if (anObstacleWouldLoop(map, pos, dir, seenCellsByDir))
                countPossibleObstructions++;
            pos = sumPair(pos, dir);
        }

    }

    std::cout << countPossibleObstructions << std::endl;

    return seenCells.size();
}

int main(int argc, char* argv[]) {
    std::pair<int,int> guardPos{};
    std::vector<std::string> map;

    std::string filename = (argc > 1) ? argv[1] : "input.txt";

    extractMap(filename, map, guardPos);
    std::cout << countVisitedCells(map, guardPos) << std::endl;
    std::cout << obstacles.size() << std::endl;

    return 0;
}