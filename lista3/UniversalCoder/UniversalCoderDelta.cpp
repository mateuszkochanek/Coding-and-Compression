//
// Created by erthax on 06.04.2020.
//

#include "UniversalCoder.h"
std::string UniversalCoderDelta::encode(int number){
    UniversalCoderGamma coder;
    std::string result, s = UniversalCoder::decToBin(number);
    int size = s.size();
    std::string sizeBits = coder.encode(size);
    result = sizeBits + s.substr(1,size-1);
    return result;
}

std::vector<int> UniversalCoderDelta::decode(std::string code){
    UniversalCoderGamma coder;
    int sizeVal;
    std::vector<int> result;
    std::string valueString;
    int size = code.size(), zeroCount = 0,i=0;

    while(i<size){
        zeroCount = 0;
        while(code[i] == '0'){
            zeroCount++;
            i++;
        }
        if(i < size)
            sizeVal = binToDec(code.substr(i, zeroCount + 1));
        else
            break;
        i+=zeroCount+1;
        valueString = "1";
        while(sizeVal > valueString.size()){
            valueString.append(1,code[i]);
            i++;
        }
        result.push_back(binToDec(valueString));
    }

    return result;
}
