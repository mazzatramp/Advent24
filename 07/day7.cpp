#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>
#include <set>
#include <math.h>

std::vector<unsigned long long> splitBySpace(std::string toSplit) {
    std::vector<unsigned long long> values = {};
    std::stringstream ss{toSplit};
    std::string value;
    while(ss >> value) {
        values.push_back(std::stoull(value));
    }
    return values;
}

int countDigits(unsigned long long n) {
    int c = 1;
    while (n / 10 > 0) {
        n /= 10;
        c++;
    }
    return c;
}

unsigned long long calculate(std::vector<unsigned long long>& operands, std::string& operators) {
    int i = 1;
    unsigned long long temp = operands[i++];
    for(; i < operands.size(); ++i) {

        switch (operators[i-2])
        {
        case '+':
            temp += operands[i];
            break;
        case '*':
            temp *= operands[i];
            break;
        case '|':
            temp = temp * pow(10,countDigits(operands[i])) +  operands[i];
            break;
        }
    }
    return temp;
}

std::string vitos(std::vector<int> v) {
    std::ostringstream s;
    for (auto& a : v) s << a << " ";
    return s.str();
}

void construct(std::string& s, int n) {
    int base10 = n;
    int base3 = 0;

    for (int i = 0; i < s.size(); i++) {
        
        base3 = base10 % 3;
        base10 = base10 / 3;

        switch (base3)
        {
        case 0:
            s[i] = '+';
            break;
        case 1:
            s[i] = '*';
            break;
        case 2:
            s[i] = '|';
            break;
        }
    }
}

//cycle over n + n, n*(2n)
bool feasibleOperation(std::vector<unsigned long long> operands) {
    std::string operators = {};
    for (int i = 1; i< operands.size()-1; i++) operators.append("*");

    for (int i = 0; i < std::pow(3,operators.size()); ++i) { //for each possible set of operators
        construct(operators, i);
        if (calculate(operands, operators) == operands[0])
            return true;
    }
    return false;
}

int main(int argc, char* argv[]) {
    std::string filename = (argc > 1) ? argv[1] : "input.txt";
    std::ifstream inputstream{filename};
    std::string line;

    unsigned long long sum = 0;
    while(std::getline(inputstream, line)) {
        std::vector<unsigned long long> operands = splitBySpace(line);
        if(feasibleOperation(operands))
            sum += operands[0];
    }
    std::cout << sum <<  std::endl;    
    return 0;
}