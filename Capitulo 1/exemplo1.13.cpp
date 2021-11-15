#include <iostream>
#include <string.h>
#include <iomanip>

class String {
    static const unsigned MAX_LEN = 128;
    char s[MAX_LEN];
    void assign ( const char *ss ) {
        if ( strlen (ss) < MAX_LEN)
            strcpy(s, ss);
        else {
            memcpy(s, ss, MAX_LEN-1);
            s[MAX_LEN-1] = '\0';    
        }
    }
public:
    String()                            {s[0]='\0';                 }
    String(const char *ss)              {assign(ss);                }
    String &operator=(const char *ss)   {assign(ss); return *this;  }
    // Sobrecarga do operador indexação "[]".
    // (1) Utilizado em strings não const.
    char &operator[] (int i)            {return s[i];               }
    // (2) Utilizado em strings const.
    char operator[] (int i) const       {return s[i];               }
    // Retorna uma string C-style.
    const char *c_str() const           {return s;                  }
    // Sobrecarga dos operadores de comparação "<" e "==".
    bool operator<(const String &s2) const {
        return strcmp(s, s2.s) < 0;
    }
    bool operator==(const String &s2) const{
        return strcmp(s, s2.s) == 0;
    }
    // Função global sobrecarga do operador extrator de
    // istream
    friend std::istream &operator>>(std::istream &in, String &s) {
        return in >> std::setw(String::MAX_LEN) >> s.s;
    }
};

// Função global sobrecarga do operador insersor em ostream.
inline std::ostream &operator<<(std::ostream &out, const String &s) {
    return out << s.c_str();
}

void toUpper(String &s) {
    int i = 0; char ch;
    do {
        ch = toupper(s[i]); // rvalue - usa (1), s não é const
        s[i++] = ch;        // lvalue - usa (1), s não é const
    }while (ch);
}

void toLower(String &s, const String &ss){
    int i=0; char ch;
    do{
        ch = tolower(ss[i]); // usa (2), porque ss é const
        s[i++] = ch;         // usa (1), porque s não é const
    }while (ch);
}

void compare(const String &s1, const String &s2) {
    std::cout << s1;        // Escreve a string s1
    // Escreve relação entre duas strings;
    if (s1 < s2)
        std::cout << " é menor que ";
    else if (s2 < s1)
        std::cout << " é maior que ";
    else
        std::cout << " é igual a ";
    std::cout << s2 << "\n"; // Escreve a string s2
}

int main (int argc, char ** argv) {
    String s1("Hello World"), s2;
    toLower(s2, s1); compare(s1, s2);
    toUpper(s2); compare(s1, s2);
    s2 = s1; compare(s1, s2);
    return 0;
}