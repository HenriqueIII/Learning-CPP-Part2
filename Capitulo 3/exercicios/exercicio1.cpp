#include <iostream>
#include <fstream>
#include "word.h"

const char * TERMINACOES[] = {"ança","ilia","ida","or","iz","is","ia","ão","ite","ã","il","gos",
"dade","im","undo","ol","al","em","oa","vo","ome","ora","ola","ul","um","ois","ais","isso","iço",
"ó","nde","lado","ita","ores","ê","inho","eza","esa","asa","aza","osa","ãe","ães","ento","aço",
"asso","ar","er","orte"};

class Rhyme : public Word {
    int sz;
public:
    Rhyme(const char * s = "") : Word((char*)s), sz(strlen(s)){ }

    int operator< (const Rhyme & r) const{
        for (int i = 1; i <= (this->sz > r.sz? r.sz : this->sz); ++i)
            if((*this)[sz-i] != r[r.sz-i])
                if( (*this)[sz-i] < r[r.sz-i])
                    return 1;
                else
                    return 0;
        // Se for igual tambem retorna 0
        return 0;
    }
};

int compara(Word &a,const char * &b) {
    for(int i = 1; i <= (strlen(a) > strlen(b) ? strlen(b) : strlen(a)); ++i){
        if(a[strlen(a)-i] != b[strlen(b)-i])
            return 0;
    }
    return 1;
}

Word getSilaba(Word & w){
    Word silaba;
    int i = 0;
    while(TERMINACOES[i]){
        if(compara(w, TERMINACOES[i])){
            silaba = TERMINACOES[i];
            return silaba;
        }
        ++i;
    }
    return silaba;
}

void printRhymes(Array<Rhyme> &a, Word &w){
    Word aux = getSilaba(w);
    const char * aux2 = aux.getStr();
    for (int i = 0; i < a.size(); ++i){
        if (compara(a[i], aux2)){
            std::cout << a[i] << std::endl;
        }
    }
}

int getLines(std::ifstream &documento){
    int numoflines = 0;
    char wordbuff[32];
    while(documento.getline(wordbuff, 31, '\n')){
        numoflines++;
    }
    documento.clear();
    documento.seekg(0, documento.beg);
    return numoflines;
}

void criaFile(std::ofstream & documento){
    char lista[][15]= {"gostar", "nada", "ninho", "vinho", "mar", "pinho", "pendurada"};
    for (int i = 0; i < 7; ++i)
        documento << lista[i] << std::endl;
}

void help(){
    std::cout << "Usage: exercicio1.exe <flag>\n\to flag - Writes the ordered list to a file";
    std::cout << "\n\t\t Expects a filename. Example:\n\t\t exercicio1.exe -o orderedlist.txt\n\n";
    std::cout << "\tr flag - Returns all the words that rhymes with the word provided";
    std::cout << "\n\t\t Expects a word to compare. Example:\n\t\t exercicio1.exe -r amar\n";
}

int main(int argc, char ** argv){
    char file[] = "palavras.txt";
    std::ifstream documento;

    documento.open(file);
    if (!documento) {
        std::ofstream doc(file, std::fstream::trunc);
        criaFile(doc);
        doc.close();
        documento.open(file);
    }
    int lines = getLines(documento);
    Array<Rhyme> listofwords(lines+1);
    char wordbuff[32];
    while(documento.getline(wordbuff, 31, '\n')){
        listofwords<<wordbuff;
    }
    documento.close();

    sort(listofwords);

    if (argc != 3) {
        help();
        return 0;
    }
    if (!strcmp(argv[1],"-o")){
        std::ofstream output (argv[2], std::fstream::trunc);
        for (int i = 0; i < listofwords.size(); ++i){
            output << listofwords[i];
            output << '\n';
        }
        output.close();
    }
    if (!strcmp(argv[1],"-r")){
        Word procura(argv[2]);
        printRhymes(listofwords, procura);
    }
    return 0;
}

