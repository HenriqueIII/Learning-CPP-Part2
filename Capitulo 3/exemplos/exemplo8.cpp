// << Acesso a membros protegidos >>
class B {
    int aa; // Zona apenas acessivel pelos métodos de B.
protected:
    int bb; // Zona acessivel aos derivados de B.
public:
    int cc; // Zona Publica
};

class D : public B {
public:     // zona publica
    int dd;
    void f() {
//      aa = 0;     // Erro. membro aa é privado da class A
        bb = 1;     // OK. membro bb é protegido da classe A
        cc = 1;     // OK. membro cc é publico da classe A
    }
};

int main (int argc, char ** argv) {
    B b;
    D d;
// b.aa = 0;   // Erro. aa é privado de B
// b.bb = 1;   // Erro. bb é protegido de B
    b.cc = 2;   // OK. cc é publico de B
//  b.dd = 3;   // Erro. dd não é membro de B
//  d.aa = 4;   // Erro. aa não é acessivel a D
//  d.bb = 5;   // Erro. bb mantem-se protegido em D
    d.cc = 6;   // OK. cc mantem-se publico em D
    d.dd = 7;   // OK. dd é publico em D
    return 0;
}