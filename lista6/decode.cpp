#include <experimental/filesystem>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <bitset>
#include <cmath>
#include <random>
#include <string>
using namespace std;

struct Settings
{
    std::string inputFileName;
    std::string outputFileName;
};

class Pixel
{
public:
    uint8_t R;
    uint8_t G;
    uint8_t B;

    Pixel(){
        this->R = 0;
        this->B = 0;
        this->G = 0;
    }

    Pixel(uint8_t R, uint8_t G, uint8_t B){
        this->R = R;
        this->G = G;
        this->B = B;
    }

    void setRGB(uint8_t R, uint8_t G, uint8_t B){
        this->R = R;
        this->G = G;
        this->B = B;
    }
};

struct Node{
    int val;
    string code;
    Node *left, *right;
    Node(int val){
        this->val = val;
        this->left = nullptr;
        this->right = nullptr;
        this->code = "";
    }

    void setLeft(Node* left){this->left = left;}
    void setRight(Node* right){this->right = right;}
    Node* getLeft(Node* left){return this->left;}
    Node* getRight(Node* right){return this->right;}
};

#pragma pack(1)
struct TGAHeader {
    char  idlength;
    char  colourmaptype;
    char  datatypecode;
    short int colourmaporigin;
    short int colourmaplength;
    char  colourmapdepth;
    short int x_origin;
    short int y_origin;
    short width;
    short height;
    char  bitsperpixel;
    char  imagedescriptor;
};

std::random_device rd;
std::mt19937 eng(rd());
Settings setSettings(string sett[], int size);
int getDictionaryValue(Node* root, string code);
string getDictionaryCode(int val, int bitsCount);

int main(int argc, char *argv[]) {

    int width, height, i = 1,j,k, pixelCount = 0, size, offset, filterSize;
    uint8_t currB,currR,currG, currChar;
    unsigned long medR=0, medG=0, medB=0;

    // We are reading our settings
    string sett[argc-1];
    for(i = 1, j=0; i<argc; i++, j++){
        sett[j] = argv[i];
    }
    Settings settings = setSettings(sett, argc - 1);
    ifstream inputFile(settings.inputFileName);
    ofstream outputFile(settings.outputFileName);
    if(!(inputFile.is_open()) ||!(outputFile.is_open())){
        cout << "You have to give parameters to a program. ./decode -input ${inputfileName} -output ${outputFileName}" << endl;
        return 0;
    }
    //________________________________________________________
    // We are reading our encoded image from a file, we have TGAheader|dictionarySize|6 dictionaries|encoded image
    TGAHeader header;
    string code;
    int bitsCount,dictionarySize,val;
    inputFile.read(reinterpret_cast<char *>(&header), sizeof(TGAHeader));
    inputFile.read((char*)&bitsCount,sizeof(bitsCount));
    dictionarySize = pow(2,bitsCount);
    pixelCount = header.width * header.height;
    map <string,int> redDownDictionary;
    map <string,int> redUpDictionary;
    map <string,int> greenDownDictionary;
    map <string,int> greenUpDictionary;
    map <string,int> blueDownDictionary;
    map <string,int> blueUpDictionary;
    for(i = 0; i<dictionarySize;i++){
        code = getDictionaryCode(i,bitsCount);

        inputFile.read((char*)&val,sizeof(val));
        redDownDictionary[code] = val;
        inputFile.read((char*)&val,sizeof(val));
        redUpDictionary[code] = val;

        inputFile.read((char*)&val,sizeof(val));
        greenDownDictionary[code] = val;
        inputFile.read((char*)&val,sizeof(val));
        greenUpDictionary[code] = val;

        inputFile.read((char*)&val,sizeof(val));
        blueDownDictionary[code] = val;
        inputFile.read((char*)&val,sizeof(val));
        blueUpDictionary[code] = val;
    }
    code = "";
    //________________________________________________________
    // we have to read encoded image (every color divided into two parts so one pixel has 6 values)
    int redDownValue,redUpValue,greenDownValue,greenUpValue,blueDownValue,blueUpValue;
    uint8_t curr;
    vector<Pixel> image (pixelCount, Pixel());
    while (inputFile >> std::noskipws >> curr) {
        code.append(std::bitset<8>(curr).to_string());
    }
    for(i = 0, j=0; i<pixelCount; i+=2){
        redDownValue = redDownDictionary[code.substr(j, bitsCount)];
        j += bitsCount;
        redUpValue = redUpDictionary[code.substr(j, bitsCount)];
        j += bitsCount;
        greenDownValue = greenDownDictionary[code.substr(j, bitsCount)];
        j += bitsCount;
        greenUpValue = greenUpDictionary[code.substr(j, bitsCount)];
        j += bitsCount;
        blueDownValue = blueDownDictionary[code.substr(j, bitsCount)];
        j += bitsCount;
        blueUpValue = blueUpDictionary[code.substr(j, bitsCount)];
        j += bitsCount;

        image[i].setRGB((max(0, min(255, redDownValue - redUpValue))), (max(0, min(255, greenDownValue - greenUpValue))), (max(0, min(255, blueDownValue - blueUpValue))));
        image[i+1].setRGB((max(0, min(255, redDownValue + redUpValue))), (max(0, min(255, greenDownValue + greenUpValue))), (max(0, min(255, blueDownValue + blueUpValue))));
    }

    //________________________________________________________
    // Write our header and image pixels into a file
    outputFile.write(reinterpret_cast<char *>(&header), sizeof(TGAHeader));
    for(i = 0; i<pixelCount; i++){
        outputFile << image[i].R;
        outputFile << image[i].G;
        outputFile << image[i].B;
    }

    outputFile.close();
    inputFile.close();
}

Settings setSettings(string sett[], int size) {
    Settings settings;
    settings.inputFileName;
    settings.outputFileName;
    for (int i = 0; i < size; i++) {
        if (sett[i] == "-input" && i + 1 < size) {
            settings.inputFileName = sett[i + 1];
        } else if (sett[i] == "-output" && i + 1 < size) {
            settings.outputFileName = sett[i + 1];
        }
    }
    return settings;
}

int pixelDistance(Pixel x, Pixel y){
    return (abs((int)x.R - (int)y.R) + abs((int)x.G - (int)y.G) + abs((int)x.B - (int)y.B));
}

int getDictionaryValue(Node* root, string code){
    Node* curr = root;

    for (int i = 0; curr->left != nullptr; i++){
        if(curr->left->code[i] == code[i]){
            curr = curr->left;
        } else {
            curr = curr->right;
        }
    }

    return curr->val;
}

string getDictionaryCode(int val, int bitsCount){
    string result(bitsCount, '0');
    for (int i = bitsCount - 1; val > 0; i--) {
        result[i] = char((val % 2) + 48);
        val /= 2;
    }
    return result;
}