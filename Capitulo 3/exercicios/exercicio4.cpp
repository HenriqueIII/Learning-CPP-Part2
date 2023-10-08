/* Declarar uma classe base abstrata Form com um único método virtual puro
show(). Derivar diretamente dessa classe, as classes concretas Line,
Sqaure e Circle, tais que quando posta em execução a função main mostre no ecrã:
Forma circular
Forma linear
Forma quadrada*/

#include <iostream>
#include <string>

class Form{
    std::string nome;
public:
    Form(std::string nn): nome(nn) {}
    std::string getNome() const { return nome; }
    virtual void show()=0;
};

class Line: public Form{
public:
    Line() : Form("Forma Linear") {}
    void show() {
        std::cout << getNome() << std::endl;
    }
};
class Square: public Form{
public:
    Square() : Form("Forma Quadrada") {}
    void show() {
        std::cout << getNome() << std::endl;
    }
    
};
class Circle: public Form{
public:
    Circle() : Form("Forma Circular") {}
    void show() {
        std::cout << getNome() << std::endl;
    }
    
};

int main(int argc, char ** argv){
    Line * line = new Line;
    Square * square = new Square;
    Circle * circle = new Circle;
    Form * array[] = { circle, line, square };
    for (int i = 0; i < 3; ++i){
        array[i]->show();
    }
}