#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>
#include <map>

/*
"Our computers are having issues, so I have no idea if we have any Chief Historians in stock! You're welcome to check the warehouse, though," says the mildly flustered shopkeeper at the North Pole Toboggan Rental Shop. The Historians head out to take a look.

The shopkeeper turns to you. "Any chance you can see why our computers are having issues again?"

The computer appears to be trying to run a program, but its memory (your puzzle input) is corrupted. All of the instructions have been jumbled up!

It seems like the goal of the program is just to multiply some numbers. It does that with instructions like mul(X,Y), where X and Y are each 1-3 digit numbers. For instance, mul(44,46) multiplies 44 by 46 to get a result of 2024. Similarly, mul(123,4) would multiply 123 by 4.

However, because the program's memory has been corrupted, there are also many invalid characters that should be ignored, even if they look like part of a mul instruction. Sequences like mul(4*, mul(6,9!, ?(12,34), or mul ( 2 , 4 ) do nothing.

For example, consider the following section of corrupted memory:

xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5))
Only the four highlighted sections are real mul instructions. Adding up the result of each instruction produces 161 (2*4 + 5*5 + 11*8 + 8*5).

Scan the corrupted memory for uncorrupted mul instructions. What do you get if you add up all of the results of the multiplications?
*/

int countCorrectExpressions(std::string filename) {
    std::ifstream inputstream{filename};
    std::vector<int> values = {};
    int value;
    std::string line;
    char ch;

    int state = 0;
    int n1 = 0;
    int n2 = 0;
    int totalSum = 0;
    bool enabled = true;

    while(inputstream.get(ch)) {

        switch (state)
        {
        case 1:
            if (ch == 'u') state++;
            else state = 0;
            break;
        case 2:
            if (ch == 'l') state++; 
            else state = 0;           
            break;
        case 3:
            if (ch == '(') state++;
            else state = 0;
            break;
        case 4: //find first n and comma
            if (ch >= '0' && ch <= '9' && n1 < 100) {
                n1 = 10 * n1 + (ch - '0');
            } else if (ch == ',') {
                state++;
            } else {
                n1 = 0;
                n2 = 0;
                state = 0;
            };
            break;
        case 5: //find second n and end
            if (ch >= '0' && ch <= '9' && n2 < 100) {
                n2 = 10 * n2 + (ch - '0');
            } else if (ch == ')') {
                totalSum = totalSum + (n1 * n2);
                //std::cout << n1 << " " << n2 << " " << totalSum << std::endl;
                n1 = 0;
                n2 = 0; 
                state = 0;
            } else {
                n1 = 0;
                n2 = 0; 
                state = 0;
            }
            break;
        case 10: //d found
            if (ch == 'o') state++;
            else state = 0;
            break;

        case 11:
            if (ch == '(') state++; //enable
            else if (ch == 'n') state  = 21; //disable
            else state = 0;
            break;
        case 12:
            if (ch == ')') {
                enabled = true;
            }
            state = 0;
            break;
        
        case 21:
            if (ch == '\'') state++;
            else state = 0;
            break;
        case 22:
            if (ch == 't') state++;
            else state = 0;
            break;
        case 23:
            if (ch == '(') state++;
            else if (ch == 'n') state  = 20;
            else state = 0;
            break;
        case 24:
            if (ch == ')') {
                enabled = false;
            }
            state = 0;
            break;
        }

        if (enabled && ch == 'm') {
            state = 1;
        }
        if (ch == 'd') {
            state = 10;
        }
    }
    return totalSum;
};

int main(int argc, char* argv[]) {
    std::string report_filename = (argc == 2) ? argv[1]:  "input.txt";
    std::cout << countCorrectExpressions(report_filename) << std::endl;
    return 0;
}