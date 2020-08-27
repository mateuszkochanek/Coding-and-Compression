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

int twoErrors;

string analyze(string data);
string decToBin4(int val);
Settings setSettings(string sett[], int size);
int main(int argc, char *argv[]) {
    twoErrors=0;
    int i = 1,j,k;
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
        cout << "You have to give parameters to a program. ./decode -input ${inputfileName} -output ${outputFileName}" << endl;
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
    //We are encoding our plain text
    string finalCode = "";
    str="";
    for (int i = 0; i < code.size(); i += 8) {
        str = analyze(code.substr(i, 8));
        finalCode.append(str);
    }
    //_____________________________________________________
    //We are putting encoded text into file
    for (int i = 0; i < finalCode.size(); i += 8) {
        uint8_t value = std::bitset<8>(finalCode.substr(i, 8)).to_ulong();
        outputFile << value;
    }
    cout<<"Podwojnych bledow bylo:" << twoErrors << endl;
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

string analyze(string data){
    int p0, p1, p3, p7;
    unsigned long index;
    bool sameP0, sameP1, sameP3, sameP7;
    string result = "0000", corractable = "000";
    int dataInt[8];
    for(int i = 0; i<8 ; i++)
        dataInt[i] = (int)(data[i])-48;
    p0 = (dataInt[2] + dataInt[4] + dataInt[6])%2;
    p1 = (dataInt[2] + dataInt[5] + dataInt[6])%2;
    p3 = (dataInt[4] + dataInt[5] + dataInt[6])%2;
    p7 = (dataInt[0] + dataInt[1] + dataInt[2] + dataInt[3] + dataInt[4] + dataInt[5] + dataInt[6])%2;
    sameP0 = (p0 == dataInt[0]), sameP1 = (p1 == dataInt[1]), sameP3 = (p3 == dataInt[3]), sameP7 = (p7 == dataInt[7]);

    if(sameP7){ //parity bit is correct, either no error or two errors
        if(!sameP0 || !sameP1 || !sameP3){ // something is incorrect, we have two errors
            twoErrors++;
        }//else code is correct or two errors cancelled ech other out
    } else{// parity bit incorrect
        if(!sameP0 || !sameP1 || !sameP3){ //one correctable error.
            corractable[2] = (sameP0?'0':'1');
            corractable[1] = (sameP1?'0':'1');
            corractable[0] = (sameP3?'0':'1');
            index = bitset<3>(corractable).to_ulong()-1;
            dataInt[index] = (dataInt[index]+1)%2;
        } else{ // parity bit incorrect, error with parity bit
            dataInt[7] = (dataInt[7]+1)%2;
        }
    }

    result[0] = (char)(dataInt[2]+48);
    result[1] = (char)(dataInt[4]+48);
    result[2] = (char)(dataInt[5]+48);
    result[3] = (char)(dataInt[6]+48);
    return result;
}