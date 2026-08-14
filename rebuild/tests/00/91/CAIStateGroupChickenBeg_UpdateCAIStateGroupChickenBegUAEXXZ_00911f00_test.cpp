#include <cstdio>

// Behaviour model of the forwarder chain: f1(this)->A, f2(A)->B, f3(B)->C (unused),
// then f4(this). Verify call order + that this is preserved for the final call.

static int log_order = 0;
static void* seen_this_f1 = 0;
static void* seen_this_f4 = 0;
static int order_f1=0, order_f2=0, order_f3=0, order_f4=0;

struct A { int tag; };
struct B { int tag; };
struct C { int tag; };
static A gA; static B gB; static C gC;

static A* f1(void* self){ seen_this_f1=self; order_f1=++log_order; return &gA; }
static B* f2(A* a){ order_f2=++log_order; return (a==&gA)?&gB:0; }
static C* f3(B* b){ order_f3=++log_order; return (b==&gB)?&gC:0; }
static void f4(void* self){ seen_this_f4=self; order_f4=++log_order; }

struct Self {
    void Update(){ f3(f2(f1(this))); f4(this); }
};

int main(){
    Self s;
    s.Update();
    bool ok = (order_f1==1 && order_f2==2 && order_f3==3 && order_f4==4)
           && (seen_this_f1==(void*)&s) && (seen_this_f4==(void*)&s);
    if(ok) printf("CHICKENBEG_UPDATE_OK\n");
    else   printf("FAIL o=%d,%d,%d,%d\n",order_f1,order_f2,order_f3,order_f4);
    return 0;
}