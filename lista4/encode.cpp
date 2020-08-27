#include <experimental/filesystem>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <bitset>
#include <cmath>

using namespace std;

double minR, minB, minG, minAll;
string sminR, sminB, sminG, sminAll;

struct Settings
{
    std::string inputFileName;
};

struct Pixel
{
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
};
struct our_cmp
{
    bool operator() ( Pixel a, Pixel b ) const
    {
        return (a.R > b.R && a.B > b.B && a.G > b.G) ;
    }
};
Settings setSettings(string sett[], int size);
uint8_t pred1(uint8_t n, uint8_t w, uint8_t nw);
uint8_t pred2(uint8_t n, uint8_t w, uint8_t nw);
uint8_t pred3(uint8_t n, uint8_t w, uint8_t nw);
uint8_t pred4(uint8_t n, uint8_t w, uint8_t nw);
uint8_t pred5(uint8_t n, uint8_t w, uint8_t nw);
uint8_t pred6(uint8_t n, uint8_t w, uint8_t nw);
uint8_t pred7(uint8_t n, uint8_t w, uint8_t nw);
uint8_t pred8(uint8_t n, uint8_t w, uint8_t nw);
void getEntropy(vector<vector<Pixel>> results, int size, int num);
int main(int argc, char *argv[]) {
    minR=1000, minB=1000, minG=1000, minAll=1000;
    sminR="", sminB="", sminG="", sminAll="";
    int width, height, i = 1,j,k, pixelCount = 0;
    uint8_t currB,currR,currG, currChar;
    string sett[argc-1];
    for(int i = 1, j=0; i<argc; i++, j++){
        sett[j] = argv[i];
    }
    Settings settings = setSettings(sett, argc - 1);

    ifstream inputFile(settings.inputFileName);
    if(!(inputFile.is_open())){
        cout << "You have to give parameters to a program. ./encode -file ${inputfileName} ${outputFileName}" << endl;
        return 0;
    }

    if(!(inputFile.is_open())){
        cout << "Give inputFile name as first parameter and output file as second." << endl;
        return 0;
    }
    std::vector<uint8_t> header(18);
    for (int i = 0; i < 18; i++) {
        inputFile >> std::noskipws >> currChar;
        header[i] = currChar;
    }

    int lengthToSkip = header[0] + header[7] * (header[6] * 256 + header[5]) / 8;
    for (int i = 0; i < lengthToSkip; i++)
        inputFile >> std::noskipws >> currChar;
    width = header[12] + header[13] * 256;
    height = header[14] + header[15] * 256;
    uint8_t (*pred[8])(uint8_t,uint8_t,uint8_t){ pred1,pred2,pred3,pred4,pred5,pred6,pred7,pred8};
    j = 1,i=0;
    pixelCount = width * height;
    vector<vector<Pixel> > results( 9 , vector<Pixel> (pixelCount, Pixel()));
    Pixel prevRow[width+1];
    Pixel row[width+1];
    while (inputFile >> std::noskipws >> currR && i < pixelCount) {
        inputFile >> std::noskipws >> currG;
        inputFile >> std::noskipws >> currB;
        row[j].R = currR;
        row[j].G = currG;
        row[j].B = currB;

        for(k = 0; k<8; k++){
            results[k][i].R = (uint8_t)((row[j].R - pred[k](prevRow[j].R,row[j-1].R,prevRow[j-1].R))%256);
            results[k][i].G = (uint8_t)((row[j].G - pred[k](prevRow[j].G,row[j-1].G,prevRow[j-1].G))%256);
            results[k][i].B = (uint8_t)((row[j].B - pred[k](prevRow[j].B,row[j-1].B,prevRow[j-1].B))%256);
        }
        results[8][i] = Pixel(row[j].R, row[j].G, row[j].B);

        if(j == width){
            while(j>0){
                prevRow[j].R = row[j].R;
                prevRow[j].G = row[j].G;
                prevRow[j].B = row[j].B;
                j--;
            }
            j=0;
        }
        j++;
        i++;
    }
    for(i = 0; i<8; i++){
        cout<<"Entropie dla predykatu: " << i + 1<< " | ";
        getEntropy(results, pixelCount, i);
        cout<<endl;
    }
    cout<<"Entropie dla obrazka " << "     | ";
    getEntropy(results, pixelCount, 8);
    cout<<endl;
    cout << "Najlepszy predykat dla Red:" << sminR << " " << minR << endl;
    cout << "Najlepszy predykat dla Green:" << sminG << " " << minG << endl;
    cout << "Najlepszy predykat dla Blue:" << sminB << " " << minB << endl;
    cout << "Najlepszy predykat dla All:" << sminAll << " " << minAll << endl;

    inputFile.close();
}

