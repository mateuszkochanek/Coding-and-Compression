//
// Created by erthax on 06.04.2020.
//

#include "UniversalCoder.h"

std::string UniversalCoder::decToBin(int n){
    std::string r;
    while(n!=0) {r=(n%2==0 ?"0":"1")+r; n/=2;}
    return r;
}

int UniversalCoder::binToDec(std::string number){
    return std::stoi(number, nullptr, 2);
}
