#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>
#include <map>

int shiftIndexOnThreshold(int i, int th) {
    if ( i >= th) return i+1;
    else return i;
}

bool stillMonotone(bool increasing, const std::pair<int,int> nears) {
    return increasing && nears.first < nears.second
            || !increasing && nears.first > nears.second;
}

bool acceptableDiff(int a, int b) {
    int diff = std::abs(a - b);
    return diff >= 1 && diff <= 3;
}

int monotony(int a, int b) {
  if (a == b) {
    return 0;  // No change
  } else if (b > a) {
    return 1;  // Increasing
  } else {
    return -1; // Decreasing
  }
}

bool sameMonotony(int a1, int b1, int a2, int b2) {
    return monotony(a1,b1) == monotony(a2,b2);
}

std::vector<int> getErrorIndexes(const std::vector<int>& r) {
    std::vector<int> errorIndexes = {};

    for (size_t i = 0; i < r.size() - 2; ++i) {
        if (!sameMonotony(r[0], r[1], r[i], r[i+1]) || !acceptableDiff(r[i], r[i+1])) { 
            errorIndexes.push_back(i);
            errorIndexes.push_back(i+1);
        }
    }
    return errorIndexes;
}

bool isSafeIgnoring(const std::vector<int> r, size_t i_ignore) {
    if (r.size() <= 2)
        return true;
    
    if (i_ignore == r.size() -1 ) return false;

    for (size_t i = 0, j = 1; j < r.size() - 1; ++i, ++j) {
        if (i == i_ignore) {j++; continue;};
        if (j == i_ignore) j++;

        if (!sameMonotony(r[0], r[1], r[i], r[j]) || !acceptableDiff(r[i], r[j])) { 
            return false;
        }
    }
    return true;
}

bool isSafe(const std::vector<int> report) {
    std::vector<int> errIndexes = getErrorIndexes(report);
    for(auto& error : errIndexes) {
        if (isSafeIgnoring(report, error))
            return true;
    }
    return errIndexes.empty();
}

std::vector<int> splitBySpace(std::string toSplit) {
    std::vector<int> values = {};
    std::stringstream ss{toSplit};
    int value;
    while(ss >> value)
        values.push_back(value);
    return values;
}

std::vector<std::vector<int>> extractReports(std::string& filename) {
    std::vector<std::vector<int>> reports;
    std::ifstream file{filename};
    std::string line;

    while(std::getline(file, line)) {
        reports.push_back(splitBySpace(line));
    }
    return reports;
};

int countSafeReports(std::vector<std::vector<int>>& reports) {
    int safe_count = 0;
    for (auto& report : reports)
        safe_count += isSafe(report);
    return safe_count;
}

int main(int argc, char* argv[]) {
    std::string report_filename = (argc == 2) ? argv[1]:  "input.txt";
    std::vector<std::vector<int>> reports = extractReports(report_filename);
    std::cout << countSafeReports(reports) << std::endl;
    return 0;
}