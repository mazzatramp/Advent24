#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>
#include <map>

int main() {
    std::ifstream file("input.txt");
    std::string line;
    std::vector<int> left;
    std::vector<int> right;

    while(std::getline(file, line)) {
        std::stringstream ss(line);
        int num1, num2;
        ss >> num1 >> num2;
        left.push_back(num1);
        right.push_back(num2);
    }

    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());

    std::vector<int>::iterator it1 = left.begin();
    std::vector<int>::iterator it2 = right.begin();
    int totalDiff = 0;

    while(it1 != left.end()) {
        totalDiff += *it1 > *it2 ? *it1 - *it2 : *it2 - *it1;
        it1++;
        it2++;
    }

    std::map<int, int> calculated_similarities = {};
    int similarity = 0;
    for (it1 = left.begin(); it1 != left.end(); it1++) {
        if (calculated_similarities.find(*it1) == calculated_similarities.end()) {
            calculated_similarities[*it1] = *it1 * std::count(right.begin(), right.end(), *it1);
        }
        similarity += calculated_similarities[*it1];
    }
    
    std::cout << totalDiff << std::endl;
    std::cout << similarity << std::endl;
    return 0;
}