Settings setSettings(string sett[], int size) {
    Settings settings;
    for (int i = 0; i < size; i++) {
        if (sett[i] == "-file" && i + 1 < size) {
            settings.inputFileName = sett[i+1];
        }
    }
    return settings;
}
uint8_t pred1(uint8_t n, uint8_t w, uint8_t nw){
    return w;
}
uint8_t pred2(uint8_t n, uint8_t w, uint8_t nw){
    return n;
}
uint8_t pred3(uint8_t n, uint8_t w, uint8_t nw){
    return nw;
}
uint8_t pred4(uint8_t n, uint8_t w, uint8_t nw){
    return n+w-nw;
}
uint8_t pred5(uint8_t n, uint8_t w, uint8_t nw){
    return n+(w-nw)/2;
}
uint8_t pred6(uint8_t n, uint8_t w, uint8_t nw){
    return w + (n-nw)/2;
}
uint8_t pred7(uint8_t n, uint8_t w, uint8_t nw){
    return (n+w)/2;
}
uint8_t pred8(uint8_t n, uint8_t w, uint8_t nw){
    if(nw >= max(w,n))
        return max(w,n);
    else if(nw <= min(w,n))
        return min(w,n);
    return w + n - nw;
}
void getEntropy(vector<vector<Pixel>> results, int size, int num) {
    vector <uint8_t> placeholder (3);
    map<uint8_t, int> weightsR;
    map<uint8_t , int> weightsB;
    map<uint8_t, int> weightsG;
    map<vector <uint8_t>, int> weightsPixel;
    map<uint8_t, int>::iterator itR;
    map<uint8_t , int> ::iterator itB;
    map<uint8_t, int> ::iterator itG;
    map<vector <uint8_t>, int> ::iterator itPixel;
    double answerR = 0;
    double answerB = 0;
    double answerG = 0;
    double answerPixel = 0;

    for(int i = 0; i<size; i++){
        weightsR[results[num][i].R]++;
        weightsG[results[num][i].G]++;
        weightsB[results[num][i].B]++;
        placeholder[0] = results[num][i].R;
        placeholder[1] = results[num][i].G;
        placeholder[2] = results[num][i].B;
        weightsPixel[placeholder]++;
    }

    for (itR = weightsR.begin(); itR != weightsR.end(); itR++)
        answerR += itR->second * log2(itR->second);
    answerR =  -(answerR / size - log2(size));

    for (itG = weightsG.begin(); itG != weightsG.end(); itG++)
        answerG += itG->second * log2(itG->second);
    answerG =  -(answerG / size - log2(size));

    for (itB = weightsB.begin(); itB != weightsB.end(); itB++)
        answerB += itB->second * log2(itB->second);
    answerB =  -(answerB / size - log2(size));

    for (itPixel = weightsPixel.begin(); itPixel != weightsPixel.end(); itPixel++)
        answerPixel += itPixel->second * log2(itPixel->second);
    answerPixel = -(answerPixel / size - log2(size));

    cout<< "All: " << answerPixel <<" | Red: "<< answerR << " | Green: " << answerG << " | Blue: " << answerB ;

    if(answerR < minR){
        minR = answerR;
        sminR = "predykator x";
        sminR[11] = ((char)(num+48+1));
    }
    if(answerG< minG){
        minG = answerG;
        sminG = "predykator x";
        sminG[11] = ((char)(num+48+1));
    }
    if(answerB < minB){
        minB = answerB;
        sminB = "predykator x";
        sminB[11] = ((char)(num+48+1));
    }
    if(answerPixel < minAll){
        minAll = answerR;
        sminAll = "predykator x";
        sminAll[11] = ((char)(num+48+1));
    }

}