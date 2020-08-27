//
// Created by erthax on 26.03.2020.
//

#ifndef KODER_BITARR_H
#define KODER_BITARR_H

#include <iostream>
#include <bitset>

class BitArr {
private :
    static const int size = 32;
public:
    std::bitset<size> bits; // w public na czas testów
    BitArr(int i);
    bool checkBit(int index);
    void moveBits(bool val);
    void set(unsigned long long num);
    void set(int index, int val);
    static int getSize();
    unsigned long long getValue();

};

#endif //KODER_BITARR_H