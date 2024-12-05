#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <map>
#include <set>

std::pair<int,int> splitRule(std::string rule) {
    int iBar = rule.find("|");
    std::string n1 = rule.substr(0, iBar);
    std::string n2 = rule.substr(iBar+1, rule.size()-1);
    return {std::stoi(n1),std::stoi(n2)};
}

std::vector<int> splitByComma(std::string str) {
    int prevComma = 0;
    int succComma = str.find(",");
    std::vector<int> pages{};
    
    while (succComma != -1) {
        pages.push_back(std::stoi(str.substr(prevComma, succComma)));
        prevComma = succComma+1;
        succComma = str.find(",", prevComma);
    }
    
    pages.push_back(std::stoi(str.substr(prevComma, str.size())));
    return pages;
}

std::multimap<int, int> readRules(std::ifstream& rulestream) {
    std::multimap<int, int> rules;

    std::string line;
    while (std::getline(rulestream, line) && line != "") {
        std::pair<int, int> rulePages = splitRule(line);
        rules.insert(rulePages);
    }
    return rules;
}

std::vector<std::vector<int>> readManuals(std::ifstream& rulestream) {
    std::vector<std::vector<int>> manuals;
    std::string line;

    while (std::getline(rulestream, line)) {
        manuals.push_back(splitByComma(line));
    }
    return manuals;
}

std::pair<int,int> getUnrespectedRule(std::vector<int>& manual, std::multimap<int,int> rules) {
    std::set<int> seenPages{};
    for(int page : manual) {
        std::set<int> unallowablePages = {};
        for(auto& rule : rules) {
            if (rule.first == page && seenPages.count(rule.second) > 0) {
                return rule;
            }
        }

        seenPages.insert(page);
    }
    return {-1,-1};
}

void swapp(std::vector<int>& man, std::pair<int,int> pagesToSwap) {
    auto it1 = std::find(man.begin(), man.end(), pagesToSwap.first);
    auto it2 = std::find(man.begin(), man.end(), pagesToSwap.second);
    int a = (*it1);
    (*it1) = (*it2);
    (*it2) = a;
}

int main(int argc, char* argv[]) {
    
    std::string report_filename = (argc == 2) ? argv[1]:  "input.txt";
    std::ifstream inputfile{report_filename};

    std::multimap rules = readRules(inputfile);
    std::vector<std::vector<int>> manuals = readManuals(inputfile);

    int orderedMiddles = 0;
    int reorderedMiddles = 0;

    for (auto& manual : manuals) {
        std::pair<int,int> toSwap = getUnrespectedRule(manual, rules);

        if (toSwap.first == -1) {
            orderedMiddles += manual[(int)manual.size()/2];
        }
        else {
            while (toSwap.first >= 0) {
                swapp(manual, toSwap);
                toSwap = getUnrespectedRule(manual, rules);
            }
            reorderedMiddles += manual[(int)manual.size()/2];
        }
    }
    std::cout << orderedMiddles   << std::endl;
    std::cout << reorderedMiddles << std::endl;
    
    return 0;
}