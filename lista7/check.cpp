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
    std::string inputSecFileName;
};

Settings setSettings(string sett[], int size);
int main(int argc, char *argv[]) {
    int i, differentBlocks = 0;
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
    ifstream inputSecFile(settings.inputSecFileName);
    if(!(inputFile.is_open()) ||!(inputSecFile.is_open())){
        cout << "You have to give parameters to a program. ./check -input ${inputfileName} -inputSec ${outputFileName}" << endl;
        return 0;
    }
    //_____________________________________________________
    string codeFir="", str="", codeSec = "";
    while (inputFile >> std::noskipws >> currChar) {
        str = std::bitset<8>(currChar).to_string();
        codeFir.append(str);
    }
    while (inputSecFile >> std::noskipws >> currChar) {
        str = std::bitset<8>(currChar).to_string();
        codeSec.append(str);
    }
    //_____________________________________________________
    for (int i = 0; i < codeSec.size(); i += 4) {
        if (codeFir.substr(i, 4) != codeSec.substr(i, 4))
            differentBlocks++;
    }
    //_____________________________________________________
    //We are putting encoded text into file
    cout << "W pliku " << settings.inputFileName<< " i "<<settings.inputSecFileName<<" mamy " << differentBlocks << " różnych bloków długości 4"<<endl;
    inputSecFile.close();
    inputFile.close();
}

Settings setSettings(string sett[], int size) {
    Settings settings;
    for (int i = 0; i < size; i++) {
        if (sett[i] == "-input" && i + 1 < size) {
            settings.inputFileName = sett[i + 1];
        } else if (sett[i] == "-inputSec" && i + 1 < size) {
            settings.inputSecFileName = sett[i + 1];
        }
    }
    return settings;
}