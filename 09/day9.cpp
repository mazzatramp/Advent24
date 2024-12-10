#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <set>

using namespace std;

struct dblock {
    int id = -1;
    int size = 0;
    dblock* next = nullptr;
    dblock* prev = nullptr;
};

void extractData(string filename, dblock& first, dblock& last) {
    ifstream inputstream{filename};
    char c;
    bool free = false;
    int id = 0;

    dblock* curr = &first;

    while(inputstream.get(c)) {
        int readsize = c - '0';
        
        curr->size = readsize;

        if (!free)
            curr->id = id++;
        free = !free;

        curr->next = new dblock;
        curr->next->prev = curr;
        curr = curr->next;
    }
    last = *curr;
}

void printDisk(dblock& b) {
    dblock* curr = &b;
    while (curr != nullptr) {
        int printed = 0;
        for (int i = 0; i < curr->size - printed; ++i) {
            if (curr->id >= 0)
                cout << curr->id;
            else
                cout << ".";
        }
        curr = curr->next;
    }
    cout << endl;
}

int freeSpace(dblock* b) {
    return (b->id < 0) ? b->size : 0;
}

bool isFreeBlock(dblock* b) {
    return b->id < 0;
}

/*
void fill(dblock* f, dblock* l) {
    while (f != l) {
        if (!isFreeBlock(f)) {
            f = f->next;
        } else if (isFreeBlock(l)) {
            l = l->prev;
        } else {
            //f is free, l is not free

            // f > l => allocable = l;
            
            if (f->size <= l->size) {
                //can allocate everything
            }
            else {
                //how much can i allocate?
                size_t allocable = f->size - l->size;
                //create two new datablock of that size
                //one free, one filles
                //resize l
                //like filled to f
                //link free to l

            }

            f->ids.push_back(l->ids.back());
            l->ids.pop_back();
        }
    }
}
*/

unsigned long checksum(dblock* b) {
    unsigned long temp = 0;

    int pos = 0;
    while (b->next != nullptr) {
        for(int i = 0; i < b->size; ++i) {
            if (b->id >= 0)
                temp += (pos)*(b->id);
            pos++;
        }
        b = b->next;
    } 
    cout << endl;
    return temp;
}

map<size_t, vector<dblock*>> groupFreeSpaceBySize(dblock* b) {
    int pos = 0;
    map<size_t, vector<dblock*>> groups;
    while (b->next != nullptr) {
        if (b->id < 0) //free space
            groups[b->size].push_back(b);
        b = b->next;
    } 
    return groups;
}

bool fits(dblock* b1, dblock* b2) {
    return b2->size >= b1->size;
}

bool allocate(dblock* data, dblock* free) {
    if (!fits(data, free)) return false;

    size_t to_allocate = data->size;
    size_t still_free = free->size - to_allocate;

    if (to_allocate == 0) {
        free->id = data->id;
        data->id = -1;
    } else {
        //free divided in two
        dblock* oldNext = free->next;
        free->id = data->id;
        free->size = to_allocate;
        free->next = new dblock;
        free->next->id = -1;
        free->next->size = still_free;
        free->next->prev = free;
        free->next->next = oldNext;
        if (oldNext != nullptr)
            free->next->next->prev = free->next;
        
        //old data now free
        data->id = -1;
    }
    return true;
}



bool searchAndAlloc(dblock* toMove, dblock* searchFree) {
    while(toMove != searchFree) {
        if (isFreeBlock(searchFree) && fits(toMove, searchFree))
            return allocate(toMove, searchFree);
        searchFree = searchFree->next;
    }
    return false;
}

void printBySize(map<size_t, vector<dblock*>>& toMove) {
    for (auto& b : toMove) {
        cout << "[size " << b.first << "] ";
        for (auto& b2 : b.second)
            cout << b2->id << ", ";
        cout << "\t";
    }
    cout << endl;
}

void optimizeAlloc(dblock* start, dblock* end) {
    while(end != start) {
        if (!isFreeBlock(end))
            searchAndAlloc(end, start);
        end = end->prev;
    }
}

int main(int argc, char* argv[]) {
    
    string filename = (argc > 1) ? argv[1] : "input.txt";

    dblock first, last;
    extractData(filename, first, last);
    dblock* curr = &first;
    
    //printDisk(first);
    optimizeAlloc(&first, &last);
    printDisk(first);
    
    cout << checksum(&first)<< endl;
    return 0;
}

