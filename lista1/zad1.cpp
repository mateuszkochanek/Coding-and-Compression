#include <iostream>
#include <iostream>
#include <fstream>
#include <map>
#include <cmath>
using namespace std;

double entropy (map <char , int> mapAll,int count){
    double ent = 0;
    for (std::map<char,int>::iterator it=mapAll.begin(); it!=mapAll.end(); it++){
        ent+=(it->second)*(log2(it->second)-log2(count));
    }
    ent/=(double)(-count);
    return ent;
}

double conditional_entropy(map <string , int> mapPrev, map <char , int> mapAll, int count){
    double ent = 0;
    for (std::map<string,int>::iterator it=mapPrev.begin(); it!=mapPrev.end(); it++){
        if(mapAll[(it->first)[0]] != 0)
            ent+=(it->second)*(log2(it->second)-log2(mapAll[(it->first)[0]]));
    }
    ent/=(double)(-count);
    return ent;
}


int main(int argc, char *argv[])
{
    if(argc >= 2){
        int count = 0;
        char ch,chBefore = 0;
        string key="00";
        map <char , int> mapAll;
        map <string , int> mapPrev;
        ifstream myReadFile;
        myReadFile.open(argv[1]);
        while(myReadFile >> noskipws >> ch){
            key[0] = chBefore;
            key[1] = ch;
            //cout << key << " " << ch << " " << chBefore << endl;
            count++;
            mapAll[ch]++;
            mapPrev[key]++;
            chBefore = ch;
            key="00";
        }
        int mapAllSize = mapAll.size(), mapPrevSize = mapPrev.size();
        cout.precision(10);
        double condEntr = conditional_entropy(mapPrev,mapAll,count), entr = entropy(mapAll,count);
        cout << "Entropia: " << fixed << entr << endl;
        cout << "Entropia warunkowa: " << fixed << condEntr << endl;
        cout << "Różnica: " << fixed << entr - condEntr << endl;

    } else {
        cout << "Nie podano parametrów";
    }
    return 0;
}
