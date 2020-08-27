//
// Created by erthax on 06.04.2020.
//
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <bitset>
#ifndef LZW_ENCODING_UNIVERSALCODER_H
#define LZW_ENCODING_UNIVERSALCODER_H


class UniversalCoder {
public:
    static std::string decToBin(int n);
    static int binToDec(std::string number);
    virtual std::string encode(int number) = 0;
    virtual std::vector<int> decode(std::string code) = 0;
};

class UniversalCoderGamma: public UniversalCoder {
public:
    std::string encode(int number);
    std::vector<int> decode(std::string code);
};

class UniversalCoderDelta: public UniversalCoder {
public:
    std::string encode(int number);
    std::vector<int> decode(std::string code);
};

class UniversalCoderOmega: public UniversalCoder {
public:
    std::string encode(int number);
    std::vector<int> decode(std::string code);
};

class UniversalCoderFib: public UniversalCoder {
public:
    std::string encode(int number);
    std::vector<int> decode(std::string code);
};


#endif //LZW_ENCODING_UNIVERSALCODER_H
