#include <iostream>
#include <fstream>
#include <map>
#include <cmath>
#include <filesystem>

#include "BitArr.h" // moja struktura do operacji na bitach
using namespace std;

//string z wynikiem
string buffer;

void setLimits(BitArr *up, BitArr *down, unsigned long long int totalCount, unsigned long long int cumulatieCount,
               int &currCharWeight);
void encode(BitArr *up, BitArr *down, unsigned long long int* scale);
void finalizeEncoding(BitArr *up, BitArr *down, map<int, int> charWeights, unsigned long long int totalCount,
                      unsigned long long int* scale);
double getEntropy(map<int, int>&charWeights, int sum);
double getCompressionRate(const string& plaintext, const string& ciphertext);

int main(int argc, char *argv[]) {
    buffer = "";
    ifstream inputFile;
    ofstream outputFile;
    if(argc >= 3){
        inputFile.open(argv[1]);
        outputFile.open(argv[2]);
    } else {
        inputFile.open("");
        outputFile.open("");
    }
    
    uint8_t currChar;
    double compressionRate = 0;
    unsigned long long cumulativeCount, totalCount = 257, maxVal = pow(2,BitArr::getSize())-1,scale=0,plaintextLength=0,codeLength=0;
    map <int,int> charWeights,codeCharWeights;
    map <int,int>::iterator it;
    BitArr* up = new BitArr(maxVal), *down = new BitArr(0);

    for(int i = 0; i < 257; i++)
        charWeights[i] = 1;

    while(inputFile >> noskipws >> currChar){
        plaintextLength++;
        cumulativeCount = 0;
        it = charWeights.begin();
        while(it->first != currChar){
            cumulativeCount += it->second;
            it++;
        }
        setLimits(up,down,totalCount,cumulativeCount,charWeights[currChar]);
        encode(up, down, &scale);
        charWeights[currChar]++;
        totalCount++;
    }

    cumulativeCount = 0;
    it = charWeights.begin();
    while(it->first != 256){
        cumulativeCount += it->second;
        it++;
    }
    setLimits(up,down,totalCount,cumulativeCount,charWeights[256]);
    encode(up, down, &scale);
    charWeights[256]++;
    totalCount++;

    finalizeEncoding(up, down, charWeights, totalCount, &scale); 
    string temp = "";

    for(int i = 0; i < buffer.size(); i++){ 
        if(temp.size() == 8){
            codeLength++;
            codeCharWeights[(char)(stoi(temp, nullptr,2))]++;
            outputFile << (char)(stoi(temp, nullptr,2));
            temp = "";
        }

        temp = temp + buffer[i];
    }

    outputFile << (char)(stoi(temp,0,2));
    outputFile.close();
    inputFile.close();
    codeLength++;

    cout << "Encoded file plaintext length: " << plaintextLength << endl;
    cout << "Encoded file coded text length: " << codeLength << endl;
    compressionRate = getCompressionRate(argv[1], argv[2]);
    cout << "Compression rate:  " << compressionRate << endl;
    cout << "Encoded file length: " << plaintextLength << endl;
    cout << "Encoded file plaintext entropy: " << getEntropy(charWeights,plaintextLength) << endl;
    cout << "Encoded file coded text entropy: " << getEntropy(codeCharWeights,codeLength) << endl;

    return 0;
}

void finalizeEncoding(BitArr *up, BitArr *down, map<int, int> charWeights, unsigned long long int totalCount,
                      unsigned long long int* scale) {
    char bit = (down->checkBit(0) == 1) ? '1' : '0';
    char bitNeg = (down->checkBit(0) == 1) ? '0' : '1';
    buffer = buffer + bit;

    while(*scale > 0){
        buffer = buffer + bitNeg;
        (*scale)--;
    }

    for(int i = 1; i < down->getSize(); i++ ){
        buffer = buffer + ((down->checkBit(i) == 1) ? '1' : '0');
    }

    while(buffer.size() % 8 != 0) //dopelnienie buffora zeby mial rowno podzielną przez 8 dlugosc i dalo sie go zakodowac do charow
        buffer = buffer + '0';
}

void encode(BitArr *up, BitArr *down, unsigned long long int *scale) {
    char bit,bitNeg;
    while(true){
        if(up->checkBit(0) == down->checkBit(0)){
            bit = (up->checkBit(0) == 1) ? '1' : '0';
            bitNeg = (up->checkBit(0) == 1) ? '0' : '1';
            buffer.append(1, bit);
            while(*scale > 0){
                buffer.append(1, bitNeg);
                (*scale)--;
            }
            down->moveBits(0);
            up->moveBits(1);
        } else if((up->checkBit(0) != down->checkBit(0)) && (up->checkBit(1) == 0) && (down->checkBit(1) == 1)) {
            down->moveBits(0);
            up->moveBits(1);
            down->set(0,0);
            up->set(0,1);
            (*scale)++;
        } else {
            return;
        }
    }
}

void setLimits(BitArr *up, BitArr *down, unsigned long long int totalCount, unsigned long long int cumulativeCount,
               int &currCharWeight) {
    unsigned long long val, downVal = down->getValue(), upVal = up->getValue();
    down->set(downVal + ((upVal - downVal + 1) * cumulativeCount)/totalCount);
    up->set(downVal + ((upVal - downVal + 1) * (cumulativeCount + currCharWeight))/totalCount - 1);
}

double getEntropy(map<int, int>& charWeights, int sum) {
        map<int, int>::iterator it;
        double answer = 0;

        for (it = charWeights.begin(); it != charWeights.end(); it++)
            answer += it->second * log2(it->second);

        return - (answer / sum - log2(sum));
    }

double getCompressionRate(const string& plaintext, const string& ciphertext) {
    return double(filesystem::file_size(plaintext)) / filesystem::file_size(ciphertext);
}