#include <experimental/filesystem>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <bitset>
#include <cmath>
#include <random>
using namespace std;

struct Settings
{
    std::string inputFileName;
    std::string outputFileName;
};

string decToBin4(int val);
string getCode(string data);
Settings setSettings(string sett[], int size);
int main(int argc, char *argv[]) {

    int width, height, i = 1,j,k, pixelCount = 0, size, offset, filterSize;
    uint8_t currChar;
    unsigned long medR=0, medG=0, medB=0;

    // We are reading our settings
    string sett[argc-1];
    for(int i = 1, j=0; i<argc; i++, j++){
        sett[j] = argv[i];
    }
    Settings settings = setSettings(sett, argc - 1);
    ifstream inputFile(settings.inputFileName);
    ofstream outputFile(settings.outputFileName);
    if(!(inputFile.is_open()) ||!(outputFile.is_open())){
        cout << "You have to give parameters to a program. ./encode -input ${inputfileName} -output ${outputFileName}" << endl;
        return 0;
    }
    //_____________________________________________________
    //We create coding map
    string dataWord;
    map<string, string> codeMap;
    for(i=0; i<16 ;i++){
        dataWord = decToBin4(i);
        codeMap[dataWord] = getCode(dataWord);
    }
    //_____________________________________________________
    //We are reading input file
    string code="", str="";
    while (inputFile >> std::noskipws >> currChar) {
        str = std::bitset<8>(currChar).to_string();
        code.append(str);
    }
    //_____________________________________________________
    //We are encoding our plain text
    string finalCode;
    str="";
    for (int i = 0; i < code.size(); i += 4) {
        str = code.substr(i, 4);
        finalCode.append(codeMap[str]);
    }
    //_____________________________________________________
    //We are putting encoded text into file
    for (int i = 0; i < finalCode.size(); i += 8) {
        uint8_t value = std::bitset<8>(finalCode.substr(i, 8)).to_ulong();
        outputFile << value;
    }
    outputFile.close();
    inputFile.close();
}

Settings setSettings(string sett[], int size) {
    Settings settings;
    for (int i = 0; i < size; i++) {
        if (sett[i] == "-input" && i + 1 < size) {
            settings.inputFileName = sett[i + 1];
        } else if (sett[i] == "-output" && i + 1 < size) {
            settings.outputFileName = sett[i + 1];
        }
    }
    return settings;
}

string decToBin4(int val){
    string data(4, '0');
    for (int i = 3; val > 0; i--) {
        data[i] = char((val % 2) + 48);
        val /= 2;
    }
    return data;
}

string getCode(string data){
    string result(8, '0');
    result[2] = data[0];
    result[4] = data[1];
    result[5] = data[2];
    result[6] = data[3];
    result[0] = (((int)(result[2]) + (int)(result[4]) + (int)result[6])%2==0) ? '0' : '1';
    result[1] = (((int)(result[2]) + (int)(result[5]) + (int)result[6])%2==0) ? '0' : '1';
    result[3] = (((int)(result[4]) + (int)(result[5]) + (int)result[6])%2==0) ? '0' : '1';
    result[7] = (((int)(result[0]) + (int)(result[1]) +(int)(result[2]) +(int)(result[3]) +(int)(result[4]) +(int)(result[5]) +(int)(result[6]) + (int)result[7])%2==0) ? '0' : '1';
    return result;
}