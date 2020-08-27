#include <filesystem>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
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
double getEntropy(map<int, int>& charWeights, int sum);
double getCompressionRate(const string& plaintext, const string& ciphertext);
int main(int argc, char *argv[]) {
    int plaintextLength = 0, codeLength = 0;
    map <int,int> plaintextCharWeights;
    map <int,int> codeCharWeights;

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
        cout<<"-o > OmegaCoding"<< endl<<"-d > DeltaCoding" << endl<<"-g > GammaCoding" << endl<<"-f > FibCoding" << endl;
        return 0;
    }
    uint8_t currChar;
    map < vector<uint8_t> ,int> dictionary;
    //vector <vector <char> > dictionary();
    string buffer;
    vector<uint8_t> currS;
    vector<uint8_t> prevS;

    int unusedCode = 257;
    for(int i = 0; i < 256; i++){
        currS.clear();
        currS.push_back(i);
        dictionary[currS] = i+1;
    }
    currS.clear();

    if(!(inputFile.is_open() && outputFile.is_open())){
        cout << "Give inputFile name as first parameter and output file as second." << endl;
        return 0;
    }

    while (inputFile >> std::noskipws >> currChar) {
        plaintextLength++;
        plaintextCharWeights[currChar]++;
        prevS = currS;
        currS.push_back(currChar);
        if (!(dictionary.find(currS) != dictionary.end())) {
            dictionary[currS] = unusedCode;
            unusedCode++;
            buffer.append(settings.coder->encode(dictionary[prevS]));
            currS.clear();
            currS.push_back(currChar);
        }
    }

    if(currS.size()>=1){
        if (dictionary.find(currS) != dictionary.end()){
            buffer.append(settings.coder->encode(dictionary[currS]));
        }
    }

    uint8_t value;
    int i=0;
    while(buffer.size()%8 != 0){
        buffer.append("0");
        i++;
    }
    value = i;
    outputFile << value;

    int size = buffer.size();
    for (int i = 0; i < size; i += 8) {
        value = stoi(buffer.substr(i, 8), nullptr, 2);
        outputFile << value;
        codeCharWeights[value]++;
        codeLength++;
    }
    cout << "Encoded file plaintext length: " << plaintextLength << endl;
    cout << "Encoded file coded text length: " << codeLength << endl;
    cout << "Compression rate:  " << getCompressionRate(settings.inputFileName, settings.outputFileName) << endl;
    cout << "Encoded file length: " << plaintextLength << endl;
    cout << "Encoded file plaintext entropy: " << getEntropy(plaintextCharWeights,plaintextLength) << endl;
    cout << "Encoded file coded text entropy: " << getEntropy(codeCharWeights,codeLength) << endl;

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

double getEntropy(map<int, int>& charWeights, int sum) {
    map<int, int>::iterator it;
    double answer = 0;

    for (it = charWeights.begin(); it != charWeights.end(); it++)
        answer += it->second * log2(it->second);

    return - (answer / sum - log2(sum));
}

double getCompressionRate(const string& plaintext, const string& ciphertext) {
    return double(filesystem::file_size(plaintext)) / filesystem::file_size(ciphertext);
return 0;
}