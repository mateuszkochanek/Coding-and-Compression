#include <iostream>
#include <fstream>
#include <map>
#include <bitset>
#include "UniversalCoder/UniversalCoder.h"
using namespace std;

struct Settings
{
    UniversalCoder* coder; // coder
    std::string inputFileName;  //>= false, <= true, if not set set to <=
    std::string outputFileName;  // 1 - int, 2 - float, 3 - double, 4 - string, -1 - wrong type
};
Settings setSettings(string sett[], int size);
int main(int argc, char *argv[]) {
    string sett[argc-1];
    for(int i = 1, j=0; i<argc; i++, j++){
        sett[j] = argv[i];
    }
    Settings settings = setSettings(sett, argc - 1);

    ifstream inputFile(settings.inputFileName);
    ofstream outputFile(settings.outputFileName);
    if(!(inputFile.is_open() && outputFile.is_open())){
        cout << "You have to give parameters to a program. ./encode -file ${inputfileName} ${outputFileName} ${-o|-g|-d|-f}" << endl;
        cout << "${-o|-g|-d|-f} are not necessary parametars, default will be -o." << endl;
        cout<<"-o > OmegaCoding" << endl<<"-d > DeltaCoding" << endl<<"-g > GammaCoding" << endl<<"-f > FibCoding" << endl;
        return 0;
    }
    int vector_size = 0,zero_number = 0;
    vector<int> lzw_encoded;
    uint8_t currChar;
    map <int,vector<uint8_t>> dictionary;
    string buffer;
    vector<uint8_t> currS;
    vector<uint8_t> prevS;

    int i,j, unusedCode = 257;
    for(i = 0; i < 256; i++){
        currS.clear();
        currS.push_back(i);
        dictionary[i+1] = currS;
    }
    currS.clear();


    while (inputFile >> std::noskipws >> currChar)
        buffer.append(std::bitset<8>(uint8_t(currChar)).to_string());
    zero_number = stoi(buffer.substr(0, 8), nullptr, 2);
    buffer = buffer.substr(8,buffer.size()-zero_number-8);
    lzw_encoded = settings.coder->decode(buffer);

    for(int enc : lzw_encoded){
        if ( enc > dictionary.size() + 1 ){
            break;
        } else if ( enc == dictionary.size() + 1 ){
            prevS.push_back(prevS[0]);
            dictionary[unusedCode] = prevS;
            prevS.pop_back();
            unusedCode++;
        }else if ( !prevS.empty()  ){
            prevS.push_back(dictionary[enc][0]);
            dictionary[unusedCode] = prevS;
            prevS.pop_back();
            unusedCode++;
        }

        vector_size = dictionary[enc].size();
        for(i = 0 ; i < vector_size; i++)
            outputFile << dictionary[enc][i];
        prevS = dictionary[enc];

    }

    inputFile.close();
    outputFile.close();
}

Settings setSettings(string sett[], int size) {
    Settings settings;
    settings.coder = new UniversalCoderOmega();
    for (int i = 0; i < size; i++) {
        if (sett[i] == "-file" && i + 2 < size) {
            settings.inputFileName = sett[i+1];
            settings.outputFileName = sett[i+2];
        } else if (sett[i] == "-o") {
            settings.coder = new UniversalCoderOmega();
        } else if (sett[i] == "-g") {
            settings.coder = new UniversalCoderGamma();
        } else if (sett[i] == "-d") {
            settings.coder = new UniversalCoderDelta();
        }else if (sett[i] == "-f") {
            settings.coder = new UniversalCoderFib();
        }
    }
    return settings;
}