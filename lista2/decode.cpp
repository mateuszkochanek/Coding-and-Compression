#include <iostream>
#include <fstream>
#include <map>
#include <cmath>
#include <bitset>
#include "BitArr.h"
using namespace std;

string buffer,result;
int bufferCount;

void setLimits(BitArr *up, BitArr *down, unsigned long long int totalCount, unsigned long long int cumulatieCount,
               int &currCharWeight);

void decode(BitArr *up, BitArr *down,BitArr *t,std::bitset<8>* actualChar, int* actualCharIndex);

int main(int argc, char *argv[]) {
    buffer = "", result = "",bufferCount = 0;
    int i,j,actualCharIndex,decodedChar = 0,k;
    ifstream inputFile;
    ofstream outputFile;
    if(argc >= 3){
        inputFile.open(argv[1]);
        outputFile.open(argv[2]);
    } else {
        inputFile.open("");
        outputFile.open("");
    }
    char currChar;
    unsigned long long cumulativeCount, totalCount = 257, maxVal = pow(2,BitArr::getSize())-1,determinant = 0;
    map <int,int> charWeights;
    map <int,int>::iterator it;
    BitArr* up = new BitArr(maxVal), *down = new BitArr(0), *t = new BitArr(0);

    for(i = 0; i < 257; i++)
        charWeights[i] = 1;

    while(inputFile >> noskipws >> currChar){
        buffer.append(1, currChar);
    }

    for(i=0,bufferCount = 0; i < up->getSize();bufferCount++){
        std::bitset<8> actualChar((int)(buffer[bufferCount]));
        actualCharIndex=7;
        do{
            t->set(i,actualChar[actualCharIndex]);
            i++;
            actualCharIndex--;
        } while(i%8 != 0 && actualCharIndex>=0);
    }

    std::bitset<8> actualChar((int)(buffer[bufferCount]));
    bufferCount++;
    actualCharIndex=7;
    //actualChar =std::bitset<8> ((int)(buffer[bufferCount]));

    while(bufferCount < buffer.size()){
        it = charWeights.begin();
        cumulativeCount = it->second;
        determinant = ((t->getValue() - down->getValue() + 1) * totalCount -1)
                /(up->getValue() - down->getValue() + 1);
        while(determinant >= cumulativeCount){
            it++;
            cumulativeCount += it->second;
        }
        decodedChar = it->first;
        if(decodedChar == 256) // powinno byc 256 ale ^^^ tam odejmuje wiec zeby zobaczyc wynik muszialem tu dac 255 (inaczej petla nieskonczona)
            break;

        result = result + (char)decodedChar;
        setLimits(up,down,totalCount,cumulativeCount,charWeights[(int)decodedChar]);
        decode(up, down, t, &actualChar, &actualCharIndex);
        charWeights[decodedChar]++;
        totalCount++;
    }
    outputFile << result;
    outputFile.close();
    return 0;
}

void decode(BitArr *up, BitArr *down,BitArr *t,std::bitset<8>* actualChar, int* actualCharIndex) {
    char bit,bitNeg;
    int  upBitVal,downBitVal,tBitVal;
    while(true){
        if(up->checkBit(0) == down->checkBit(0)){
            down->moveBits(0);
            up->moveBits(1);
            if(*actualCharIndex < 0){
                *actualChar =std::bitset<8> ((int)(buffer[bufferCount]));
                bufferCount++;
                (*actualCharIndex)=7;
            }
            t->moveBits((*actualChar)[*actualCharIndex]);
            (*actualCharIndex)--;
        } else if((up->checkBit(0) != down->checkBit(0)) && (up->checkBit(1) == 0) && (down->checkBit(1) == 1)) {
            down->moveBits(0);
            up->moveBits(1);
            if(*actualCharIndex < 0){
                *actualChar =std::bitset<8> ((int)(buffer[bufferCount]));
                bufferCount++;
                (*actualCharIndex)=7;
            }
            t->moveBits((*actualChar)[*actualCharIndex]);
            (*actualCharIndex)--;

            down->set(0,0);
            up->set(0,1);
            t->set(0,1 - t->checkBit(0));
        } else {
            return;
        }
    }
}

void setLimits(BitArr *up, BitArr *down, unsigned long long int totalCount, unsigned long long int cumulativeCount,
               int &currCharWeight) {
    unsigned long long val, downVal = down->getValue(), upVal = up->getValue();
    down->set(downVal + ((upVal - downVal + 1) * (cumulativeCount - currCharWeight))/totalCount);
    up->set(downVal + ((upVal - downVal + 1) * cumulativeCount)/totalCount - 1);
}