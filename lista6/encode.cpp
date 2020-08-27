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
    int colorCount;
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
void create_dictionary(Node* root, vector <int> v, int k, string code);
int pixelDistance(Pixel x, Pixel y);
int getDictionaryValue(Node* root, string code);
string getDictionaryCode(Node* root, int val);

int main(int argc, char *argv[]) {

    int width, height, i = 1,j,k, pixelCount = 0, size, offset, filterSize;
    uint8_t currB,currR,currG, currChar;
    unsigned long medR=0, medG=0, medB=0;

    // We are reading our settings
    string sett[argc-1];
    for(int i = 1, j=0; i<argc; i++, j++){
        sett[j] = argv[i];
    }
    Settings settings = setSettings(sett, argc - 1);
    ifstream inputFile(settings.inputFileName);
    ofstream outputFile(settings.outputFileName);
    if(!(inputFile.is_open()) ||!(outputFile.is_open()) || (settings.colorCount == -1)){
        cout << "You have to give parameters to a program. ./encode -input ${inputfileName} -output ${outputFileName} -bits ${colorCount}" << endl;
        return 0;
    }
    //________________________________________________________
    // We are reading our image from a file
    i=0;
    TGAHeader header;
    inputFile.read(reinterpret_cast<char *>(&header), sizeof(TGAHeader));
    pixelCount = header.width * header.height;
    vector<Pixel> image (pixelCount, Pixel());
    while (inputFile >> std::noskipws >> currR && i < pixelCount) {
        inputFile >> std::noskipws >> currG;
        inputFile >> std::noskipws >> currB;
        image[i].setRGB(currR, currG, currB);
        i++;
    }
    //________________________________________________________
    // We are creating Down and Up filters that we will use in encoding
    filterSize = pixelCount / 2;
    vector <int> redDownFilter(filterSize);
    vector <int> redUpFilter(filterSize);
    vector <int> greenDownFilter(filterSize);
    vector <int> greenUpFilter(filterSize);
    vector <int> blueDownFilter(filterSize);
    vector <int> blueUpFilter(filterSize);
    int redUpMed=0, redDownMed=0, greenUpMed=0, greenDownMed=0, blueUpMed=0, blueDownMed=0;

    for (int i = 1, j = 0; i < pixelCount; i += 2, j++) {
        redDownFilter[j] = (image[i].R + image[i - 1].R) / 2;
        redDownMed+=redDownFilter[j];
        redUpFilter[j] = (image[i].R - image[i - 1].R) / 2;
        redUpMed+=redUpFilter[j];

        greenDownFilter[j] = (image[i].G + image[i - 1].G) / 2;
        greenDownMed+=greenDownFilter[j];
        greenUpFilter[j] = (image[i].G - image[i - 1].G) / 2;
        greenUpMed+=greenUpFilter[j];

        blueDownFilter[j] = (image[i].B + image[i - 1].B) / 2;
        blueDownMed+=blueDownFilter[j];
        blueUpFilter[j] = (image[i].B - image[i - 1].B) / 2;
        blueUpMed+=blueUpFilter[j];
    }
    //________________________________________________________
    // We are creating Dictionaries for every filter we have
    redUpMed /= filterSize, redDownMed /= filterSize, greenUpMed /= filterSize, greenDownMed /= filterSize, blueUpMed /= filterSize, blueDownMed /= filterSize;
    Node* redDownDictionary = new Node(redDownMed);
    Node* redUpDictionary = new Node(redUpMed);
    Node* greenDownDictionary = new Node(greenDownMed);
    Node* greenUpDictionary = new Node(greenUpMed);
    Node* blueDownDictionary = new Node(blueDownMed);
    Node* blueUpDictionary = new Node(blueUpMed);

    create_dictionary(redDownDictionary, redDownFilter, settings.colorCount, "");
    create_dictionary(redUpDictionary, redUpFilter, settings.colorCount, "");
    create_dictionary(greenDownDictionary, greenDownFilter, settings.colorCount, "");
    create_dictionary(greenUpDictionary, greenUpFilter, settings.colorCount, "");
    create_dictionary(blueDownDictionary, blueDownFilter, settings.colorCount, "");
    create_dictionary(blueUpDictionary, blueUpFilter, settings.colorCount, "");
    //________________________________________________________
    // writing TGA header and dictionaries into output file
    int val, dictionarySize = pow(2,settings.colorCount);
    outputFile.write(reinterpret_cast<char *>(&header), sizeof(TGAHeader));
    val = settings.colorCount;
    outputFile.write((char*)&val,sizeof(val));
    i=0;
    string code;

    while (i < dictionarySize) {
        code = std::bitset<8>(i).to_string();
        code = code.substr(8-settings.colorCount);

        val = getDictionaryValue(redDownDictionary, code);
        outputFile.write((char*)&val,sizeof(val));
        val = getDictionaryValue(redUpDictionary, code);
        outputFile.write((char*)&val,sizeof(val));

        val = getDictionaryValue(greenDownDictionary, code);
        outputFile.write((char*)&val,sizeof(val));
        val = getDictionaryValue(greenUpDictionary, code);
        outputFile.write((char*)&val,sizeof(val));

        val = getDictionaryValue(blueDownDictionary, code);
        outputFile.write((char*)&val,sizeof(val));
        val = getDictionaryValue(blueUpDictionary, code);
        outputFile.write((char*)&val,sizeof(val));
        i++;
    }
    //________________________________________________________
    // quantization and writing fiters into output file
    string finalCode;
    for (i = 0; i < filterSize; i++) {
        finalCode.append(getDictionaryCode(redDownDictionary, redDownFilter[i]));
        finalCode.append(getDictionaryCode(redUpDictionary, redUpFilter[i]));

        finalCode.append(getDictionaryCode(greenDownDictionary, greenDownFilter[i]));
        finalCode.append(getDictionaryCode(greenUpDictionary, greenUpFilter[i]));

        finalCode.append(getDictionaryCode(blueDownDictionary, blueDownFilter[i]));
        finalCode.append(getDictionaryCode(blueUpDictionary, blueUpFilter[i]));
    }
    while (finalCode.size() % 8 != 0) {
        finalCode.append("0");
    }
    for (int i = 0; i < finalCode.size(); i += 8) {
        uint8_t value = std::bitset<8>(finalCode.substr(i, 8)).to_ulong();
        outputFile << value;
    }
    outputFile.close();
    inputFile.close();
}

