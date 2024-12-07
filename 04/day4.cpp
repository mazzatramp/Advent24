#include <string>
#include <iostream>
#include <vector>
#include <fstream>

std::vector<std::string> extractMatrix (std::string filename) {
    std::ifstream inputstream{filename};
    std::vector<std::string> matrix;
    std::string line;
    while(std::getline(inputstream, line)) {
        matrix.push_back(line);
    }
    return matrix;
}

bool findXmas(std::vector<std::string> matrix, int line, int col, std::pair<int,int> dir) {
    return line + dir.first * 3 < matrix.size() && line + dir.first * 3 >= 0 &&
        col + dir.second * 3 < matrix[0].size() && col + dir.second * 3 >= 0 &&
        matrix[line + dir.first*1][col + dir.second*1] == 'M' &&
        matrix[line + dir.first*2][col + dir.second*2] == 'A' &&
        matrix[line + dir.first*3][col + dir.second*3] == 'S';
}

int countXmasOccurences(std::vector<std::string> matrix) {
    int count = 0;
    std::vector<std::pair<int,int>> directions = {
        { -1,  1},{  0,  1},{  1,  1},
        { -1,  0},          {  1,  0},
        { -1, -1},{  0, -1},{  1, -1},  
    };

    for (int line = 0; line < matrix.size(); ++line) {
        for (int col = 0; col < matrix[line].size(); ++col) {
            if (matrix[line][col] == 'X') {
                for (auto& dir : directions) {
                    count += findXmas(matrix, line, col, dir);
                }
            }
        }
    }

    return count;
}

bool findMas(std::vector<std::string> matrix, std::pair<int,int> posA, std::pair<int,int> dir) {
    if (posA.first + dir.first >= 0 &&
        posA.second + dir.second >= 0 &&
        posA.first + dir.first < matrix.size() &&
        posA.second + dir.second < matrix.size() &&
        posA.first - dir.first >= 0 &&
        posA.second - dir.second >= 0 &&
        posA.first - dir.first < matrix.size() &&
        posA.second - dir.second < matrix.size())
    {
        return (matrix[posA.first + dir.first][posA.second + dir.second] == 'M' &&
        matrix[posA.first - dir.first][posA.second - dir.second] == 'S') ||
        (matrix[posA.first + dir.first][posA.second + dir.second] == 'S' &&
        matrix[posA.first - dir.first][posA.second - dir.second] == 'M');
    }
    return false;
}

int countMasXFormations(std::vector<std::string> matrix) {
    int count = 0;
    for (int line = 0; line < matrix.size(); ++line) {
        for (int col = 0; col < matrix[line].size(); ++col) {
            if (matrix[line][col] == 'A') {
                if (findMas(matrix, {line, col}, {1, 1}) &&
                    findMas(matrix, {line, col}, {-1, 1})) count++;
            }
        }
    }

    return count;
}

int main(int argc, char* argv[]) {
    std::string filename = (argc == 2) ? argv[1] : "input.txt";
    std::vector<std::string> xmasMatrix = extractMatrix(filename);
    std::cout << countXmasOccurences(xmasMatrix) << std::endl;
    std::cout << countMasXFormations(xmasMatrix) << std::endl;
    return 0;
}