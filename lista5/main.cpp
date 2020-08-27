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
    Pixel val;
    Node *left, *right;
    Node(Pixel val){
        this->val = val;
        this->left = nullptr;
        this->right = nullptr;
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
void create_dictionary(Node* root, vector<Pixel> v, int k);
int pixelDistance(Pixel x, Pixel y);

int main(int argc, char *argv[]) {

    int width, height, i = 1,j,k, pixelCount = 0, size, offset;
    uint8_t currB,currR,currG, currChar;
    unsigned long medR=0, medG=0, medB=0;
    string sett[argc-1]; // wczytanie ustawień
    for(int i = 1, j=0; i<argc; i++, j++){
        sett[j] = argv[i];
    }
    Settings settings = setSettings(sett, argc - 1);
    ifstream inputFile(settings.inputFileName);
    ofstream outputFile(settings.outputFileName);
    if(!(inputFile.is_open()) ||!(outputFile.is_open()) || (settings.colorCount == -1)){
        cout << "You have to give parameters to a program. ./encode -file ${inputfileName} ${outputFileName}" << endl;
        return 0;
    }

    i=0;
    TGAHeader header;
    inputFile.read(reinterpret_cast<char *>(&header), sizeof(TGAHeader));
    pixelCount = header.width * header.height;
    vector<Pixel> image (pixelCount, Pixel());
    while (inputFile >> std::noskipws >> currR && i < pixelCount) {
        inputFile >> std::noskipws >> currG;
        inputFile >> std::noskipws >> currB;
        image[i].setRGB(currR, currG, currB);
        medR += currR;
        medG += currG;
        medB += currB;
        i++;
    }

    Node* curr;
    vector <Pixel> tempDictionary;
    medR /= pixelCount;
    medG /= pixelCount;
    medB /= pixelCount;
    Pixel* centroid = new Pixel(medR,medG,medB);
    Node* dictionary = new Node(*centroid);

    create_dictionary(dictionary, image, settings.colorCount);

    unsigned long valueforMse = 0;
    unsigned long valueForSnr = 0;
    outputFile.write(reinterpret_cast<char *>(&header), sizeof(TGAHeader));
    i=0;
    while (i < pixelCount) {
        curr = dictionary;
        while(curr->left != nullptr && curr->right != nullptr){
            if (pixelDistance((curr->left->val), image[i]) < pixelDistance((curr->right->val), image[i])) {
                curr = curr->left;
            } else {
                curr = curr->right;
            }
        }
        outputFile << curr->val.R;
        outputFile << curr->val.G;
        outputFile << curr->val.B;
        valueforMse += pixelDistance(curr->val, image[i]);
        valueForSnr += curr->val.R * curr->val.R + curr->val.G * curr->val.G + curr->val.B * curr->val.B;
        i++;
    }
    double mse = double(valueforMse) / (pixelCount);
    double snr = (double)(valueForSnr) / ((double)(pixelCount)) / (double)(mse);
    cout << "MSE = " << mse << endl;
    cout << "SNR = " << snr << endl;
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
        } else if (sett[i] == "-colors" && i + 1 < size) {
            settings.colorCount = stoi(sett[i + 1]);
        }
    }
    return settings;
}

void create_dictionary(Node* root, vector<Pixel> v, int k){
    if(k>0){
        std::uniform_int_distribution<int> distr(-2, 2);
        int i;
        Pixel first = Pixel(root->val.R,root->val.G,root->val.B), second = Pixel(), lastSecond = Pixel(), lastFirst = Pixel();
        vector <Pixel> firstGroup, secondGroup;
        int firstCurrentR, firstCurrentG, firstCurrentB, secondCurrentR,secondCurrentG,secondCurrentB;

        do{
            secondCurrentR = max(0,min(255,(int)first.R+distr(eng)));
            secondCurrentG = max(0,min(255,(int)first.G+distr(eng)));
            secondCurrentB = max(0,min(255,(int)first.B+distr(eng)));
            second.setRGB(secondCurrentR, secondCurrentG,secondCurrentB);
        }while((first.R == second.R) && (first.G == second.G) && (first.B == second.B));

        do {
            firstGroup.clear();
            secondGroup.clear();
            firstCurrentR=firstCurrentG=firstCurrentB=secondCurrentR=secondCurrentG=secondCurrentB=0;
            lastSecond.setRGB(second.R, second.G, second.B);
            lastFirst.setRGB(first.R, first.G, first.B);
            for(i = 0; i<v.size(); i++) {
                if (pixelDistance(v[i], first) < pixelDistance(v[i], second)) {
                    firstGroup.push_back(v[i]);
                    firstCurrentR += v[i].R;
                    firstCurrentG += v[i].G;
                    firstCurrentB += v[i].B;
                } else {
                    secondGroup.push_back(v[i]);
                    secondCurrentR += v[i].R;
                    secondCurrentG += v[i].G;
                    secondCurrentB += v[i].B;
                }
            }

            if(!firstGroup.empty()){
                firstCurrentR = firstCurrentR / firstGroup.size();
                firstCurrentG = firstCurrentG / firstGroup.size();
                firstCurrentB = firstCurrentB / firstGroup.size();
                first.setRGB(firstCurrentR, firstCurrentG, firstCurrentB);
            }
            if(!secondGroup.empty()){
                secondCurrentR = secondCurrentR / secondGroup.size();
                secondCurrentG = secondCurrentG / secondGroup.size();
                secondCurrentB = secondCurrentB / secondGroup.size();
                second.setRGB(secondCurrentR, secondCurrentG, secondCurrentB);
            }
            if((first.R == second.R) && (first.G == second.G) && (first.B == second.B)){
                do{
                    secondCurrentR = max(0,min(255,(int)second.R+distr(eng)));
                    secondCurrentG = max(0,min(255,(int)second.G+distr(eng)));
                    secondCurrentB = max(0,min(255,(int)second.B+distr(eng)));
                    second.setRGB(secondCurrentR, secondCurrentG,secondCurrentB);
                }while((first.R == second.R) && (first.G == second.G) && (first.B == second.B));
            }

        }while((first.R != lastFirst.R) || (second.R != lastSecond.R)
               || (first.G != lastFirst.G) || (second.G != lastSecond.G)
               || (first.B != lastFirst.B) || (second.B != lastSecond.B));
        root->right = new Node(first);
        root->left = new Node(second);
        create_dictionary(root->right, firstGroup, k-1);
        create_dictionary(root->left, secondGroup, k-1);
    }
}

int pixelDistance(Pixel x, Pixel y){
    return (abs((int)x.R - (int)y.R) + abs((int)x.G - (int)y.G) + abs((int)x.B - (int)y.B));
}