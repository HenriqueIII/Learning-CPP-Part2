#include "Str1.h"

void Str1::dimNormalize(size_t dimension){
    // duplicar **dim** em cada ampliação
    for (dim=DIM_MIN; dim < dimension || sz >= dim; dim*=2);
}

void Str1::init(const char * s, size_t size, size_t cap){
    sz = size;
    dimNormalize(cap+1);
    rep = new char[dim];
    memcpy(rep, s, sz+1);
}

void Str1::assign(const char * s, size_t size){
    if ( (sz = size) >= dim) {
        dimNormalize(sz+1);
        delete [] rep;
        rep = new char [dim];
    }
    memcpy(rep, s, sz+1);
}


void Str1::append(const char *s, size_t size) {
	size_t newSz = sz + size;
	if (newSz >= dim) {
		dimNormalize(newSz + 1);
		char *r = new char[dim];
		memcpy (r, rep, sz);
		delete[] rep;
		rep = r;
	}
	memcpy(rep + sz, s, size );
	rep[sz = newSz] = '\0';
}