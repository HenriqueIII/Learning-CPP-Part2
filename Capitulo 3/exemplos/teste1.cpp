// Testar ambiguidade entre derivado de várias classes

struct B1 {
    int b;
    B1() : b(1) { }
};

struct B2 {
    int b;
    B2() : b(2) { }
};

struct D : public B1, public B2 {
    void f(D &d) {
        int x;
        // x = d.b;            // Erro. Nao consegue resolver b
        x = d.B1::b;        // OK. x = 1
        x = d.B2::b;        // OK. x = 2
    }
};

class F {
public:
    int x;
};

class F1 : public F {/*     */};
class F2 : public F {/*     */};

class G : public F1, public F2 {
    void f(G &g) {
        int x;
        // x = g.x;        // Erro. Ambiguidade
        x = g.F1::x;    // OK. membro i herdado de F1
        x = g.F2::x;    // OK. membro i hendade de F2
    }
};