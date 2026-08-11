#include <cstdio>
struct Pair { int x; };

static void* g_self;
static Pair* g_arg1;
static Pair* g_arg2;

struct Storage {
    int marker;
    void Construct(Pair* a, Pair* b);
};
void Storage::Construct(Pair* a, Pair* b){ g_self=this; g_arg1=a; g_arg2=b; }

struct Alloc {
    char pad[0x90];
    Storage storage_area;
    void ConsVal(Pair* a, Pair* b);
};
void Alloc::ConsVal(Pair* a, Pair* b){ this->storage_area.Construct(b, a); }

int main(){
    static Alloc obj;
    Pair pa={11}, pb={22};
    obj.ConsVal(&pa,&pb);
    // Construct(b,a): arg1=b(&pb), arg2=a(&pa), self=&storage_area
    bool ok = (g_self==(void*)&obj.storage_area) && (g_arg1==&pb) && (g_arg2==&pa);
    if(ok) printf("006b83f0_TEST PASS\n"); else printf("FAIL\n");
    return 0;
}