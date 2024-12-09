#include <map>
#include <set>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

int between(pair<int,int> n, pair<int,int> low, pair<int,int> high) {
    return n.first  >= low.first  && n.first  < high.first
        && n.second >= low.second && n.second < high.second;
}

void extractInput(std::string filename, map<char, set<pair<int,int>>>& frequencies, size_t& inputsize) {
    std::ifstream inputstream{filename};
    int x = 0, y = 0;
    char readchar;
    while (inputstream.get(readchar)) {
        if (isalnum(readchar)) {
            frequencies[readchar].insert({x,y});
        }

        if (readchar == '\n') {x = 0; y++;}
        else {x++;}
    }
    inputsize = x;
}

set<pair<int,int>> calculateAntinodes (pair<int,int> n1, pair<int,int> n2, size_t mapsize) {
    set<pair<int,int>> ans;

    pair<int,int> diff = {n2.first-n1.first, n2.second-n1.second}; 
    pair<int,int> an1 = n1;
    pair<int,int> an2 = n2;

    while(between(an1, {0,0}, {mapsize, mapsize})) {
        ans.insert(an1);
        an1 = {an1.first - diff.first, an1.second - diff.second};
    }

    while(between(an2, {0,0}, {mapsize, mapsize})) {
        ans.insert(an2);
        an2 = {an2.first + diff.first, an2.second + diff.second};
    }

    return ans;
}


set<pair<int,int>> getAntinodes(const map<char, set<pair<int,int>>>& frequencies, size_t mapsize) {
    
    set<pair<int,int>> antinodes;

    for(const auto& freq : frequencies) {
        const set<pair<int,int>>& nodes = freq.second;

        for(auto n1 = nodes.begin(); n1 != nodes.end(); ++n1) {
            for(auto n2 = n1; n2 != nodes.end(); ++n2) {
                if (n1 == n2) continue;
                auto calculated = calculateAntinodes(*n1, *n2, mapsize);
                for (auto& c : calculated) {
                    antinodes.insert(c);
                }
            }
        }
    }
    
    return antinodes;
}

int main(int argc, char* argv[]) {

    string filename = (argc > 1) ? argv[1] : "input.txt";
    map<char, set<pair<int,int>>> frequencies;
    size_t mapsize, count = 0;

    extractInput(filename, frequencies, mapsize);

    set<pair<int,int>> antinodes = getAntinodes(frequencies, 50);

    //for (auto& an : antinodes) cout << an.first << " " << an.second << endl;

    cout << antinodes.size() << endl;

    for(int i = 0; i<mapsize; i++) {
        for(int j = 0; j<mapsize; j++) {
            if (antinodes.find({j,i}) != antinodes.end()) {
                cout << "#";
            }
            else cout << "."; 
        }
        cout << endl;
    }


    return 0;
}