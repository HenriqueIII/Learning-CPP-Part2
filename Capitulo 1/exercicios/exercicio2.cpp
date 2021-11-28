#include <iostream>
#include <iomanip>
#include <string.h>

class Time
{
public:
    enum
    {
        HOURS = 24,
        MINUTES = 60
    };
    typedef unsigned char TimeValue;
    Time(TimeValue h = 0, TimeValue m = 0)
    {
        hh = h;
        mm = m;
        normalize();
    }
    Time &operator-=(const Time &t)
    {
        if (hh < t.hh)
            hh += HOURS;
        if (mm < t.mm)
        {
            --hh;
            mm += MINUTES;
        }
        hh -= t.hh;
        mm -= t.mm;
        return *this;
    }
    Time operator-(const Time &t)
    {
        Time aux(*this);
        aux -= t;
        return aux;
    }
    // Operadores de comparação
    bool operator< (const Time &t) const {
        if (hh < t.hh)
            return true;
        if (hh == t.hh && mm < t.mm)
            return true;
        return false;
    }
    bool operator> (const Time &t) const {
        if (hh > t.hh)
            return true;
        if (hh == t.hh && mm > t.mm)
            return true;
        return false;
    }
    bool operator== (const Time &t) const {
        return !(*this < t || *this > t);
    }
    bool operator!= (const Time &t) const {
        return (*this < t || *this > t);
    }

    // Setters e Getters
    TimeValue getHours() const { return hh; }
    TimeValue getMinutes() const { return mm; }
    void setHours(TimeValue h) { hh = h % HOURS; }
    void setMinutes(TimeValue m) { mm = m % MINUTES; }

    friend std::istream &operator>>(std::istream &in, Time &t)
    {
        //Remover espaços antes do numero
        char c;
        while ((c = in.get())  == ' ');
        in.putback(c);

        // Retirar do istream toda a string que representa a hora.
        char buffer[6];
        in.getline(buffer, 6, ' ');

        // Retirar o primeiro numero do buffer
        char * token = strtok(buffer, ": ");

        if (token){ // se existir
            if (buffer[0] == 58){   // se buffer começa por ':'
                t.mm = std::atoi(token);    // colocar numero nos minutos
                t.hh = 0;                   // e hora a zero
                return in;
            }else{ 
                t.hh = std::atoi(token);    // colocar numero nos minutos
                token = strtok(NULL, ": "); // obter novo token
            }
        }
        if (token)  // se foi obtido mais um numero 
            t.mm = std::atoi(token); //colocar nos minutos
        else    // se nao colocar minutos a zero.
            t.mm = 0;    

        return in;
    }

private:
    TimeValue hh, mm;
    void normalize()
    {
        hh %= HOURS;
        mm %= MINUTES;
    }
};

Time operator+(const Time &t1, const Time &t2)
{   
    int min = t1.getMinutes() + t2.getMinutes();
    int hour = t1.getHours() + t2.getHours();
    if (min >= Time::MINUTES){
        hour++;min %= Time::MINUTES;
    }
    return Time(hour, min);
}

Time &operator+=(Time &t1, const Time &t2)
{
    t1 = t1 + t2;
    return t1;
}

Time operator^(const Time &t1, const Time &t2){
    int h = t1.getHours(), m = t1.getMinutes();
    int elap_h = 0, elap_m = 0;
    for (;(m % Time::MINUTES) != t2.getMinutes(); ++m, ++elap_m);
    if (m >= Time::MINUTES)
        ++h;
    for (;(h % Time::HOURS) != t2.getHours(); ++h, ++elap_h);
    return Time(elap_h, elap_m%Time::MINUTES);
}

std::ostream &operator<<(std::ostream &o, const Time &t)
{
    char buffer[3];
    char cFill = o.fill();
    return o << std::setfill('0') << std::setw(2) << std::to_string(t.getHours()) << ':' << std::setw(2) << std::to_string(t.getMinutes()) << std::setfill(cFill);
}

int main(int argc, char **argv)
{
    std::cout << "Escreva uma expressão da forma:\n"
                 "\t<expressão>::<operando><operador><operando> = \n"
                 "\t<operador> :: '+'|'-'\n"
                 "\t<operando> :: <horas> |\n"
                 "\t              <horas> ':' <minutos> |\n"
                 "\t              ':' <minutos>"
              << std::endl;
    Time t1, t2, t3(2,0), t4(6,0), t5(3,30);
    char op, igual;
    std::cin >> t1 >> op >> t2 >> igual;
    Time res , res2;
    switch (op)
    {
    case '+':
        res = t1 + t2;
        break;
    case '-':
        res = t1 - t2;
    }
    res2 = t1 ^ t2;
    std::cout << t1 << ' ' << op << ' ' << t2 << " = " << res << std::endl;
    std::cout << t1 << " % " << t2 << " = " << res2 << std::endl;
    std::cout << t3 << " < " << t4 << " = " << (t3<t4?"Verdade":"Falso") << std::endl;
    std::cout << t4 << " > " << t5 << " = " << (t4>t5?"Verdade":"Falso") << std::endl;
    std::cout << t4 << " == " << t4 << " = " << (t4==t4?"Verdade":"Falso") << std::endl;
    std::cout << t5 << " < " << t3 << " = " << (t5<t3?"Verdade":"Falso") << std::endl;
    std::cout << t3 << " == " << t4 << " = " << (t3==t4?"Verdade":"Falso") << std::endl;
    
    
    
}
