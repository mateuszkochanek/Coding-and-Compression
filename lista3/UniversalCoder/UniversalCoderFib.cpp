//
// Created by erthax on 06.04.2020.
//

#include "UniversalCoder.h"
std::string UniversalCoderFib::encode(int number){
    std::string result = "1";
    int prevFib = 1, currFib = 1,tempFib;

    while(currFib <= number){
        tempFib = prevFib;
        prevFib = currFib;
        currFib = currFib + tempFib;
    }

    tempFib = prevFib;
    prevFib = currFib - prevFib;
    currFib = tempFib;

    while (prevFib > 0) {
        if (currFib <= number) {
            result.insert(0,"1");
            number -= currFib;
        } else {
            result.insert(0,"0");
        }

        tempFib = prevFib;
        prevFib = currFib - prevFib;
        currFib = tempFib;
    }
    return result;
}

std::vector<int> UniversalCoderFib::decode(std::string code){
    while (code[code.size()-1] == '0'){
        code.pop_back();
    }
    std::vector<int> result;
    int size = code.size(),prevFib = 1, currFib = 1,tempFib, i=0,resNum;
    while(i<size){
        resNum = 0;
        prevFib = 1;
        currFib = 1;
        while(code[i]=='0' || code[i+1]=='0'){
            if(code[i]=='1'){
                resNum+=currFib;
            }
            tempFib = prevFib;
            prevFib = currFib;
            currFib = currFib + tempFib;
            i++;
        }
        resNum+=currFib;
        result.push_back(resNum);
        i+=2;
    }

    return result;
}
