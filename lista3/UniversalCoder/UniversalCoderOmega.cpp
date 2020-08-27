//
// Created by erthax on 06.04.2020.
//

#include "UniversalCoder.h"
std::string UniversalCoderOmega::encode(int number){
    std::string s = UniversalCoder::decToBin(number), valueString = "0";

    while(number > 1){
        valueString = s + valueString;
        number = s.size() - 1;
        s = UniversalCoder::decToBin(number);
    }
    return valueString;
}

std::vector<int> UniversalCoderOmega::decode(std::string code){
    std::vector<int> result;
    int size = code.size(),i=0, n = 1,move_index = 0;
    while(i<size){
        n = 1;
        move_index = 0;
        while(code[i] != '0' && i<size-1){
            move_index = n+1;
            n = UniversalCoder::binToDec(code.substr(i,n+1));
            i += move_index;
        }
        if(i <= size){
            i++;
            result.push_back(n);
        }
    }

    return result;
}