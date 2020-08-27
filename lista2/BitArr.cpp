//
// Created by erthax on 26.03.2020.
//

#include "BitArr.h"

BitArr::BitArr(int num){
    bits = std::bitset<size> (num);
}

bool BitArr::checkBit(int index){
    return bits[size - index - 1];
}

void BitArr::moveBits(bool val){
    bits<<=1;
    bits[0] = val;
}

void BitArr::set(unsigned long long num){
    bits = std::bitset<size> (num);
}

void BitArr::set(int index, int val){
    bits[size - index - 1] = val;
}

int BitArr::getSize(){
    return size;
}

unsigned long long BitArr::getValue(){
    return bits.to_ullong();
}

