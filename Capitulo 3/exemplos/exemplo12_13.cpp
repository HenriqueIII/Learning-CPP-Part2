#include <iostream>
#include <string>
#include <vector>

class Entry{
    std::string name;
public:
    Entry (std::string nm) : name(nm) { }
    std::string & getName() { return name; }
    virtual unsigned getSize() = 0;
    virtual void print ( int ident = 0) {
        for (int i = 0; i < ident; ++i)
            std::cout << "   ";
        std::cout << name << " (" << getType() << ") : " << getSize() << " bytes." << std::endl;
    }
    virtual char * getType() = 0;
    virtual Entry * find (std::string nm) {
        return nm == name ? this : NULL;
    }
};

class File: public Entry {
private:
    unsigned dim;
public:
    File(std::string nm, unsigned size) : Entry(nm), dim(size) {}
    unsigned getSize() { return dim; }
    char * getType() { return (char *) "File"; }
};

class Directory: public Entry {
private:
    std::vector <Entry *> entrys;
public:
    Directory(std::string nm) : Entry (nm) { }
    char * getType() { return (char *) "Dir"; }
    void dir()  { print(0); }
    void add(Entry * e) { entrys.push_back(e); }
    unsigned getSize();
    Entry * find(std::string nm);
    void print( int ident = 0);
};

unsigned Directory::getSize() {
    unsigned res = 0;
    for (unsigned i = 0; i < entrys.size(); ++i)
        res += entrys[i]->getSize();
    return res;
}

Entry * Directory::find(std::string nm) {
    Entry * res = Entry::find(nm);
    for (unsigned i=0; !res && i <entrys.size(); ++i)
        res = entrys[i]->find(nm);
    return res;
}

void Directory::print(int ident) {
    Entry::print(ident);
    for (unsigned i=0; i < entrys.size(); ++i) {
        entrys[i]->print(ident+1);
    }
}

int main (int argc, char ** argv){
    Directory d1("root"), d2("FileSystem");
    File f1("abc.txt", 123), f2("File.cpp", 230);
    File f3("Directory.h", 547), f4("Entry.h", 54);
    d1.add(&f1); d1.add(&d2);
    d2.add(&f2); d2.add(&f3); d2.add(&f4);
    d1.dir(); std::cout << std::endl;
    Entry *e = d1.find("File.cpp");
    if (e) { std:: cout << "OK: "; e->print(); }
}