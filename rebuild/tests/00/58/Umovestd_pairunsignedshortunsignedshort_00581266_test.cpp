#include <cstdio>
struct Pr { unsigned short a; unsigned short b; };

// behaviour model: _Umove forwards to an uninitialized-move helper and
// returns the container (this). We model the helper as a move of [first,last)
// into dest, and confirm doit returns self and moves the elements.
static Pr* g_dest;
static void helper(Pr* first, Pr* last, Pr* dest){
    for(Pr* p=first; p!=last; ++p, ++dest){ *dest = *p; }
}
struct Vec { int marker; };
static Pr* doit(Vec* self, Pr* first, Pr* last, Pr* dest){
    helper(first,last,dest);
    return (Pr*)self;
}

int main(){
    Pr src[3] = {{1,2},{3,4},{5,6}};
    Pr dst[3] = {{0,0},{0,0},{0,0}};
    Vec v; v.marker=0x1234;
    Pr* r = doit(&v, src, src+3, dst);
    int ok = (r == (Pr*)&v);
    for(int i=0;i<3;i++){ ok &= (dst[i].a==src[i].a && dst[i].b==src[i].b); }
    if(ok) printf("UMOVE_PARITY_OK\n");
    else printf("FAIL\n");
    return 0;
}