Settings setSettings(string sett[], int size) {
    Settings settings;
    settings.inputFileName;
    settings.outputFileName;
    settings.colorCount = -1;
    for (int i = 0; i < size; i++) {
        if (sett[i] == "-input" && i + 1 < size) {
            settings.inputFileName = sett[i + 1];
        } else if (sett[i] == "-output" && i + 1 < size) {
            settings.outputFileName = sett[i + 1];
        } else if (sett[i] == "-bits" && i + 1 < size) {
            settings.colorCount = stoi(sett[i + 1]);
        }
    }
    return settings;
}

void create_dictionary(Node* root, vector <int> v, int k,string code){
    if(k>0){
        std::uniform_int_distribution<int> distr(-2, 2);
        int i,size;
        int first = root->val, second, lastSecond, lastFirst;
        vector <int> firstGroup, secondGroup;
        int firstCurrent,secondCurrent;

        do{
            second = first+distr(eng);
        }while(first == second);

        do {
            firstGroup.clear();
            secondGroup.clear();
            firstCurrent=secondCurrent=0;
            lastSecond=second;
            lastFirst=first;
            for(i = 0; i<v.size(); i++) {
                if (abs(v[i] - first) < abs(v[i] - second)) {
                    firstCurrent+=v[i];
                    firstGroup.push_back(v[i]);
                } else {
                    secondCurrent+=v[i];
                    secondGroup.push_back(v[i]);
                }
            }

            if(!firstGroup.empty()){
                size = firstGroup.size();
                firstCurrent = firstCurrent / size;
                first = firstCurrent;
            }
            if(!secondGroup.empty()){
                size = secondGroup.size();
                secondCurrent = secondCurrent / size;
                second=secondCurrent;
            }
            if(first == second){
                do{
                    secondCurrent = max(0,min(255,(int)second+distr(eng)));
                    second = secondCurrent;
                }while(first == second);
            }

        }while((first != lastFirst) || (second != lastSecond));

        root->right = new Node(first);
        root->left = new Node(second);
        root->right->code = code+"1";
        root->left->code = code+"0";
        create_dictionary(root->right, firstGroup, k-1, code+"1");
        create_dictionary(root->left, secondGroup, k-1, code+"0");
    }
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

string getDictionaryCode(Node* root, int val){
    Node* curr = root;
    while (curr->left != nullptr) {
        if (abs(val - curr->left->val) < abs(val - curr->right->val)) {
            curr = curr->left;
        } else {
            curr = curr->right;
        }
    }
    return curr->code;
}