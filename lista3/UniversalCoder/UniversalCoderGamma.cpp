//
// Created by erthax on 06.04.2020.
//

#include "UniversalCoder.h"
std::string UniversalCoderGamma::encode(int number){
    std::string s = UniversalCoder::decToBin(number);
    int zeroCount = s.size() - 1;
    while(zeroCount > 0){
        s = "0" + s;
        zeroCount--;
    }
    return s;
}

std::vector<int> UniversalCoderGamma::decode(std::string code){
    std::vector<int> result;
    int size = code.size(), zeroCount = 0,i=0;

    while(i<size){
        zeroCount = 0;
        while(code[i] == '0'){
            zeroCount++;
            i++;
        }
        if(i < size)
            result.push_back(binToDec(code.substr(i, zeroCount + 1)));
        i+=zeroCount+1;
    }
    return result;
}