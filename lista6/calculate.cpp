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
    std::string originalImageName;
    std::string newImageName;
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
int pixelDistance(Pixel x, Pixel y);

int main(int argc, char *argv[]) {

    int width, height, i = 1,j,k, pixelCount = 0, size, offset, filterSize;
    uint8_t currB,currR,currG, currChar;
    long mseR=0, mseG=0, mseB=0;
    long snr=0, mse=0;
    // We are reading our settings
    string sett[argc-1];
    for(int i = 1, j=0; i<argc; i++, j++){
        sett[j] = argv[i];
    }
    Settings settings = setSettings(sett, argc - 1);
    ifstream orgFile(settings.originalImageName);
    ifstream newFile(settings.newImageName);
    if(!(orgFile.is_open()) ||!(newFile.is_open())){
        cout << "You have to give parameters to a program. ./calculate -org ${orgFileName} -new ${newFileName}" << endl;
        return 0;
    }
    //________________________________________________________
    // We are reading our original image
    i=0;
    TGAHeader headerOrg;
    orgFile.read(reinterpret_cast<char *>(&headerOrg), sizeof(TGAHeader));
    pixelCount = headerOrg.width * headerOrg.height;
    vector<Pixel> orgImage (pixelCount, Pixel());
    while (orgFile >> std::noskipws >> currR && i < pixelCount) {
        orgFile >> std::noskipws >> currG;
        orgFile >> std::noskipws >> currB;
        orgImage[i].setRGB(currR, currG, currB);
        i++;
    }
    //________________________________________________________
    // We are reading our new image
    i=0;
    TGAHeader headerNew;
    newFile.read(reinterpret_cast<char *>(&headerNew), sizeof(TGAHeader));
    pixelCount = headerNew.width * headerNew.height;
    vector<Pixel> newImage (pixelCount, Pixel());
    while (newFile >> std::noskipws >> currR && i < pixelCount) {
        newFile >> std::noskipws >> currG;
        newFile >> std::noskipws >> currB;
        newImage[i].setRGB(currR, currG, currB);
        i++;
    }
    //________________________________________________________
    //calcule mse arg
    for(i=0 ; i<pixelCount; i++){
        mseR+=(orgImage[i].R-newImage[i].R)*(orgImage[i].R-newImage[i].R);
        mseG+=(orgImage[i].G-newImage[i].G)*(orgImage[i].G-newImage[i].G);
        mseB+=(orgImage[i].B-newImage[i].B)*(orgImage[i].B-newImage[i].B);
        //cout << orgImage[i].R-newImage[i].R <<" | " << mseR << "         ";
        //cout << orgImage[i].G-newImage[i].G <<" | " << mseG << endl;
        snr+=orgImage[i].R*newImage[i].R*orgImage[i].G*newImage[i].G*orgImage[i].B*newImage[i].B;
    }

    double mseRFin = (double)(mseR) / (double)(pixelCount);
    double mseGFin = (double)(mseG) / (double)(pixelCount);
    double mseBFin = (double)(mseB) / (double)(pixelCount);
    double mseFin = (double)(mseRFin + mseGFin + mseBFin) / 3;
    double snrFin = ((double)(snr) / (double)(pixelCount * 3)) / mseFin;
    cout << "MSE Red: " << mseRFin << endl;
    cout << "MSE Green: " << mseGFin << endl;
    cout << "MSE Blue: " << mseBFin << endl;
    cout << "MSE: " << mseFin << endl;
    cout << "SNR: " << snrFin << endl;

    orgFile.close();
    newFile.close();
}

Settings setSettings(string sett[], int size) {
    Settings settings;
    settings.originalImageName;
    settings.newImageName;
    for (int i = 0; i < size; i++) {
        if (sett[i] == "-org" && i + 1 < size) {
            settings.originalImageName = sett[i + 1];
        } else if (sett[i] == "-new" && i + 1 < size) {
            settings.newImageName = sett[i + 1];
        }
    }
    return settings;
}

int pixelDistance(Pixel x, Pixel y){
    return (abs((int)x.R - (int)y.R) + abs((int)x.G - (int)y.G) + abs((int)x.B - (int)y.B));
}
