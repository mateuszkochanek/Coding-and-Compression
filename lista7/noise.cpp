#include <experimental/filesystem>
#include <iostream>
#include <fstream>
#include <map>
#include <bitset>
#include <random>
using namespace std;

struct Settings
{
    std::string inputFileName;
    std::string outputFileName;
};

Settings setSettings(string sett[], int size);
int main(int argc, char *argv[]) {
    int i;
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_real_distribution<> dist(0, 1);
    uint8_t currChar;

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
    //We are reading input file
    string code="", str="";
    while (inputFile >> std::noskipws >> currChar) {
        str = std::bitset<8>(currChar).to_string();
        code.append(str);
    }
    //_____________________________________________________
    //We are fliping random bits
    for(i = 0; i < code.size(); i++){
        if(dist(engine) < 0.05){
            code[i] = (char)((code[i]=='1')?'0':'1');
        }
    }
    /*code[2] = '1';
    code[7] = '0';

    code[18] = '1';
    code[26] = '1';

    code[32] = '0';*/
    //_____________________________________________________
    //We are putting encoded text into file
    for (int i = 0; i < code.size(); i += 8) {
        uint8_t value = std::bitset<8>(code.substr(i, 8)).to_ulong();
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