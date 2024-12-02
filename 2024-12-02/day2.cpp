#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>
#include <map>

std::pair<int,int> getNears(std::vector<int>& report, int i, int ig=-1) {
    int first = i == ig ? i+1 : i; 
    int second = (i == ig || i+1 == ig) ? i+2 : i+1;

    if (first > report.size()) first = report.size() - 1;
    if (second > report.size() - 1) second = report.size() - 1;

    return {report[first], report[second]};
}

bool stillMonotone(bool increasing, std::pair<int,int> nears) {
    return increasing && nears.first < nears.second
            || !increasing && nears.first > nears.second;
}

bool smallDifference(std::pair<int,int> nears) {
    int diff = std::abs(nears.first - nears.second);
    return diff <= 3 && diff >= 1;
}

bool isSafe (std::vector<int> report) {
    bool increasing = report.front() < report.back();
    bool safe_report = true;
    std::vector<int> toIgnore = {};

    for (int i = 0; i < report.size() - 1; ++i) {
        std::pair<int,int> nears = getNears(report, i);
        if (!stillMonotone(increasing, nears) || !smallDifference(nears)) { 
                safe_report = false;
                toIgnore.push_back(i);
                toIgnore.push_back(i+1);
        }
    }

    for(int j = 0; j < toIgnore.size() && !safe_report; ++j) {
        safe_report = true;
        std::pair<int,int> initials = getNears(report, 0, toIgnore[j]);
        increasing = initials.first < initials.second;
        for (int i = 0; i < report.size() - 1 && safe_report; ++i) {
            std::pair<int,int> nears = getNears(report, i, toIgnore[j]);
            safe_report = stillMonotone(increasing, nears) && smallDifference(nears);
        }
    }

    return safe_report;
}

int main() {
    std::ifstream file("input.txt");
    std::string line;
    std::vector<std::vector<int>> reports;

    while(std::getline(file, line)) {
        std::stringstream ss(line);
        reports.push_back({});
        int num;
        while(ss >> num) {
            reports.back().push_back(num);
        }
    }

    int safe_count = 0;
    for (auto& report : reports) {
        bool issafeee = isSafe(report);
        safe_count += issafeee;
        if (!issafeee) {
            for(int& n : report) {
               //std::cout << n << " ";
            }
            //std::cout << std::endl;
        }
    }

    std::cout << safe_count << std::endl;

    return 0;
}